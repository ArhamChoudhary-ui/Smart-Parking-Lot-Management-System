#ifndef VEHICLEENTRYWIDGET_H
#define VEHICLEENTRYWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include "ParkingLot.h"

class VehicleEntryWidget : public QWidget {
    Q_OBJECT

public:
    VehicleEntryWidget(ParkingLot *parkingLot, QWidget *parent = nullptr);
    void resetForm();

private slots:
    void parkVehicle();
    void showSuccess();
    void showError(const QString &message);

private:
    void setupUI();
    void clearFields();
    bool validateInputs();

    ParkingLot *parkingLot;
    
    QComboBox *spotTypeCombo;
    QLineEdit *vehicleNumberInput;
    QComboBox *vehicleCategoryCombo;
    QSpinBox *hoursSpinBox;
    QPushButton *parkButton;
    QPushButton *clearButton;
    QLabel *statusLabel;
    QLabel *previewLabel;
};

#endif // VEHICLEENTRYWIDGET_H
