#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir> // for temp path

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    captureUtil = std::unique_ptr<CaptureUtils>(new CaptureUtils(QDir::tempPath().toStdString()));
    captureUtil->SetupHotkeys();
}

MainWindow::~MainWindow()
{
    delete ui;
}
