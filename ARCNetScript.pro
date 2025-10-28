QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = net-script
TEMPLATE = app

CONFIG += c++17

# Output directories
DESTDIR = release
OBJECTS_DIR = release/.obj
MOC_DIR = release/.moc
RCC_DIR = release/.rcc
UI_DIR = release/.ui

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    net-script.cpp

HEADERS +=

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Resource file (if you have icons/images)
# RESOURCES += resources.qrc

# Windows specific
win32 {
    RC_ICONS = icon.ico
    VERSION = 2.0.0.0
    QMAKE_TARGET_COMPANY = "Amateur Radio Community"
    QMAKE_TARGET_PRODUCT = "ARC Net Script Generator"
    QMAKE_TARGET_DESCRIPTION = "Amateur Radio Net Script Generator"
    QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2025"
}

# macOS specific
macx {
    ICON = icon.icns
    QMAKE_INFO_PLIST = Info.plist
}
