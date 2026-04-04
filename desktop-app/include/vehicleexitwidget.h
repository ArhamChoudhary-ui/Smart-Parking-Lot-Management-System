#ifndef VEHICLEEXITWIDGET_H
#define VEHICLEEXITWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include "ParkingLot.h"

class VehicleExitWidget : public QWidget {
    Q_OBJECT

public:
    VehicleExitWidget(ParkingLot *parkingLot, QWidget *parent = nullptr);
    void resetForm();

private slots:
    void calculateFee();
    void processExit();
    void showReceipt(double totalFee, double change);
    void showError(const QString &message);

private:
    void setupUI();
    void clearFields();
    bool validateInputs();

    ParkingLot *parkingLot;
    
    QLineEdit *spotIdInput;
    QSpinBox *actualHoursSpinBox;
    QSpinBox *extraMinutesSpinBox;
    QLineEdit *paidAmountInput;
    QPushButton *calculateButton;
    QPushButton *exitButton;
    QPushButton *clearButton;
    QLabel *feeDisplay;
    QLabel *statusLabel;
    QLabel *vehicleInfoLabel;
    QLabel *billingDetailsLabel;
};

#endif // VEHICLEEXITWIDGET_H
