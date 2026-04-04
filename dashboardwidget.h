#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include "ParkingLot.h"

class DashboardWidget : public QWidget {
    Q_OBJECT

public:
    DashboardWidget(ParkingLot *parkingLot, QWidget *parent = nullptr);
    void refresh();

private:
    void setupUI();
    void updateStats();

    ParkingLot *parkingLot;
    QLabel *totalSpotsLabel;
    QLabel *occupiedSpotsLabel;
    QLabel *availableSpotsLabel;
    QLabel *occupancyPercentageLabel;
    QLabel *standardSpotsLabel;
    QLabel *disabledSpotsLabel;
    QLabel *vipSpotsLabel;
    QLabel *totalParkingsLabel;
    QLabel *totalExitsLabel;
    QLabel *totalRevenueLabel;
};

#endif // DASHBOARDWIDGET_H
