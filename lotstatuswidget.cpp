#include "lotstatuswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QFont>
#include <QGroupBox>

LotStatusWidget::LotStatusWidget(ParkingLot *lot, QWidget *parent)
    : QWidget(parent), parkingLot(lot) {
    setupUI();
    refresh();
}

void LotStatusWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *titleLabel = new QLabel("Parking Lot Status", this);
    QFont titleFont;
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2c3e50;");
    mainLayout->addWidget(titleLabel);

    // Overall Status
    QGroupBox *overallGroup = new QGroupBox("Overall Status", this);
    QVBoxLayout *overallLayout = new QVBoxLayout(overallGroup);
    overallStatusLabel = new QLabel("Loading...", this);
    overallStatusLabel->setWordWrap(true);
    overallStatusLabel->setStyleSheet("font-size: 14px; color: #2c3e50;");
    overallLayout->addWidget(overallStatusLabel);
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

    // Parking Spots Table
    QGroupBox *tableGroup = new QGroupBox("Detailed Spot Status", this);
    QVBoxLayout *tableLayout = new QVBoxLayout(tableGroup);

    spotTable = new QTableWidget(this);
    spotTable->setColumnCount(6);
    spotTable->setHorizontalHeaderLabels({
        "Spot ID",
        "Type",
        "Status",
        "Vehicle Number",
        "Vehicle Category",
        "Allotted Hours"
    });

    spotTable->horizontalHeader()->setStretchLastSection(true);
    spotTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    spotTable->setSelectionMode(QAbstractItemView::SingleSelection);
    spotTable->setAlternatingRowColors(true);
    spotTable->setStyleSheet(
        "QTableWidget {"
        "    border: 2px solid #bdc3c7;"
        "    border-radius: 5px;"
        "    background-color: white;"
        "}"
        "QTableWidget::item {"
        "    padding: 8px;"
        "    border-bottom: 1px solid #ecf0f1;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #3498db;"
        "    color: white;"
        "}"
        "QHeaderView::section {"
        "    background-color: #2c3e50;"
        "    color: white;"
        "    padding: 8px;"
        "    border: none;"
        "    font-weight: bold;"
        "}"
        "QTableWidget::alternatingRowColors {"
        "    background-color: #f8f9fa;"
        "}"
    );

    // Set column widths
    spotTable->setColumnWidth(0, 80);
    spotTable->setColumnWidth(1, 100);
    spotTable->setColumnWidth(2, 100);
    spotTable->setColumnWidth(3, 150);
    spotTable->setColumnWidth(4, 120);
    spotTable->setColumnWidth(5, 120);

    tableLayout->addWidget(spotTable);
    tableGroup->setStyleSheet(
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

    mainLayout->addWidget(tableGroup, 1);

    // Legend
    QGroupBox *legendGroup = new QGroupBox("Legend", this);
    QHBoxLayout *legendLayout = new QHBoxLayout(legendGroup);

    QLabel *availableLabel = new QLabel("🟢 Available", this);
    availableLabel->setStyleSheet("color: #27ae60; font-weight: bold;");
    legendLayout->addWidget(availableLabel);

    QLabel *occupiedLabel = new QLabel("🔴 Occupied", this);
    occupiedLabel->setStyleSheet("color: #e74c3c; font-weight: bold;");
    legendLayout->addWidget(occupiedLabel);

    QLabel *standardLabel = new QLabel("🚗 Standard", this);
    standardLabel->setStyleSheet("color: #3498db; font-weight: bold;");
    legendLayout->addWidget(standardLabel);

    QLabel *disabledLabel = new QLabel("♿ Disabled", this);
    disabledLabel->setStyleSheet("color: #f39c12; font-weight: bold;");
    legendLayout->addWidget(disabledLabel);

    QLabel *vipLabel = new QLabel("⭐ VIP", this);
    vipLabel->setStyleSheet("color: #9b59b6; font-weight: bold;");
    legendLayout->addWidget(vipLabel);

    legendLayout->addStretch();

    legendGroup->setStyleSheet(
        "QGroupBox {"
        "    border: 2px solid #bdc3c7;"
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

    mainLayout->addWidget(legendGroup);
}

void LotStatusWidget::refresh() {
    populateLotStatus();
}

void LotStatusWidget::populateLotStatus() {
    // Clear existing data
    spotTable->setRowCount(0);

    // Populate sample data
    int totalSpots = 30;
    int occupiedSpots = 18;

    QString statusText = QString(
        "Total Parking Spots: %1\n"
        "Occupied Spots: %2\n"
        "Available Spots: %3\n"
        "Occupancy Rate: %4%"
    ).arg(totalSpots).arg(occupiedSpots).arg(totalSpots - occupiedSpots).arg((occupiedSpots * 100) / totalSpots);

    overallStatusLabel->setText(statusText);

    // Add sample spot data
    const char* spotTypes[] = {"Standard", "Standard", "Standard", "Disabled", "Disabled", "VIP", "VIP"};
    const bool spotStatus[] = {true, false, true, true, false, false, true};
    const char* vehicles[] = {"MH-02-AB-1234", "", "DL-01-XY-5678", "KA-03-CD-9876", "", "", "TN-04-EF-2345"};

    int row = 0;
    for (int i = 1; i <= 7; i++) {
        spotTable->insertRow(row);

        // Spot ID
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(i));
        idItem->setTextAlignment(Qt::AlignCenter);
        spotTable->setItem(row, 0, idItem);

        // Type
        QString type = spotTypes[i-1];
        QString typeEmoji = (type == "Standard" ? "🚗" : 
                            (type == "Disabled" ? "♿" : "⭐"));
        QTableWidgetItem *typeItem = new QTableWidgetItem(typeEmoji + " " + type);
        spotTable->setItem(row, 1, typeItem);

        // Status
        QString statusStr = spotStatus[i-1] ? "🔴 Occupied" : "🟢 Available";
        QTableWidgetItem *statusItem = new QTableWidgetItem(statusStr);
        statusItem->setTextAlignment(Qt::AlignCenter);
        if (spotStatus[i-1]) {
            statusItem->setBackground(QColor("#ffebee"));
        } else {
            statusItem->setBackground(QColor("#e8f5e9"));
        }
        spotTable->setItem(row, 2, statusItem);

        // Vehicle Number
        QTableWidgetItem *vehicleItem = new QTableWidgetItem(vehicles[i-1]);
        vehicleItem->setTextAlignment(Qt::AlignCenter);
        spotTable->setItem(row, 3, vehicleItem);

        // Vehicle Category
        QTableWidgetItem *categoryItem = new QTableWidgetItem(spotStatus[i-1] ? "4W" : "-");
        categoryItem->setTextAlignment(Qt::AlignCenter);
        spotTable->setItem(row, 4, categoryItem);

        // Allotted Hours
        QTableWidgetItem *hoursItem = new QTableWidgetItem(spotStatus[i-1] ? "2" : "-");
        hoursItem->setTextAlignment(Qt::AlignCenter);
        spotTable->setItem(row, 5, hoursItem);

        row++;
    }
}
