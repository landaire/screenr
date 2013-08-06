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
    appSettings = std::shared_ptr<QSettings>(new QSettings("lander", "unnamed"));

    // Get the credentials from the settings
    appSettings->beginGroup("AWS Credentials");
    userCredentials.BucketName = appSettings->value("Bucket", "").toString().toStdString();
    userCredentials.AccessKeyId = appSettings->value("ID", "").toString().toStdString();
    userCredentials.AccessSecretKey = appSettings->value("Secret", "").toString().toStdString();
    appSettings->endGroup();

    ui->lineS3Bucket->setText(userCredentials.BucketName.c_str());
    ui->lineS3Id->setText(userCredentials.AccessKeyId.c_str());
    ui->lineS3Secret->setText(userCredentials.AccessSecretKey.c_str());

    connect(ui->lineS3Bucket, SIGNAL(editingFinished()), this, SLOT(lineBucketEditingFinished()));
    connect(ui->lineS3Id, SIGNAL(editingFinished()), this, SLOT(lineIdEditingFinished()));
    connect(ui->lineS3Secret, SIGNAL(editingFinished()), this, SLOT(lineSecretEditingFinished()));

    captureUtil = std::unique_ptr<CaptureUtils>(new CaptureUtils(QDir::tempPath().toStdString()));
    captureUtil->AwsCredentials = userCredentials;
    captureUtil->SetupHotkeys();
}

//TODO: Add text input masks/validation (invalid chars, lengths, etc.)
void MainWindow::lineBucketEditingFinished( void )
{
    appSettings->beginGroup("AWS Credentials");
    appSettings->setValue("Bucket", ui->lineS3Bucket->text());
    appSettings->endGroup();
    userCredentials.BucketName = ui->lineS3Bucket->text().toStdString();
    captureUtil->AwsCredentials = userCredentials;
}

void MainWindow::lineIdEditingFinished( void )
{
    appSettings->beginGroup("AWS Credentials");
    appSettings->setValue("ID", ui->lineS3Id->text());
    appSettings->endGroup();
    userCredentials.AccessKeyId = ui->lineS3Id->text().toStdString();
    captureUtil->AwsCredentials = userCredentials;
}

void MainWindow::lineSecretEditingFinished( void )
{
    appSettings->beginGroup("AWS Credentials");
    appSettings->setValue("Secret", ui->lineS3Secret->text());
    appSettings->endGroup();
    userCredentials.AccessSecretKey = ui->lineS3Secret->text().toStdString();
    captureUtil->AwsCredentials = userCredentials;
}

MainWindow::~MainWindow()
{
    delete ui;
}
