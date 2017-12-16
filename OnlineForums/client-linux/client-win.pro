TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lpthread
SOURCES += main.c \
    init.c \
    client.c \
    sconcat.c \
    decode.c \
    chathandler.c \
    sendbytes.c \
    sendint.c \
    readbytes.c \
    readlint.c \
    readn.c

HEADERS += \
    client.h
