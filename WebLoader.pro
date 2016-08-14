TEMPLATE = lib
TARGET = webloader
DEPENDPATH += . src src/QFreeDesktopMime
INCLUDEPATH += . src src/QFreeDesktopMime

QT += network xml

HEADERS += src/HttpMultiPart.h \
           src/WebLoader.h \
           src/WebRequest.h \
           src/QFreeDesktopMime/freedesktopmime.h \
           src/tester.h \
           src/NetworkQueue.h \
           src/NetworkRequest.h \
           src/NetworkRequestInternal.h

SOURCES += src/HttpMultiPart.cpp \
           src/WebLoader.cpp \
           src/WebRequest.cpp \
           src/QFreeDesktopMime/freedesktopmime.cpp \
           src/main.cpp \
           src/tester.cpp \
           src/NetworkQueue.cpp \
           src/NetworkRequest.cpp \
           src/NetworkRequestInternal.cpp
