QT       += core websockets
QT       -= gui

TARGET = tv
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src\main.cpp \
    src\qbizmanager.cpp\
    src\qhttpmanager.cpp

HEADERS += \
    src\qbizmanager.h\
    src\qhttpmanager.h

target.path = $$[QT_INSTALL_EXAMPLES]/websockets/sslechoclient
INSTALLS += target
