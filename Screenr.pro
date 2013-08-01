#-------------------------------------------------
#
# Project created by QtCreator 2013-07-28T15:32:37
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -std=c++11 -stdlib=libc++
LIBS += -mmacosx-version-min=10.7 -stdlib=libc++ /System/Library/Frameworks/Carbon.framework/Carbon
CONFIG += c++11 QT_MAC_USE_COCOA
DEFINES += QT_MAC_USE_COCOA

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Screenr
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    captureutils.cpp

HEADERS  += mainwindow.h \
    captureutils.h

FORMS    += mainwindow.ui
cache()

# Only Intel binaries are accepted so force this
#CONFIG -= x86_64
#CONFIG += x86

# Minimum OS X version for submission is 10.6.6
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6

# Copy the custom Info.plist to the app bundle
plist.path = "$$DESTDIR/$$join(TARGET,,,.app)/Contents"
plist.files = Info.plist
INSTALLS += plist

# Icon is mandatory for submission
#ICON = appicon.icns
