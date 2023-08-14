#include "CameraCaptureThread.h"

void CameraCaptureThread::run() {
    Mat origin;

    try {
        cap.open(0);
        cap.set(CAP_PROP_FRAME_WIDTH, 640);
        cap.set(CAP_PROP_FRAME_HEIGHT, 480);
        cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    }
    catch (Exception& e) {
        cap.release();
        return;
    }

    running = true;

    for(;;) {
        cap >> origin;
        cvtColor(origin, img, COLOR_BGR2RGB);
        emit(frameCaptured(img));

        mutex.unlock();
        if (!running) {
            cap.release();
            return;
        }

        msleep(40);
    }
}

void CameraCaptureThread::stop() {
    mutex.lock();
    running = false;
}

Mat CameraCaptureThread::getImage() {
    return this->img;
}
