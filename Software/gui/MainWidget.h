#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QSizePolicy>

#include "CameraArea.h"

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent);

public slots:
    void onModeChanged(int mode);

private:
    QPushButton* returnBtn = nullptr;
    QLabel* label = nullptr;
    QPushButton* cameraBtn = nullptr;
    QPushButton* registerBtn = nullptr;
    QPushButton* checkBtn = nullptr;
    QWidget* widget = nullptr;
    CameraArea* cameraArea = nullptr;

    void setupUI();

private slots:
    void onCameraBtnClicked(bool checked);
    void onRegisterBtnClicked();
    void onCheckBtnClicked();
    void onReturnBtnClicked();
};
