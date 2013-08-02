#include "captureutils.h"
#include <sstream>
#include <chrono> // time
#include <iomanip> // put_time
#include <stdlib.h>
#include <QDebug>

CaptureUtils::CaptureUtils(const std::string temp)
{
    tempFileName(temp);
}

OSStatus CaptureUtils::HotkeyHandler(EventHandlerCallRef nextHandler,EventRef theEvent, void *userData)
{
    takeScreenshot();
    return noErr;
}

void CaptureUtils::takeScreenshot()
{
    std::stringstream command;
    std::string tempPath = tempFileName();

    // screencapture command
    command << "screencapture -i \""
            << tempPath
            << "\"";
    system(command.str().c_str());

    // open command for debugging
    command.str("");
    command << "open \""
            << tempPath
            << "\"";
    system(command.str().c_str());
}

// Returns absolute path to a temp file. Must be static so it's accessable to the HotkeyHandler
std::string CaptureUtils::tempFileName(const std::string root)
{
    static std::string rootTempDirectory = root;
    if (root != "")
        return "";
    // Generate the hash
    std::stringstream ss;
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    ss << rootTempDirectory
       << "/Screenr Grab "
       << std::put_time(std::localtime(&time), "%c %Z")
       << ".png";
    return ss.str();
}

/* reference: http://dbachrach.com/blog/2005/11/program-global-hotkeys-in-cocoa-easily/ */
void CaptureUtils::SetupHotkeys()
{
    eventType.eventClass = kEventClassKeyboard;
    eventType.eventKind = kEventHotKeyPressed;

    // Allows the application to get events and shit
    InstallApplicationEventHandler(&(CaptureUtils::HotkeyHandler), 1, &eventType, NULL, NULL);

    gMyHotKeyID.signature = signature;
    gMyHotKeyID.id = 1;

    // Regsiters the hotkey ⌘⇪+CTRL
    RegisterEventHotKey(kVK_ANSI_8, cmdKey|shiftKey, gMyHotKeyID, GetApplicationEventTarget(), 0, &gMyHotKeyRef);
}

CaptureUtils::~CaptureUtils()
{

}
