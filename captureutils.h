#ifndef CAPTUREUTILS_H
#define CAPTUREUTILS_H
#include <Carbon/Carbon.h> // required for key shortcut stuff

class CaptureUtils
{
public:
    CaptureUtils();
    ~CaptureUtils();

    void SetupHotkeys();
    //OSStatus HotkeyHandler(EventHandlerCallRef nextHandler,EventRef theEvent, void *userData);

private:
    const uint32_t signature = ('s' << 24) | ('c' << 16) | ('r' << 8) | 'n';

    EventHotKeyRef gMyHotKeyRef;
    EventHotKeyID gMyHotKeyID;
    EventTypeSpec eventType;
};

#endif // CAPTUREUTILS_H
