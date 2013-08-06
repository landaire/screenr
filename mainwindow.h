#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "captureutils.h"
#include <QSettings>
#include "aws.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

    std::unique_ptr<CaptureUtils> captureUtil;
    std::shared_ptr<QSettings> appSettings;
    AWS::AWSCredentials userCredentials;

private slots:
    void lineBucketEditingFinished( void );
    void lineIdEditingFinished( void );
    void lineSecretEditingFinished( void );
};

#endif // MAINWINDOW_H
