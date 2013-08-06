#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir> // for temp path
#include <QtMacExtras/QMacNativeToolBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // makes the toolbar look like "not shit" as I'd describe it
    QtMacExtras::setNativeToolBar(ui->toolBar);
    captureUtil = std::unique_ptr<CaptureUtils>(new CaptureUtils(QDir::tempPath().toStdString()));
    captureUtil->SetupHotkeys();
}

MainWindow::~MainWindow()
{
    delete ui;
}
