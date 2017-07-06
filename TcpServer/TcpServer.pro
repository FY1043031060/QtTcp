#-------------------------------------------------
#
# Project created by QtCreator 2017-05-10T08:52:35
#
#-------------------------------------------------


# network �������ģ��
QT  += core  network
QT  -= gui

CONFIG += console
CONFIG -= app_bundle

TARGET = TcpServer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    TcpServerThread.cpp \
    BusTCPServer.cpp \
    main.cpp \
    TcpServerThread.cpp \
    PollThread.cpp

HEADERS  += \
    TcpServerThread.h \
    BusTCPServer.h \
    TcpServerThread.h \
    PollThread.h

FORMS    += serverwidget.ui \
    serverwidget.ui


CONFIG += C++11

SUBDIRS += \
    TcpServer.pro
