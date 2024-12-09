#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui sql quick
QT       += printsupport
QT       += charts
QT += charts
QT       += core  network
QT       += network
QT       += serialport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Atelier_Connexion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11
CONFIG += debug


SOURCES += \
    arduino.cpp \
    client.cpp \
    commande.cpp \
    employe.cpp \
    main.cpp \
    connection.cpp \
    mainwin.cpp \
    mainwindow.cpp \
    randomstringgenerator.cpp \
    smtp.cpp \
    smtp2.cpp

HEADERS += \
    arduino.h \
    client.h \
    commande.h \
    connection.h \
    employe.h \
    mainwin.h \
    mainwindow.h \
    randomstringgenerator.h \
    smtp.h \
    smtp2.h

FORMS += \
    mainwin.ui \
    mainwindow.ui

# Set OpenSSL Include Path and Library Path

LIBS += -lQt5Charts
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
