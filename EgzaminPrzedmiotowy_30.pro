QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    W_DANYCH/bazapytan.cpp \
    W_DANYCH\importer.cpp \
    W_DANYCH\importerBlokowy.cpp \
    main.cpp \
    W_PREZENTACJI\mainwindow.cpp \
    W_USLUG\program.cpp \
    W_DANYCH\pytanie.cpp \
    W_DANYCH\urn.cpp

HEADERS += \
    W_DANYCH/bazapytan.h \
    W_DANYCH\importer.h \
    W_DANYCH\importerBlokowy.h \
    W_PREZENTACJI\mainwindow.h \
    W_USLUG\program.h \
    W_DANYCH\pytanie.h \
    W_DANYCH\urn.h

FORMS += \
    W_PREZENTACJI\mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Pytania/PK/BlokA \
    Pytania/PK/BlokB \
    Pytania/PK/WiedzaA \
    Pytania/PK/WiedzaB \
    Pytania/PK/ZrozumienieA \
    Pytania/PK/ZrozumienieB \
    Pytania/PPK/BlokA \
    Pytania/PPK/WiedzaA \
    Pytania/PPK/ZrozumienieA \
    Pytania/TODO
