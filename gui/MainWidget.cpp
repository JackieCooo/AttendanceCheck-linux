#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void MainWidget::setupUI() {
    auto* vLayout = new QVBoxLayout(this);
    vLayout->setAlignment(Qt::AlignCenter);

    returnBtn = new QPushButton("< Back", this);
    connect(returnBtn, SIGNAL(clicked(bool)), this, SLOT(onReturnBtnClicked()));
    vLayout->addWidget(returnBtn);

    label = new QLabel(this);
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    label->setFixedHeight(50);
    label->setFont(QFont("Consolas", 14, QFont::Normal));
    label->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(label);

    cameraArea = new CameraArea(this);
    connect(cameraArea, SIGNAL(modeChanged(int)), this, SLOT(onModeChanged(int)));
    connect(cameraArea, SIGNAL(runningStatus(QString)), label, SLOT(setText(QString)));
    vLayout->addWidget(cameraArea);

    cameraBtn = new QPushButton("Open Camera", this);
    cameraBtn->setCheckable(true);
    cameraBtn->setChecked(false);
    connect(cameraBtn, SIGNAL(toggled(bool)), this, SLOT(onCameraBtnClicked(bool)));
    vLayout->addWidget(cameraBtn);

    widget = new QWidget(this);
    widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    auto* hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(0, 0, 0, 0);

    registerBtn = new QPushButton("Register", widget);
    registerBtn->setEnabled(false);
    connect(registerBtn, SIGNAL(clicked(bool)), this, SLOT(onRegisterBtnClicked()));
    hLayout->addWidget(registerBtn);

    checkBtn = new QPushButton("Check", widget);
    checkBtn->setEnabled(false);
    connect(checkBtn, SIGNAL(clicked(bool)), this, SLOT(onCheckBtnClicked()));
    hLayout->addWidget(checkBtn);

    vLayout->addWidget(widget);

    this->setLayout(vLayout);
}

void MainWidget::onCameraBtnClicked(bool checked) {
    if (checked) {
        cameraArea->setMode(1);
        cameraBtn->setText("Close Camera");
    }
    else {
        cameraArea->setMode(0);
        cameraBtn->setText("Open Camera");
    }
}

void MainWidget::onRegisterBtnClicked() {
    cameraArea->setMode(2);
}

void MainWidget::onCheckBtnClicked() {
    cameraArea->setMode(3);
}

void MainWidget::onModeChanged(int mode) {
    if (mode == 0) {
        registerBtn->setEnabled(false);
        checkBtn->setEnabled(false);
    }
    else if (mode == 1) {
        registerBtn->setEnabled(true);
        checkBtn->setEnabled(true);
    }
    else if (mode == 2) {
        checkBtn->setEnabled(false);
    }
    else if (mode == 3) {
        registerBtn->setEnabled(false);
    }
}

void MainWidget::onReturnBtnClicked() {
    int mode = cameraArea->getMode();
    if (mode == 0 || mode == 1) return;
    else if (mode == 2 || mode == 3) cameraArea->returnNormalMode();
}
