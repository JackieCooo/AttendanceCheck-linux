#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
}

void MainWindow::setupUI() {
    this->resize(800, 600);
    this->setWindowTitle("AttendanceCheck");

    mainWidget = new MainWidget(this);
    this->setCentralWidget(mainWidget);
}
