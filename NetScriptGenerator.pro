QT += core widgets

CONFIG += c++17

SOURCES += net-script.cpp

TARGET = ARCNetScript

# Install path
target.path = /usr/local/bin
INSTALLS += target
