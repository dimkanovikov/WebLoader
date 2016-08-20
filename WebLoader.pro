TEMPLATE = app
TARGET = webloader
DEPENDPATH += . src src/QFreeDesktopMime
INCLUDEPATH += . src src/QFreeDesktopMime

QT += network xml

HEADERS += src/HttpMultiPart_p.h \
           src/QFreeDesktopMime/freedesktopmime.h \
           src/NetworkQueue_p.h \
           src/NetworkRequest.h \
           src/NetworkRequestInternal_p.h \
           src/WebLoader_p.h \
           src/WebRequest_p.h

SOURCES += src/HttpMultiPart_p.cpp \
           src/WebLoader_p.cpp \
           src/WebRequest_p.cpp \
           src/QFreeDesktopMime/freedesktopmime.cpp \
           src/NetworkQueue_p.cpp \
           src/NetworkRequest.cpp \
           src/NetworkRequestInternal_p.cpp \
src/main.cpp
