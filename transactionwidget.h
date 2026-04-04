#ifndef TRANSACTIONWIDGET_H
#define TRANSACTIONWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QComboBox>
#include "ParkingLot.h"

class TransactionWidget : public QWidget {
    Q_OBJECT

public:
    TransactionWidget(ParkingLot *parkingLot, QWidget *parent = nullptr);
    void refresh();

private slots:
    void filterTransactions();
    void exportTransactions();

private:
    void setupUI();
    void populateTransactions();

    ParkingLot *parkingLot;
    QTableWidget *transactionTable;
    QComboBox *filterCombo;
    QLabel *summaryLabel;
};

#endif // TRANSACTIONWIDGET_H
