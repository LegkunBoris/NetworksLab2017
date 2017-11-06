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
    chathandler.c

HEADERS += \
    client.h
