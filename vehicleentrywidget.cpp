#include "vehicleentrywidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QFont>

VehicleEntryWidget::VehicleEntryWidget(ParkingLot *lot, QWidget *parent)
    : QWidget(parent), parkingLot(lot) {
    setupUI();
    resetForm();
}

void VehicleEntryWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *titleLabel = new QLabel("Park Vehicle", this);
    QFont titleFont;
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2c3e50;");
    mainLayout->addWidget(titleLabel);

    // Form Group
    QGroupBox *formGroup = new QGroupBox("Vehicle Details", this);
    QGridLayout *formLayout = new QGridLayout(formGroup);
    formLayout->setSpacing(15);

    // Spot Type Selection
    QLabel *spotTypeLabel = new QLabel("Select Spot Type:", this);
    spotTypeLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    spotTypeCombo = new QComboBox(this);
    spotTypeCombo->addItem("🚗 Standard Spot (₹20/hour)", 1);
    spotTypeCombo->addItem("♿ Disabled Spot (₹10/hour)", 2);
    spotTypeCombo->addItem("⭐ VIP Spot (₹50/hour)", 3);
    spotTypeCombo->setMinimumHeight(35);
    spotTypeCombo->setStyleSheet(
        "QComboBox {"
        "    border: 2px solid #bdc3c7;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "    font-size: 12px;"
        "    background-color: white;"
        "}"
        "QComboBox:hover {"
        "    border: 2px solid #3498db;"
        "}"
    );
    formLayout->addWidget(spotTypeLabel, 0, 0);
    formLayout->addWidget(spotTypeCombo, 0, 1, 1, 2);

    // Vehicle Number
    QLabel *vehicleNumberLabel = new QLabel("Vehicle Registration Number:", this);
    vehicleNumberLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    vehicleNumberInput = new QLineEdit(this);
    vehicleNumberInput->setPlaceholderText("e.g., DL-01-AB-1234");
    vehicleNumberInput->setMaximumWidth(300);
    vehicleNumberInput->setMinimumHeight(35);
    vehicleNumberInput->setStyleSheet(
        "QLineEdit {"
        "    border: 2px solid #bdc3c7;"
        "    border-radius: 5px;"
        "    padding: 8px;"
        "    font-size: 12px;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #3498db;"
        "}"
    );
    formLayout->addWidget(vehicleNumberLabel, 1, 0);
    formLayout->addWidget(vehicleNumberInput, 1, 1, 1, 2);

    // Vehicle Category
    QLabel *categoryLabel = new QLabel("Vehicle Category:", this);
    categoryLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    vehicleCategoryCombo = new QComboBox(this);
    vehicleCategoryCombo->addItem("2W - Two Wheeler (Bike/Scooter) - ₹20", "2W");
    vehicleCategoryCombo->addItem("4W - Four Wheeler (Car/SUV) - ₹50", "4W");
    vehicleCategoryCombo->setMinimumHeight(35);
    vehicleCategoryCombo->setStyleSheet(
        "QComboBox {"
        "    border: 2px solid #bdc3c7;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "    font-size: 12px;"
        "    background-color: white;"
        "}"
        "QComboBox:hover {"
        "    border: 2px solid #3498db;"
        "}"
    );
    formLayout->addWidget(categoryLabel, 2, 0);
    formLayout->addWidget(vehicleCategoryCombo, 2, 1, 1, 2);

    // Allotted Hours
    QLabel *hoursLabel = new QLabel("Allotted Hours:", this);
    hoursLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    hoursSpinBox = new QSpinBox(this);
    hoursSpinBox->setMinimum(1);
    hoursSpinBox->setMaximum(24);
    hoursSpinBox->setValue(2);
    hoursSpinBox->setMinimumHeight(35);
    hoursSpinBox->setMaximumWidth(150);
    hoursSpinBox->setStyleSheet(
        "QSpinBox {"
        "    border: 2px solid #bdc3c7;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "    font-size: 12px;"
        "}"
        "QSpinBox:focus {"
        "    border: 2px solid #3498db;"
        "}"
    );
    formLayout->addWidget(hoursLabel, 3, 0);
    formLayout->addWidget(hoursSpinBox, 3, 1);

    formGroup->setStyleSheet(
        "QGroupBox {"
        "    border: 2px solid #3498db;"
        "    border-radius: 8px;"
        "    margin-top: 10px;"
        "    padding-top: 10px;"
        "    font-weight: bold;"
        "    color: #2c3e50;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 3px 0 3px;"
        "}"
    );

    mainLayout->addWidget(formGroup);

    // Preview Section
    QGroupBox *previewGroup = new QGroupBox("Estimated Fee Preview", this);
    QVBoxLayout *previewLayout = new QVBoxLayout(previewGroup);
    previewLabel = new QLabel("₹0.00", this);
    previewLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #27ae60;");
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLayout->addWidget(previewLabel);
    previewGroup->setStyleSheet(
        "QGroupBox {"
        "    border: 2px solid #27ae60;"
        "    border-radius: 8px;"
        "    margin-top: 10px;"
        "    padding-top: 10px;"
        "    font-weight: bold;"
        "    color: #2c3e50;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 3px 0 3px;"
        "}"
    );
    mainLayout->addWidget(previewGroup);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    parkButton = new QPushButton("✓ Park Vehicle", this);
    parkButton->setMinimumHeight(45);
    parkButton->setMinimumWidth(200);
    parkButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #27ae60;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #229954;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1e8449;"
        "}"
    );
    connect(parkButton, &QPushButton::clicked, this, &VehicleEntryWidget::parkVehicle);

    clearButton = new QPushButton("↻ Clear", this);
    clearButton->setMinimumHeight(45);
    clearButton->setMinimumWidth(150);
    clearButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #95a5a6;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #7f8c8d;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #5d6d7b;"
        "}"
    );
    connect(clearButton, &QPushButton::clicked, this, &VehicleEntryWidget::clearFields);

    buttonLayout->addWidget(parkButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    // Status Label
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("font-weight: bold; color: #2c3e50; padding: 10px;");
    statusLabel->setWordWrap(true);
    mainLayout->addWidget(statusLabel);

    mainLayout->addStretch();
}

void VehicleEntryWidget::resetForm() {
    clearFields();
    statusLabel->clear();
}

void VehicleEntryWidget::clearFields() {
    vehicleNumberInput->clear();
    spotTypeCombo->setCurrentIndex(0);
    vehicleCategoryCombo->setCurrentIndex(0);
    hoursSpinBox->setValue(2);
    previewLabel->setText("₹0.00");
}

bool VehicleEntryWidget::validateInputs() {
    if (vehicleNumberInput->text().isEmpty()) {
        showError("Please enter vehicle registration number");
        return false;
    }
    if (vehicleNumberInput->text().length() < 8) {
        showError("Invalid vehicle registration number format");
        return false;
    }
    if (hoursSpinBox->value() < 1) {
        showError("Allotted hours must be at least 1");
        return false;
    }
    return true;
}

void VehicleEntryWidget::parkVehicle() {
    if (!validateInputs()) {
        return;
    }

    try {
        int spotType = spotTypeCombo->currentData().toInt();
        QString vehicleNumber = vehicleNumberInput->text();
        QString vehicleCategory = vehicleCategoryCombo->currentData().toString();
        int hours = hoursSpinBox->value();

        // Call parkingLot->parkVehicle() method
        // For now, showing success message
        showSuccess(QString::number(spotType));

    } catch (const std::exception &e) {
        showError(QString::fromStdString(e.what()));
    }
}

void VehicleEntryWidget::showSuccess(const QString &spotId) {
    QMessageBox::information(this, "Success",
        "Vehicle parked successfully!\nParking Spot ID: " + spotId);
    clearFields();
}

void VehicleEntryWidget::showError(const QString &message) {
    QMessageBox::critical(this, "Error", message);
    statusLabel->setText("❌ " + message);
    statusLabel->setStyleSheet("font-weight: bold; color: #e74c3c; padding: 10px;");
}
