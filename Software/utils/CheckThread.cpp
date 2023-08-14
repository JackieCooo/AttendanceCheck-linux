#include "CheckThread.h"

void CheckThread::run() {
    running = true;

    for(;;) {
        emit(requestImage(1));

        mutex.lock();
        imageGot.wait(&mutex, 1000);
        matrix<dlib::rgb_pixel> mat;
        full_object_detection face;
        assign_image(mat, cv_image<dlib::rgb_pixel>(img));
        int ret = faceUtils.detectFace(mat, face);
        if (ret == 0) {
            matrix<float, 0, 1> des;
            string name;
            faceUtils.getFaceDescriptor(mat, face, des);
            if (faceUtils.recogniseFace(des, name) != -1) {
                emit(userChecked(QString(QString::fromLocal8Bit(name.c_str()))));
                running = false;
            }
        }
        mutex.unlock();

        if (!running) {
            return;
        }
    }
}

void CheckThread::stop() {
    mutex.lock();
    running = false;
}

void CheckThread::onImageGet(cv::Mat mat) {
    this->img = mat(Rect((640 - 400) / 2, (480 - 400) / 2, 400, 400));
    imageGot.wakeAll();
}
