QT += core gui widgets charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    CityGrid.cpp \
    Plant.cpp \
    PollenParticle.cpp \
    Building.cpp \
    Human.cpp \
    WindField.cpp \
    Weather.cpp

HEADERS += \
    mainwindow.h \
    CityGrid.h \
    Plant.h \
    PollenParticle.h \
    Building.h \
    Human.h \
    WindField.h \
    Weather.h \
    Config.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
