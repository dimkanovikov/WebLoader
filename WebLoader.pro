TEMPLATE = lib
TARGET = webloader
DEPENDPATH += . src src/QFreeDesktopMime
INCLUDEPATH += . src src/QFreeDesktopMime

QT += network

HEADERS += src/HttpMultiPart.h \
           src/WebLoader.h \
           src/WebRequest.h \
           src/QFreeDesktopMime/freedesktopmime.h

SOURCES += src/HttpMultiPart.cpp \
           src/WebLoader.cpp \
           src/WebRequest.cpp \
           src/QFreeDesktopMime/freedesktopmime.cpp
