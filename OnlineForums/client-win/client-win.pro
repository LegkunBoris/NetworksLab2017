TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lws2_32
SOURCES += main.c \
    readn.c \
    init.c \
    closesock.c \
    client.c \
    sconcat.c \
    decode.c \
    chathandler.c \
    sendbytes.c \
    sendint.c \
    readbytes.c \
    readlength.c

HEADERS += \
    client.h
