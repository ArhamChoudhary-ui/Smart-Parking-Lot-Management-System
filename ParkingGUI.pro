QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = ParkingLotGUI
TEMPLATE = app

SOURCES += \
    main_gui.cpp \
    mainwindow.cpp \
    dashboardwidget.cpp \
    vehicleentrywidget.cpp \
    vehicleexitwidget.cpp \
    lotstatuswidget.cpp \
    transactionwidget.cpp

HEADERS += \
    mainwindow.h \
    dashboardwidget.h \
    vehicleentrywidget.h \
    vehicleexitwidget.h \
    lotstatuswidget.h \
    transactionwidget.h \
    ParkingLot.h \
    ParkingSpot.h \
    DerivedSpots.h

RESOURCES += resources.qrc

# Platform-specific settings
unix|macx {
    QMAKE_CXXFLAGS += -Wall -Wextra
}

win32 {
    QMAKE_CXXFLAGS += /W4
}
