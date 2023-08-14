#pragma once

#include <QMainWindow>

#include "MainWidget.h"

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent);

private:
    MainWidget* mainWidget = nullptr;

    void setupUI();
};
