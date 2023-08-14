#include "CameraArea.h"

CameraArea::CameraArea(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupListener();
}

void CameraArea::setupUI() {
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFixedSize(400, 400);
}

void CameraArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QPen pen(Qt::SolidLine);
    pen.setWidth(1);
    painter.setPen(pen);
    QBrush brush(Qt::NoBrush);
    painter.setBrush(brush);
    QFont font("Consolas", 18, QFont::Normal);
    painter.setFont(font);

    if (camThread.isRunning()) {
        painter.drawImage(event->rect(), this->img);
    }
    else {
        QFontMetrics fm(font);
        QString s("Camera Closed");
        int textWidth = fm.horizontalAdvance(s);
        int textHeight = fm.height();
        painter.drawText((400 - textWidth) / 2, (400 - textHeight) / 2 + textHeight, s);
    }

    if (this->mode == 2) {
        QImage mask(event->rect().width(), event->rect().height(), QImage::Format_ARGB32_Premultiplied);
        QPainter maskPainter(&mask);

        maskPainter.setRenderHints(QPainter::Antialiasing);
        maskPainter.setPen(QPen(Qt::NoPen));
        maskPainter.setBrush(QBrush(QColor(127, 127, 127)));
        maskPainter.drawRect(event->rect());
        maskPainter.setBrush(QBrush(QColor(255, 255, 255)));
        maskPainter.setCompositionMode(QPainter::CompositionMode_SourceOut);
        maskPainter.drawEllipse(QPoint(event->rect().width() / 2, event->rect().height() / 2), 200, 200);

        painter.drawImage(event->rect(), mask);
    }

    painter.drawRect(event->rect());
}

void CameraArea::setupListener() {
    connect(&camThread, SIGNAL(frameCaptured(cv::Mat)), this, SLOT(onCameraCaptured(cv::Mat)), Qt::DirectConnection);
    connect(&camThread, SIGNAL(finished()), this, SLOT(onCameraClosed()));

    connect(&regThread, SIGNAL(requestImage(int)), this, SLOT(onImageRequest(int)));
    connect(this, SIGNAL(imageToRegisterThread(cv::Mat)), &regThread, SLOT(onImageGet(cv::Mat)), Qt::DirectConnection);
    connect(&regThread, SIGNAL(detectResult(int)), this, SLOT(onDetectResultGet(int)));
    connect(this, SIGNAL(nameGot(QString)), &regThread, SLOT(onNameGet(QString)));
    connect(&regThread, SIGNAL(finished()), this, SLOT(returnNormalMode()));
    connect(&regThread, SIGNAL(userRegistered(QString)), this, SLOT(onRegistered(QString)));

    connect(&checkThread, SIGNAL(requestImage(int)), this, SLOT(onImageRequest(int)));
    connect(this, SIGNAL(imageToCheckedThread(cv::Mat)), &checkThread, SLOT(onImageGet(cv::Mat)), Qt::DirectConnection);
    connect(&checkThread, SIGNAL(userChecked(QString)), this, SLOT(onChecked(QString)));
    connect(&checkThread, SIGNAL(finished()), this, SLOT(returnNormalMode()));
}

void CameraArea::onCameraCaptured(cv::Mat img) {
    QImage tmp(img.data, img.cols, img.rows, img.cols * img.channels(), QImage::Format_RGB888);
    tmp.copy((640 - 400) / 2, (480 - 400) / 2, 400, 400);
    this->img = tmp;
    update();
}

void CameraArea::setMode(const int &mode) {
    if (this->mode == mode) return;

    if (mode == 0) {
        if (this->mode == 2) {
            regThread.stop();
        }
        else if (this->mode == 3) {
            checkThread.stop();
        }

        camThread.stop();
    }
    else if (mode == 1) {
        if (this->mode == 0) {
            camThread.start();
        }
        else if (this->mode == 2) {
            regThread.stop();
        }
        else if (this->mode == 3) {
            checkThread.stop();
        }
    }
    else if (mode == 2) {
        if (this->mode != 1) return;
        else {
            regThread.start();
            update();
        }
    }
    else if (mode == 3) {
        if (this->mode != 1) return;
        else {
            checkThread.start();
        }
    }

    this->mode = mode;
    emit(modeChanged(this->mode));
}

void CameraArea::onImageRequest(int id) {
    if (id == 0) {
        emit(imageToRegisterThread(camThread.getImage()));
    }
    else if (id == 1) {
        emit(imageToCheckedThread(camThread.getImage()));
    }
}

CameraArea::~CameraArea() {
    if (regThread.isRunning()) regThread.stop();
    if (camThread.isRunning()) camThread.stop();
    if (checkThread.isRunning()) checkThread.stop();
}

void CameraArea::onDetectResultGet(int ret) {
    if (ret == 0) {
        emit(runningStatus("Face detected"));
        bool ok;
        QString name = QInputDialog::getText(nullptr, "Please enter your name", "Name: ", QLineEdit::Normal, QString(), &ok);
        if (ok && !name.isEmpty()) {
            emit(nameGot(name));
        }
        else {
            emit(nameGot(""));
        }
    }
    else if (ret == 1) {
        emit(runningStatus("No face detected"));
    }
    else if (ret == 2) {
        emit(runningStatus("Multiple faces detected"));
    }
    else if (ret == 3) {
        emit(runningStatus("Face out of bound"));
    }
}

void CameraArea::returnNormalMode() {
    this->setMode(1);
}

void CameraArea::onRegistered(QString name) {
    QString s;
    s += "User ";
    s += name;
    s += " registered";

    emit(runningStatus(s));
}

void CameraArea::onChecked(QString name) {
    QString s;
    s += "User ";
    s += name;
    s += " checked";

    emit(runningStatus(s));
}

int CameraArea::getMode() const {
    return this->mode;
}

void CameraArea::onCameraClosed() {
    update();
}
