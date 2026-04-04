#include "dashboardwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QProgressBar>
#include <QPixmap>
#include <QPalette>

DashboardWidget::DashboardWidget(ParkingLot *lot, QWidget *parent)
    : QWidget(parent), parkingLot(lot) {
    setupUI();
    refresh();
}

void DashboardWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *titleLabel = new QLabel("Dashboard", this);
    QFont titleFont;
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2c3e50;");
    mainLayout->addWidget(titleLabel);

    // Overall Statistics Section
    QGroupBox *overallGroup = new QGroupBox("Overall Statistics", this);
    QGridLayout *overallLayout = new QGridLayout(overallGroup);
    overallLayout->setSpacing(20);

    // Total Spots
    QLabel *totalLabel = new QLabel("Total Spots:", this);
    totalLabel->setStyleSheet("font-weight: bold;");
    totalSpotsLabel = new QLabel("0", this);
    totalSpotsLabel->setStyleSheet("font-size: 18px; color: #3498db; font-weight: bold;");
    overallLayout->addWidget(totalLabel, 0, 0);
    overallLayout->addWidget(totalSpotsLabel, 0, 1);

    // Occupied Spots
    QLabel *occupiedLabel = new QLabel("Occupied Spots:", this);
    occupiedLabel->setStyleSheet("font-weight: bold;");
    occupiedSpotsLabel = new QLabel("0", this);
    occupiedSpotsLabel->setStyleSheet("font-size: 18px; color: #e74c3c; font-weight: bold;");
    overallLayout->addWidget(occupiedLabel, 0, 2);
    overallLayout->addWidget(occupiedSpotsLabel, 0, 3);

    // Available Spots
    QLabel *availableLabel = new QLabel("Available Spots:", this);
    availableLabel->setStyleSheet("font-weight: bold;");
    availableSpotsLabel = new QLabel("0", this);
    availableSpotsLabel->setStyleSheet("font-size: 18px; color: #2ecc71; font-weight: bold;");
    overallLayout->addWidget(availableLabel, 0, 4);
    overallLayout->addWidget(availableSpotsLabel, 0, 5);

    // Occupancy Percentage with Progress Bar
    QLabel *occupancyLabel = new QLabel("Occupancy Rate:", this);
    occupancyLabel->setStyleSheet("font-weight: bold;");
    overallLayout->addWidget(occupancyLabel, 1, 0);

    QHBoxLayout *occupancyLayout = new QHBoxLayout();
    QProgressBar *occupancyBar = new QProgressBar(this);
    occupancyBar->setMaximumHeight(25);
    occupancyBar->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid #bdc3c7;"
        "    border-radius: 5px;"
        "    text-align: center;"
        "    background-color: #ecf0f1;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #3498db;"
        "}"
    );
    occupancyPercentageLabel = new QLabel("0%", this);
    occupancyPercentageLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #3498db;");
    occupancyLayout->addWidget(occupancyBar, 1);
    occupancyLayout->addWidget(occupancyPercentageLabel, 0);
    overallLayout->addLayout(occupancyLayout, 1, 1, 1, 5);

    overallGroup->setStyleSheet(
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

    mainLayout->addWidget(overallGroup);

    // Spot Type Distribution
    QGroupBox *spotTypeGroup = new QGroupBox("Parking Spot Types", this);
    QGridLayout *spotTypeLayout = new QGridLayout(spotTypeGroup);
    spotTypeLayout->setSpacing(15);

    // Standard Spots
    QLabel *standardLabel = new QLabel("🚗 Standard Spots:", this);
    standardLabel->setStyleSheet("font-weight: bold;");
    standardSpotsLabel = new QLabel("0", this);
    standardSpotsLabel->setStyleSheet("font-size: 16px; color: #27ae60; font-weight: bold;");
    spotTypeLayout->addWidget(standardLabel, 0, 0);
    spotTypeLayout->addWidget(standardSpotsLabel, 0, 1);

    // Disabled Spots
    QLabel *disabledLabel = new QLabel("♿ Disabled Spots:", this);
    disabledLabel->setStyleSheet("font-weight: bold;");
    disabledSpotsLabel = new QLabel("0", this);
    disabledSpotsLabel->setStyleSheet("font-size: 16px; color: #f39c12; font-weight: bold;");
    spotTypeLayout->addWidget(disabledLabel, 0, 2);
    spotTypeLayout->addWidget(disabledSpotsLabel, 0, 3);

    // VIP Spots
    QLabel *vipLabel = new QLabel("⭐ VIP Spots:", this);
    vipLabel->setStyleSheet("font-weight: bold;");
    vipSpotsLabel = new QLabel("0", this);
    vipSpotsLabel->setStyleSheet("font-size: 16px; color: #9b59b6; font-weight: bold;");
    spotTypeLayout->addWidget(vipLabel, 0, 4);
    spotTypeLayout->addWidget(vipSpotsLabel, 0, 5);

    spotTypeGroup->setStyleSheet(
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

    mainLayout->addWidget(spotTypeGroup);

    // Activity Statistics
    QGroupBox *activityGroup = new QGroupBox("Activity Statistics", this);
    QGridLayout *activityLayout = new QGridLayout(activityGroup);
    activityLayout->setSpacing(20);

    // Total Parkings
    QLabel *totalParkingLabel = new QLabel("Total Parkings:", this);
    totalParkingLabel->setStyleSheet("font-weight: bold;");
    totalParkingsLabel = new QLabel("0", this);
    totalParkingsLabel->setStyleSheet("font-size: 18px; color: #2980b9; font-weight: bold;");
    activityLayout->addWidget(totalParkingLabel, 0, 0);
    activityLayout->addWidget(totalParkingsLabel, 0, 1);

    // Total Exits
    QLabel *totalExitLabel = new QLabel("Total Exits:", this);
    totalExitLabel->setStyleSheet("font-weight: bold;");
    totalExitsLabel = new QLabel("0", this);
    totalExitsLabel->setStyleSheet("font-size: 18px; color: #8e44ad; font-weight: bold;");
    activityLayout->addWidget(totalExitLabel, 0, 2);
    activityLayout->addWidget(totalExitsLabel, 0, 3);

    // Total Revenue
    QLabel *revenueLabel = new QLabel("💰 Total Revenue:", this);
    revenueLabel->setStyleSheet("font-weight: bold;");
    totalRevenueLabel = new QLabel("₹0.00", this);
    totalRevenueLabel->setStyleSheet("font-size: 18px; color: #16a085; font-weight: bold;");
    activityLayout->addWidget(revenueLabel, 0, 4);
    activityLayout->addWidget(totalRevenueLabel, 0, 5);

    activityGroup->setStyleSheet(
        "QGroupBox {"
        "    border: 2px solid #2980b9;"
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

    mainLayout->addWidget(activityGroup);

    mainLayout->addStretch();
}

void DashboardWidget::refresh() {
    updateStats();
}

void DashboardWidget::updateStats() {
    // These values would be retrieved from the ParkingLot object
    // For now, showing placeholder values
    totalSpotsLabel->setText("30");
    occupiedSpotsLabel->setText("18");
    availableSpotsLabel->setText("12");
    
    int occupancyRate = (18 * 100) / 30;
    occupancyPercentageLabel->setText(QString::number(occupancyRate) + "%");
    
    standardSpotsLabel->setText("15");
    disabledSpotsLabel->setText("8");
    vipSpotsLabel->setText("7");
    
    totalParkingsLabel->setText("245");
    totalExitsLabel->setText("227");
    totalRevenueLabel->setText("₹18,550.00");
}
