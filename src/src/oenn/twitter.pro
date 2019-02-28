QT       += core websockets script networkauth
QT       -= gui

TARGET = twitter
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES +=	src\main.cpp 
SOURCES +=	src\qbizmanager.cpp
SOURCES +=	src\qhttpmanager.cpp
SOURCES +=	src\twitter.cpp
SOURCES +=	src\qimap.cpp



HEADERS +=	src\qbizmanager.h
HEADERS +=	src\qhttpmanager.h
HEADERS +=	src\twitter.h
HEADERS +=	src\qimap.h
