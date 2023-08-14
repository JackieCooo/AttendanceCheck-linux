#pragma once

#include <QThread>
#include <QMutex>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class CameraCaptureThread : public QThread {
    Q_OBJECT

signals:
    void frameCaptured(cv::Mat img);

public:
    void run() override;
    void stop();
    Mat getImage();

private:
    bool running = false;
    QMutex mutex;
    VideoCapture cap;
    Mat img;
};
