#include "vehicleexitwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QFont>

VehicleExitWidget::VehicleExitWidget(ParkingLot *lot, QWidget *parent)
    : QWidget(parent), parkingLot(lot) {
    setupUI();
    resetForm();
}

void VehicleExitWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *titleLabel = new QLabel("Vehicle Exit & Billing", this);
    QFont titleFont;
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2c3e50;");
    mainLayout->addWidget(titleLabel);

    // Vehicle Information
    QGroupBox *vehicleInfoGroup = new QGroupBox("Parking Information", this);
    QVBoxLayout *vehicleInfoLayout = new QVBoxLayout(vehicleInfoGroup);
    vehicleInfoLabel = new QLabel("Enter spot ID to retrieve vehicle information", this);
    vehicleInfoLabel->setStyleSheet("color: #7f8c8d; font-style: italic;");
    vehicleInfoLayout->addWidget(vehicleInfoLabel);
    vehicleInfoGroup->setStyleSheet(
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
    mainLayout->addWidget(vehicleInfoGroup);

    // Exit Details Form
    QGroupBox *exitFormGroup = new QGroupBox("Exit Details", this);
    QGridLayout *formLayout = new QGridLayout(exitFormGroup);
    formLayout->setSpacing(15);

    // Spot ID
    QLabel *spotIdLabel = new QLabel("Parking Spot ID:", this);
    spotIdLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    spotIdInput = new QLineEdit(this);
    spotIdInput->setPlaceholderText("Enter spot ID");
    spotIdInput->setMaximumWidth(300);
    spotIdInput->setMinimumHeight(35);
    spotIdInput->setStyleSheet(
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
    formLayout->addWidget(spotIdLabel, 0, 0);
    formLayout->addWidget(spotIdInput, 0, 1, 1, 2);

    // Actual Hours
    QLabel *actualHoursLabel = new QLabel("Actual Hours Used:", this);
    actualHoursLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    actualHoursSpinBox = new QSpinBox(this);
    actualHoursSpinBox->setMinimum(0);
    actualHoursSpinBox->setMaximum(24);
    actualHoursSpinBox->setValue(2);
    actualHoursSpinBox->setMinimumHeight(35);
    actualHoursSpinBox->setMaximumWidth(150);
    actualHoursSpinBox->setStyleSheet(
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
    formLayout->addWidget(actualHoursLabel, 1, 0);
    formLayout->addWidget(actualHoursSpinBox, 1, 1);

    // Extra Minutes
    QLabel *extraMinutesLabel = new QLabel("Extra Minutes (0-59):", this);
    extraMinutesLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    extraMinutesSpinBox = new QSpinBox(this);
    extraMinutesSpinBox->setMinimum(0);
    extraMinutesSpinBox->setMaximum(59);
    extraMinutesSpinBox->setValue(0);
    extraMinutesSpinBox->setMinimumHeight(35);
    extraMinutesSpinBox->setMaximumWidth(150);
    extraMinutesSpinBox->setStyleSheet(
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
    formLayout->addWidget(extraMinutesLabel, 1, 2);
    formLayout->addWidget(extraMinutesSpinBox, 1, 3);

    exitFormGroup->setStyleSheet(
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
    mainLayout->addWidget(exitFormGroup);

    // Billing Details
    QGroupBox *billingGroup = new QGroupBox("Billing Details", this);
    QVBoxLayout *billingLayout = new QVBoxLayout(billingGroup);
    
    billingDetailsLabel = new QLabel("Click 'Calculate Fee' to view billing details", this);
    billingDetailsLabel->setStyleSheet("color: #7f8c8d; font-style: italic; white-space: pre-wrap;");
    billingLayout->addWidget(billingDetailsLabel);

    feeDisplay = new QLabel("₹0.00", this);
    feeDisplay->setStyleSheet("font-size: 32px; font-weight: bold; color: #e74c3c; text-align: center;");
    feeDisplay->setAlignment(Qt::AlignCenter);
    billingLayout->addWidget(feeDisplay);

    billingGroup->setStyleSheet(
        "QGroupBox {"
        "    border: 2px solid #e74c3c;"
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
    mainLayout->addWidget(billingGroup);

    // Payment Section
    QGroupBox *paymentGroup = new QGroupBox("Payment", this);
    QGridLayout *paymentLayout = new QGridLayout(paymentGroup);
    paymentLayout->setSpacing(15);

    QLabel *paidAmountLabel = new QLabel("Amount Paid:", this);
    paidAmountLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    paidAmountInput = new QLineEdit(this);
    paidAmountInput->setPlaceholderText("0.00");
    paidAmountInput->setMaximumWidth(300);
    paidAmountInput->setMinimumHeight(35);
    paidAmountInput->setStyleSheet(
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
    paymentLayout->addWidget(paidAmountLabel, 0, 0);
    paymentLayout->addWidget(paidAmountInput, 0, 1, 1, 2);

    paymentGroup->setStyleSheet(
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
    mainLayout->addWidget(paymentGroup);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    calculateButton = new QPushButton("🧮 Calculate Fee", this);
    calculateButton->setMinimumHeight(45);
    calculateButton->setMinimumWidth(180);
    calculateButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #3498db;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1f618d;"
        "}"
    );
    connect(calculateButton, &QPushButton::clicked, this, &VehicleExitWidget::calculateFee);

    exitButton = new QPushButton("✓ Process Exit & Pay", this);
    exitButton->setMinimumHeight(45);
    exitButton->setMinimumWidth(200);
    exitButton->setStyleSheet(
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
    connect(exitButton, &QPushButton::clicked, this, &VehicleExitWidget::processExit);

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
    connect(clearButton, &QPushButton::clicked, this, &VehicleExitWidget::clearFields);

    buttonLayout->addWidget(calculateButton);
    buttonLayout->addWidget(exitButton);
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

void VehicleExitWidget::resetForm() {
    clearFields();
    statusLabel->clear();
    vehicleInfoLabel->setText("Enter spot ID to retrieve vehicle information");
    billingDetailsLabel->setText("Click 'Calculate Fee' to view billing details");
}

void VehicleExitWidget::clearFields() {
    spotIdInput->clear();
    actualHoursSpinBox->setValue(2);
    extraMinutesSpinBox->setValue(0);
    paidAmountInput->clear();
    feeDisplay->setText("₹0.00");
}

bool VehicleExitWidget::validateInputs() {
    if (spotIdInput->text().isEmpty()) {
        showError("Please enter parking spot ID");
        return false;
    }
    if (paidAmountInput->text().isEmpty()) {
        showError("Please enter amount paid");
        return false;
    }
    return true;
}

void VehicleExitWidget::calculateFee() {
    if (spotIdInput->text().isEmpty()) {
        showError("Please enter parking spot ID");
        return;
    }

    try {
        int spotId = spotIdInput->text().toInt();
        int actualHours = actualHoursSpinBox->value();
        int extraMinutes = extraMinutesSpinBox->value();
        if (spotId <= 0) {
            showError("Please enter a valid parking spot ID");
            return;
        }

        ParkingLot::BillingPreview bill = parkingLot->getExitBillingPreview(spotId, actualHours, extraMinutes);
        feeDisplay->setText("₹" + QString::number(bill.totalFee, 'f', 2));

        vehicleInfoLabel->setText(
            QString("Spot: %1 (%2) | Vehicle: %3 | Category: %4 | Allotted: %5h")
                .arg(bill.spotId)
                .arg(QString::fromStdString(bill.spotType))
                .arg(QString::fromStdString(bill.vehicleNumber))
                .arg(QString::fromStdString(bill.vehicleCategory))
                .arg(bill.allottedHours)
        );
        vehicleInfoLabel->setStyleSheet("color: #2c3e50; font-weight: bold;");

        QString details = QString(
            "Spot Type: %1 (₹%2/hr)\n"
            "Used Duration: %3 hours %4 minutes\n"
            "Parking Charge: ₹%5\n"
            "Vehicle Base Charge: ₹%6\n"
            "Overtime Fine: ₹%7\n"
            "─────────────────\n"
            "Total Fee: ₹%8"
        )
            .arg(QString::fromStdString(bill.spotType))
            .arg(bill.spotHourlyRate, 0, 'f', 2)
            .arg(bill.actualHours)
            .arg(bill.actualExtraMinutes)
            .arg(bill.spotFee, 0, 'f', 2)
            .arg(bill.vehicleInitialCharge, 0, 'f', 2)
            .arg(bill.overtimeFine, 0, 'f', 2)
            .arg(bill.totalFee, 0, 'f', 2);
        billingDetailsLabel->setText(details);
        billingDetailsLabel->setStyleSheet("color: #2c3e50; font-family: monospace;");

    } catch (const std::exception &e) {
        showError(QString::fromStdString(e.what()));
    }
}

void VehicleExitWidget::processExit() {
    if (!validateInputs()) {
        return;
    }

    try {
        int spotId = spotIdInput->text().toInt();
        int actualHours = actualHoursSpinBox->value();
        int extraMinutes = extraMinutesSpinBox->value();
        double paidAmount = paidAmountInput->text().toDouble();
        if (spotId <= 0) {
            showError("Please enter a valid parking spot ID");
            return;
        }
        if (paidAmount <= 0) {
            showError("Please enter a valid paid amount");
            return;
        }

        ParkingLot::Receipt receipt = parkingLot->exitVehicleWithPayment(
            spotId,
            actualHours,
            extraMinutes,
            paidAmount
        );
        showReceipt(receipt);
        statusLabel->setText("✅ Payment successful. Vehicle exited from spot " + QString::number(receipt.spotId));
        statusLabel->setStyleSheet("font-weight: bold; color: #27ae60; padding: 10px;");

    } catch (const std::exception &e) {
        showError(QString::fromStdString(e.what()));
    }
}

void VehicleExitWidget::showReceipt(const ParkingLot::Receipt &receipt) {
    QString receiptText = QString(
        "═══════════════════════════════════\n"
        "          PARKING RECEIPT\n"
        "═══════════════════════════════════\n"
        "Spot: %1 (%2)\n"
        "Vehicle: %3 (%4)\n"
        "Used: %5h %6m (Allotted: %7h)\n"
        "Parking Charge:     ₹%8\n"
        "Vehicle Charge:     ₹%9\n"
        "Overtime Fine:      ₹%10\n"
        "Total Fee:          ₹%11\n"
        "Amount Paid:        ₹%12\n"
        "Change:             ₹%13\n"
        "═══════════════════════════════════\n"
        "Thank you! Drive safely!"
    )
        .arg(receipt.spotId)
        .arg(QString::fromStdString(receipt.spotType))
        .arg(QString::fromStdString(receipt.vehicleNumber))
        .arg(QString::fromStdString(receipt.vehicleCategory))
        .arg(receipt.actualHours)
        .arg(receipt.actualExtraMinutes)
        .arg(receipt.allottedHours)
        .arg(receipt.spotFee, 0, 'f', 2)
        .arg(receipt.vehicleInitialCharge, 0, 'f', 2)
        .arg(receipt.overtimeFine, 0, 'f', 2)
        .arg(receipt.totalFee, 0, 'f', 2)
        .arg(receipt.paidAmount, 0, 'f', 2)
        .arg(receipt.change, 0, 'f', 2);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Receipt");
    msgBox.setText(receiptText);
    msgBox.setFont(QFont("Courier", 10));
    msgBox.setStyleSheet("QMessageBox { background-color: #f0f2f5; }");
    msgBox.exec();

    clearFields();
}

void VehicleExitWidget::showError(const QString &message) {
    QMessageBox::critical(this, "Error", message);
    statusLabel->setText("❌ " + message);
    statusLabel->setStyleSheet("font-weight: bold; color: #e74c3c; padding: 10px;");
}
