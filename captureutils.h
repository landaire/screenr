#ifndef CAPTUREUTILS_H
#define CAPTUREUTILS_H
#include <Carbon/Carbon.h> // required for key shortcut stuff
#include <string>
#include <QNetworkReply>
#include <QObject>

class CaptureUtils : public QObject
{
    Q_OBJECT
public:
    CaptureUtils(std::string temp);
    ~CaptureUtils();

    void SetupHotkeys();
    static OSStatus HotkeyHandler(EventHandlerCallRef nextHandler,EventRef theEvent, void *userData);

#ifdef DEBUG
    static std::string AwsKeyId;
    static std::string AwsSecretKey;
#endif

private:
    // Stuff required for hotkeys
    const uint32_t signature = ('s' << 24) | ('c' << 16) | ('r' << 8) | 'n';
    EventHotKeyRef gMyHotKeyRef;
    EventHotKeyID gMyHotKeyID;
    EventTypeSpec eventType;

    static void takeScreenshot(CaptureUtils *instance);
    static std::string tempFileName(const std::string root = "");
public slots:
    void finished();
    void onError(QNetworkReply::NetworkError code);
};

#endif // CAPTUREUTILS_H
