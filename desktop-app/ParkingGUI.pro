QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = ParkingLotGUI
TEMPLATE = app

SOURCES += \
    src/main_gui.cpp \
    src/mainwindow.cpp \
    src/dashboardwidget.cpp \
    src/vehicleentrywidget.cpp \
    src/vehicleexitwidget.cpp \
    src/lotstatuswidget.cpp \
    src/transactionwidget.cpp

HEADERS += \
    include/mainwindow.h \
    include/dashboardwidget.h \
    include/vehicleentrywidget.h \
    include/vehicleexitwidget.h \
    include/lotstatuswidget.h \
    include/transactionwidget.h \
    include/ParkingLot.h \
    include/ParkingSpot.h \
    include/DerivedSpots.h

RESOURCES += resources/resources.qrc

INCLUDEPATH += include

# Platform-specific settings
unix|macx {
    QMAKE_CXXFLAGS += -Wall -Wextra
}

win32 {
    QMAKE_CXXFLAGS += /W4
}
