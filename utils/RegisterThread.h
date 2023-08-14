#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QString>

#include <opencv2/opencv.hpp>

#include <dlib/image_processing.h>
#include <dlib/opencv.h>
#include <dlib/dnn.h>

#include "FaceUtils.h"

using namespace cv;
using namespace dlib;
using namespace std;

class RegisterThread : public QThread {
    Q_OBJECT

signals:
    void requestImage(int id);
    void detectResult(int ret);
    void userRegistered(QString name);

public:
    void run() override;
    void stop();

public slots:
    void onImageGet(cv::Mat mat);
    void onNameGet(QString name);

private:
    bool running = false;
    QMutex mutex;
    QWaitCondition imageGot;
    QWaitCondition nameGot;
    Mat img;
    QString name;
};
