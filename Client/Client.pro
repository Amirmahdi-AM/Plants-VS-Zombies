QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boomerang.cpp \
    jalapeno.cpp \
    main.cpp \
    mainwindow.cpp \
    peashooter.cpp \
    person.cpp \
    plants.cpp \
    plummine.cpp \
    twopeashooter.cpp \
    walnut.cpp

HEADERS += \
    boomerang.h \
    jalapeno.h \
    mainwindow.h \
    peashooter.h \
    person.h \
    plants.h \
    plummine.h \
    twopeashooter.h \
    walnut.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Pictures.qrc
