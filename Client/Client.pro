QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    astronautzombie.cpp \
    boomerang.cpp \
    bucketheadzombie.cpp \
    jalapeno.cpp \
    leafheadzombie.cpp \
    main.cpp \
    mainwindow.cpp \
    peashooter.cpp \
    person.cpp \
    plants.cpp \
    plummine.cpp \
    purplehairzombie.cpp \
    regularzombie.cpp \
    tallzombie.cpp \
    twopeashooter.cpp \
    walnut.cpp \
    zombies.cpp

HEADERS += \
    astronautzombie.h \
    boomerang.h \
    bucketheadzombie.h \
    jalapeno.h \
    leafheadzombie.h \
    mainwindow.h \
    peashooter.h \
    person.h \
    plants.h \
    plummine.h \
    purplehairzombie.h \
    regularzombie.h \
    tallzombie.h \
    twopeashooter.h \
    walnut.h \
    zombies.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Pictures.qrc
