#include "mainwindow.h"
#include <QApplication>

std::string CaptureUtils::AwsKeyId = "";
std::string CaptureUtils::AwsSecretKey = "";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
