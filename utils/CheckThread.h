#pragma once

#include <string>

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QString>

#include <opencv2/opencv.hpp>

#include <dlib/image_processing.h>
#include <dlib/opencv.h>

#include "FaceUtils.h"

using namespace dlib;
using namespace cv;
using namespace std;

class CheckThread : public QThread {
    Q_OBJECT

signals:
    void requestImage(int id);
    void userChecked(QString name);

public:
    void run() override;
    void stop();

public slots:
    void onImageGet(cv::Mat mat);

private:
    bool running = false;
    QMutex mutex;
    QWaitCondition imageGot;
    Mat img;
};
