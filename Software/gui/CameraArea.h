#pragma once

#include <QWidget>
#include <QSizePolicy>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QFontMetrics>
#include <QString>
#include <QColor>
#include <QInputDialog>

#include <opencv2/opencv.hpp>

#include "CameraCaptureThread.h"
#include "RegisterThread.h"
#include "CheckThread.h"

using namespace cv;

class CameraArea : public QWidget {
    Q_OBJECT

signals:
    void imageToRegisterThread(cv::Mat img);
    void imageToCheckedThread(cv::Mat img);
    void nameGot(QString name);
    void modeChanged(int mode);
    void runningStatus(QString text);

public:
    explicit CameraArea(QWidget* parent);
    ~CameraArea() override;
    int getMode() const;

public slots:
    void setMode(const int& mode);
    void returnNormalMode();

protected:
    void paintEvent(QPaintEvent* e) override;

private:
    QImage img;
    CameraCaptureThread camThread;
    RegisterThread regThread;
    CheckThread checkThread;
    int mode = 0;  // 0: Standby, 1: Normal, 2: Register, 3: Check

    void setupUI();
    void setupListener();

private slots:
    void onCameraCaptured(cv::Mat img);
    void onImageRequest(int id);
    void onDetectResultGet(int ret);
    void onRegistered(QString name);
    void onChecked(QString name);
    void onCameraClosed();
};
