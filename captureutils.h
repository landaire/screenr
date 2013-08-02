#ifndef CAPTUREUTILS_H
#define CAPTUREUTILS_H
#include <Carbon/Carbon.h> // required for key shortcut stuff
#include <string>

class CaptureUtils
{
public:
    CaptureUtils(std::string temp);
    ~CaptureUtils();

    void SetupHotkeys();
    static OSStatus HotkeyHandler(EventHandlerCallRef nextHandler,EventRef theEvent, void *userData);

private:
    // Stuff required for hotkeys
    const uint32_t signature = ('s' << 24) | ('c' << 16) | ('r' << 8) | 'n';
    EventHotKeyRef gMyHotKeyRef;
    EventHotKeyID gMyHotKeyID;
    EventTypeSpec eventType;

    static void takeScreenshot();
    static std::string tempFileName(const std::string root = "");
};

#endif // CAPTUREUTILS_H
