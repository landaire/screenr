#ifndef CAPTUREUTILS_H
#define CAPTUREUTILS_H
#include <Carbon/Carbon.h> // required for key shortcut stuff
#include <string>
#include <QNetworkReply>
#include <QObject>
#include "aws.h"

class CaptureUtils : public QObject
{
    Q_OBJECT
public:
    CaptureUtils(std::string temp);
    ~CaptureUtils();

    void SetupHotkeys( void );
    static OSStatus HotkeyHandler(EventHandlerCallRef nextHandler,EventRef theEvent, void *userData);
    static void SetCredentials(AWS::AWSCredentials credentials);

    static AWS::AWSCredentials AwsCredentials;

private:
    // Stuff required for hotkeys
    const uint32_t signature = ('s' << 24) | ('c' << 16) | ('r' << 8) | 'n';
    EventHotKeyRef gMyHotKeyRef;
    EventHotKeyID gMyHotKeyID;
    EventTypeSpec eventType;

    static void takeScreenshot(CaptureUtils *instance);
    static std::string tempFileName(const std::string root = "");
public slots:
    void finished(void);
    void onError(QNetworkReply::NetworkError code);
};

#endif // CAPTUREUTILS_H
