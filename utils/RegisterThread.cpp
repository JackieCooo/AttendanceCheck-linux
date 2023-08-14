#include "RegisterThread.h"

void RegisterThread::run() {
    running = true;

    for (;;) {
        emit(requestImage(0));
        mutex.lock();
        imageGot.wait(&mutex, 1000);
        matrix<dlib::rgb_pixel> mat;
        full_object_detection face;
        assign_image(mat, cv_image<dlib::rgb_pixel>(img));
        int ret = faceUtils.detectFace(mat, face, true);
        mutex.unlock();

        emit(detectResult(ret));
        if (ret == 0) {
            mutex.lock();
            nameGot.wait(&mutex);
            if (!this->name.isEmpty()) {
                matrix<float, 0, 1> des;
                faceUtils.getFaceDescriptor(mat, face, des);
                faceUtils.saveFaceData(this->name.toStdString(), des);
                emit(userRegistered(this->name));
                running = false;
            }
            mutex.unlock();
        }

        mutex.unlock();
        if (!running) {
            return;
        }

        msleep(10);
    }
}

void RegisterThread::stop() {
    mutex.lock();
    running = false;
}

void RegisterThread::onImageGet(cv::Mat mat) {
    this->img = mat(Rect((640 - 400) / 2, (480 - 400) / 2, 400, 400));
    imageGot.wakeAll();
}

void RegisterThread::onNameGet(QString name) {
    this->name = name;
    nameGot.wakeAll();
}
