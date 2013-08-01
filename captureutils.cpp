#include "captureutils.h"
#include <QMessageBox>

CaptureUtils::CaptureUtils()
{

}

OSStatus HotkeyHandler(EventHandlerCallRef nextHandler,EventRef theEvent, void *userData)
{
    QMessageBox mb(QMessageBox::Icon::NoIcon, QString("TEST"), QString("hello, the hotkey was pressed and stuff"));
    mb.exec();
    return noErr;
}


/* reference: http://dbachrach.com/blog/2005/11/program-global-hotkeys-in-cocoa-easily/ */
void CaptureUtils::SetupHotkeys()
{
    eventType.eventClass = kEventClassKeyboard;
    eventType.eventKind = kEventHotKeyPressed;

    // Allows the application to get events and shit
    InstallApplicationEventHandler(&HotkeyHandler, 1, &eventType, NULL, NULL);

    gMyHotKeyID.signature = signature;
    gMyHotKeyID.id = 1;

    // Regsiters the hotkey ⌘⇪+CTRL
    RegisterEventHotKey(kVK_ANSI_8, cmdKey|shiftKey, gMyHotKeyID, GetApplicationEventTarget(), 0, &gMyHotKeyRef);
}

CaptureUtils::~CaptureUtils()
{

}
