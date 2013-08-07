#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "captureutils.h"
#include <QSettings>
#include "aws.h"
#include <QActionGroup>

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
    std::unique_ptr<QActionGroup> toolBarGroup;

    void setToolBarGroup( void );

private slots:
    void toolBarS3Clicked(bool checked );
    void toolBarAboutClicked( bool checked );

    void lineBucketEditingFinished( void );
    void lineIdEditingFinished( void );
    void lineSecretEditingFinished( void );
};

#endif // MAINWINDOW_H
