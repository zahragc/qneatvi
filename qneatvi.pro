QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qneatvi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    neatvi/cmd.c \
    neatvi/conf.c \
    neatvi/dir.c \
    neatvi/ex.c \
    neatvi/lbuf.c \
    neatvi/led.c \
    neatvi/mot.c \
    neatvi/reg.c \
    neatvi/regex.c \
    neatvi/ren.c \
    neatvi/rset.c \
    neatvi/sbuf.c \
    neatvi/syn.c \
    neatvi/term.c \
    neatvi/uc.c \
    neatvi/vi.c \
    textedit.cpp \
    api.cpp \
    toolbar.cpp \
    textedit2.cpp \
    finddialog.cpp \
    helpwidget.cpp \
    settingdialog.cpp \
    textedit3.cpp

HEADERS  += mainwindow.h \
    neatvi/conf.h \
    neatvi/kmap.h \
    neatvi/regex.h \
    neatvi/vi.h \
    textedit.h \
    api.h \
    xterm256torgb.h \
    toolbar.h \
    finddialog.h \
    helpwidget.h \
    settingdialog.h

DISTFILES +=

RESOURCES += \
    resources.qrc
