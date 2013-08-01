#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    captureUtil = std::unique_ptr<CaptureUtils>(new CaptureUtils());
    captureUtil->SetupHotkeys();
}

MainWindow::~MainWindow()
{
    delete ui;
}
