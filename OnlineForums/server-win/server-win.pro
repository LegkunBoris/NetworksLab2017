TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lws2_32
SOURCES += main.c \
    decodestring.c \
    sendbytes.c \
    sendint.c \
    loadforums.c \
    readn.c \
    connectinohandler.c \
    writemessagetofile.c \
    readfromfile.c \
    cancatstrings.c \
    forumshandler.c

DISTFILES += \
    forums.txt \
    help.txt

HEADERS += \
    server.h
