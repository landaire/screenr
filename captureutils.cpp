#include "captureutils.h"
#include <sstream>
#include <chrono> // time
#include <iomanip> // put_time
#include <stdlib.h>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <QClipboard>
#include <QDir>
#include "aws.h"
#include <QNetworkReply>
#include <QClipboard>
#include <QApplication>
#include <QFile>

CaptureUtils::CaptureUtils(const std::string temp) : QObject()
{
    tempFileName(temp);
    takeScreenshot(this);

    // Load the AWS keys
#ifdef MANUAL_LOAD_KEYS
    std::ifstream keyFile("/Users/lander/Documents/Programming/Screenr/awskeys.secret");
    if (keyFile.is_open())
    {
        std::getLine(keyFile, AwsCredentials.Bucket)
        std::getline(keyFile, AwsCredentials.AwsKeyId);
        std::getline(keyFile, AwsCredentials.AwsSecretKey);
        keyFile.close();
    }
#endif
}

OSStatus CaptureUtils::HotkeyHandler(EventHandlerCallRef nextHandler,EventRef theEvent, void *userData)
{
    takeScreenshot(nullptr);
    return noErr;
}

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

void CaptureUtils::takeScreenshot(CaptureUtils* instance)
{
    static CaptureUtils* classInstance = instance;
    if (instance != nullptr)
        return;

    std::stringstream command;
    std::string tempPath = tempFileName();

    // screencapture command
    command << "screencapture -i \""
            << tempPath
            << "\"";
    int result = system(command.str().c_str());
    qDebug("%d", result);

    // open command for debugging
//    command.str("");
//    command << "open \""
//            << tempPath
//            << "\"";
//    system(command.str().c_str());

    std::ifstream screenshotFile(tempPath.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
    if (screenshotFile.is_open())
    {
        qDebug("Opened screenshot file");
        long size = screenshotFile.tellg();
        screenshotFile.seekg(0, std::ios::beg);
        std::unique_ptr<char> fileBuf(new char[size]);
        screenshotFile.read(fileBuf.get(), size);
        screenshotFile.close();

        // Now that the file is closed, it can be deleted
        QFile file(tempPath.c_str());
        file.remove();

        const QByteArray data(fileBuf.get(), size);

        AWS aws(AwsCredentials);
        auto request = aws.MakeRequest(random_string(5) + ".png", data);
        static QNetworkAccessManager* manager = new QNetworkAccessManager(classInstance);

        QNetworkReply* response = manager->put(*request, data);
        connect(response, SIGNAL(finished()), classInstance, SLOT(finished()));
        connect(response, SIGNAL(error(QNetworkReply::NetworkError)), classInstance, SLOT(onError(QNetworkReply::NetworkError)));

        //s3_put(bf.get(), const_cast<char*>(QDir(tempPath.c_str()).dirName().toStdString().c_str()));
    }
    else
    {
        qDebug("Couldn't open the screenshot file");
    }

}

void CaptureUtils::onError(QNetworkReply::NetworkError code)
{
    qDebug("error: %d", code);
}

void CaptureUtils::finished( void )
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    qDebug("%s", reply->errorString().toStdString().c_str());
    while (!reply->isFinished())
    {
        // do nothing
    }
    qDebug("%d", reply->error());
    qDebug("%d", QString(reply->readAll()).toStdString().c_str());
    QApplication::clipboard()->setText(reply->url().toString());
    delete reply;
}

// Returns absolute path to a temp file. Must be static so it's accessable to the HotkeyHandler
std::string CaptureUtils::tempFileName(const std::string root)
{
    static std::string rootTempDirectory = root;
    if (root != "")
        return "";
    // Generate the path
    std::stringstream ss;
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    ss << rootTempDirectory
       << "/Screenr Grab "
       << std::put_time(std::localtime(&time), "%c %Z")
       << ".png";
    qDebug("%s", ss.str().c_str());
    return ss.str();
}

/* reference: http://dbachrach.com/blog/2005/11/program-global-hotkeys-in-cocoa-easily/ */
void CaptureUtils::SetupHotkeys( void )
{
    eventType.eventClass = kEventClassKeyboard;
    eventType.eventKind = kEventHotKeyPressed;

    // Allows the application to get events and shit
    InstallApplicationEventHandler(&(CaptureUtils::HotkeyHandler), 1, &eventType, NULL, NULL);

    gMyHotKeyID.signature = signature;
    gMyHotKeyID.id = 1;

    // Regsiters the hotkey ⌘⇪+5
    RegisterEventHotKey(kVK_ANSI_5, cmdKey|shiftKey, gMyHotKeyID, GetApplicationEventTarget(), 0, &gMyHotKeyRef);
}

CaptureUtils::~CaptureUtils()
{

}
