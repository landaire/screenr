#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir> // for temp path
#include <QtMacExtras/QMacNativeToolBar>
#include <QPropertyAnimation>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    //this->setMinimumSize(ui->pageGeneral->sizeHint());
    //this->setMaximumSize(ui->pageGeneral->sizeHint());
    ui->pageGeneral->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->pageGeneral->adjustSize();
    adjustSize();
    animation = std::unique_ptr<QPropertyAnimation>(new QPropertyAnimation(this, "size"));

    // remove min/maximize buttons and fullscreen buttons
    this->setWindowFlags((this->windowFlags() & (~(Qt::WindowMinMaxButtonsHint | Qt::WindowFullscreenButtonHint))));
    ui->stackedWidget->setCurrentIndex(0);

    toolBarGroup = std::unique_ptr<QActionGroup>(new QActionGroup(this));
    setToolBarGroup();
    // makes the toolbar look like "not shit" as I'd describe it
    QtMacExtras::setNativeToolBar(ui->toolBar);
    appSettings = std::shared_ptr<QSettings>(new QSettings(QSettings::NativeFormat, QSettings::UserScope, "lander", "unnamed"));

    // Get the credentials from the settings
    appSettings->beginGroup("AWS Credentials");
    userCredentials.BucketName = appSettings->value("Bucket", "").toString().toStdString();
    userCredentials.AccessKeyId = appSettings->value("ID", "").toString().toStdString();
    userCredentials.AccessSecretKey = appSettings->value("Secret", "").toString().toStdString();
    appSettings->endGroup();

    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(stackedWidgetPageChanged(int)));

    // Toolbar stuff
    ui->lineS3Bucket->setText(userCredentials.BucketName.c_str());
    ui->lineS3Id->setText(userCredentials.AccessKeyId.c_str());
    ui->lineS3Secret->setText(userCredentials.AccessSecretKey.c_str());

    connect(ui->lineS3Bucket, SIGNAL(editingFinished()), this, SLOT(lineBucketEditingFinished()));
    connect(ui->lineS3Id, SIGNAL(editingFinished()), this, SLOT(lineIdEditingFinished()));
    connect(ui->lineS3Secret, SIGNAL(editingFinished()), this, SLOT(lineSecretEditingFinished()));

    // Finally initialize the main CaptureUtils instance
    captureUtil = std::unique_ptr<CaptureUtils>(new CaptureUtils(QDir::tempPath().toStdString()));
    captureUtil->AwsCredentials = userCredentials;
    captureUtil->SetupHotkeys();
}

void MainWindow::resizeWithAnimation( const QSize& size)
{
    animation->setDuration(150);
    animation->setStartValue(this->size());
    animation->setEndValue(size);
    animation->start();
}

void MainWindow::setToolBarGroup( void )
{
    ui->actionGeneral_Settings->setChecked(true);
    toolBarGroup->addAction(ui->actionGeneral_Settings);
    toolBarGroup->addAction(ui->actionS3);
    toolBarGroup->addAction(ui->actionAbout);

    ui->toolBar->addActions(toolBarGroup->actions());
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    ui->toolBar->insertWidget(ui->actionAbout, spacer);

    connect(ui->actionGeneral_Settings, SIGNAL(triggered(bool)), this, SLOT(changeStackedIndex(bool)));
    connect(ui->actionS3, SIGNAL(triggered(bool)), this, SLOT(changeStackedIndex(bool)));//SLOT(toolBarS3Clicked(bool)));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(changeStackedIndex(bool)));//SLOT(toolBarAboutClicked(bool)));
}

void MainWindow::setPageIndex(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
    for (int i = 0; i < ui->stackedWidget->count(); i++)
    {
        QWidget* widget = ui->stackedWidget->widget(i);
        if (i != index)
        {
            widget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        }
    }
    ui->stackedWidget->currentWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->stackedWidget->currentWidget()->adjustSize();
    resizeWithAnimation(sizeHint());
}

void MainWindow::changeStackedIndex( bool checked)
{
    if (!checked)
        return;
    // Get all of the items and see which one is checked
    for (QAction* action: toolBarGroup->actions())
    {
        if (action->isChecked())
        {
            if (action == ui->actionGeneral_Settings)
                setPageIndex(0);
            else if (action == ui->actionS3)
                setPageIndex(1);
            else if (ui->actionAbout)
                setPageIndex(2);
        }
    }
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
