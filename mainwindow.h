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

class QPropertyAnimation;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void resizeWithAnimation( const QSize& size);
    
private:
    Ui::MainWindow *ui;

    std::unique_ptr<CaptureUtils> captureUtil;
    std::shared_ptr<QSettings> appSettings;
    AWS::AWSCredentials userCredentials;
    std::unique_ptr<QActionGroup> toolBarGroup;
    std::unique_ptr<QPropertyAnimation> animation; // used for changing the window size

    void setToolBarGroup( void );
    void setPageIndex(int index);

private slots:
    void changeStackedIndex(bool checked);

    void lineBucketEditingFinished( void );
    void lineIdEditingFinished( void );
    void lineSecretEditingFinished( void );
};

#endif // MAINWINDOW_H
