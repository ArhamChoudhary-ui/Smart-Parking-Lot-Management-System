#ifndef LOTSTATUSWIDGET_H
#define LOTSTATUSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include "ParkingLot.h"

class LotStatusWidget : public QWidget {
    Q_OBJECT

public:
    LotStatusWidget(ParkingLot *parkingLot, QWidget *parent = nullptr);
    void refresh();

private:
    void setupUI();
    void populateLotStatus();

    ParkingLot *parkingLot;
    QTableWidget *spotTable;
    QLabel *overallStatusLabel;
};

#endif // LOTSTATUSWIDGET_H
