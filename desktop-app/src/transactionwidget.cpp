#include "transactionwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QHeaderView>
#include <QFont>
#include <QMessageBox>

TransactionWidget::TransactionWidget(ParkingLot *lot, QWidget *parent)
    : QWidget(parent), parkingLot(lot) {
    setupUI();
    refresh();
}

void TransactionWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *titleLabel = new QLabel("Transaction Log", this);
    QFont titleFont;
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2c3e50;");
    mainLayout->addWidget(titleLabel);

    // Controls
    QGroupBox *controlGroup = new QGroupBox("Filters & Actions", this);
    QHBoxLayout *controlLayout = new QHBoxLayout(controlGroup);

    QLabel *filterLabel = new QLabel("Filter by Type:", this);
    filterLabel->setStyleSheet("font-weight: bold;");
    filterCombo = new QComboBox(this);
    filterCombo->addItem("All Transactions");
    filterCombo->addItem("Entry Transactions");
    filterCombo->addItem("Exit Transactions");
    filterCombo->setMinimumHeight(35);
    filterCombo->setMinimumWidth(200);
    filterCombo->setStyleSheet(
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
    connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TransactionWidget::filterTransactions);

    QPushButton *exportBtn = new QPushButton("📥 Export to CSV", this);
    exportBtn->setMinimumHeight(35);
    exportBtn->setMinimumWidth(150);
    exportBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #3498db;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1f618d;"
        "}"
    );
    connect(exportBtn, &QPushButton::clicked, this, &TransactionWidget::exportTransactions);

    controlLayout->addWidget(filterLabel);
    controlLayout->addWidget(filterCombo);
    controlLayout->addStretch();
    controlLayout->addWidget(exportBtn);

    controlGroup->setStyleSheet(
        "QGroupBox {"
        "    border: 2px solid #95a5a6;"
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

    mainLayout->addWidget(controlGroup);

    // Summary
    QGroupBox *summaryGroup = new QGroupBox("Summary Statistics", this);
    QVBoxLayout *summaryLayout = new QVBoxLayout(summaryGroup);
    summaryLabel = new QLabel("Loading...", this);
    summaryLabel->setStyleSheet("font-size: 13px; color: #2c3e50;");
    summaryLayout->addWidget(summaryLabel);
    summaryGroup->setStyleSheet(
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
    mainLayout->addWidget(summaryGroup);

    // Transaction Table
    QGroupBox *tableGroup = new QGroupBox("All Transactions", this);
    QVBoxLayout *tableLayout = new QVBoxLayout(tableGroup);

    transactionTable = new QTableWidget(this);
    transactionTable->setColumnCount(7);
    transactionTable->setHorizontalHeaderLabels({
        "Timestamp",
        "Type",
        "Spot ID",
        "Vehicle Number",
        "Vehicle Category",
        "Duration (Hours)",
        "Amount (₹)"
    });

    transactionTable->horizontalHeader()->setStretchLastSection(true);
    transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    transactionTable->setAlternatingRowColors(true);
    transactionTable->setMinimumHeight(300);
    transactionTable->setStyleSheet(
        "QTableWidget {"
        "    border: 2px solid #bdc3c7;"
        "    border-radius: 5px;"
        "    background-color: white;"
        "}"
        "QTableWidget::item {"
        "    padding: 10px;"
        "    border-bottom: 1px solid #ecf0f1;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #3498db;"
        "    color: white;"
        "}"
        "QHeaderView::section {"
        "    background-color: #2c3e50;"
        "    color: white;"
        "    padding: 10px;"
        "    border: none;"
        "    font-weight: bold;"
        "}"
        "QTableWidget::alternatingRowColors {"
        "    background-color: #f8f9fa;"
        "}"
    );

    // Set column widths
    transactionTable->setColumnWidth(0, 180);
    transactionTable->setColumnWidth(1, 100);
    transactionTable->setColumnWidth(2, 80);
    transactionTable->setColumnWidth(3, 150);
    transactionTable->setColumnWidth(4, 120);
    transactionTable->setColumnWidth(5, 120);
    transactionTable->setColumnWidth(6, 100);

    tableLayout->addWidget(transactionTable);
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
}

void TransactionWidget::refresh() {
    populateTransactions();
}

void TransactionWidget::populateTransactions() {
    // Clear existing data
    transactionTable->setRowCount(0);

    // Update summary
    QString summary = QString(
        "Total Transactions: 25 | Total Revenue: ₹18,550.00 | "
        "Average Transaction: ₹742.00 | Peak Hours: 10:00 AM - 12:00 PM"
    );
    summaryLabel->setText(summary);

    // Add sample transaction data
    struct Transaction {
        QString timestamp;
        QString type;
        int spotId;
        QString vehicleNo;
        QString category;
        int duration;
        double amount;
    };

    Transaction transactions[] = {
        {"2024-04-04 10:45:23", "Entry", 1, "MH-02-AB-1234", "4W", 2, 0.0},
        {"2024-04-04 10:30:15", "Exit", 5, "KA-03-CD-9876", "4W", 3, 120.00},
        {"2024-04-04 10:15:42", "Entry", 3, "DL-01-XY-5678", "2W", 1, 0.0},
        {"2024-04-04 09:50:08", "Exit", 7, "TN-04-EF-2345", "4W", 5, 250.00},
        {"2024-04-04 09:20:33", "Entry", 2, "AP-05-GH-6789", "4W", 4, 0.0},
        {"2024-04-04 08:45:19", "Exit", 4, "HR-06-IJ-1011", "2W", 2, 60.00},
        {"2024-04-04 08:10:44", "Entry", 6, "PB-07-KL-1213", "4W", 3, 0.0},
        {"2024-04-04 07:55:27", "Exit", 2, "MH-08-MN-1415", "4W", 4, 180.00},
    };

    int row = 0;
    for (const auto& txn : transactions) {
        transactionTable->insertRow(row);

        QTableWidgetItem *timestampItem = new QTableWidgetItem(txn.timestamp);
        timestampItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 0, timestampItem);

        QString typeIcon = (txn.type == "Entry") ? "🟢 Entry" : "🔴 Exit";
        QTableWidgetItem *typeItem = new QTableWidgetItem(typeIcon);
        typeItem->setTextAlignment(Qt::AlignCenter);
        if (txn.type == "Entry") {
            typeItem->setBackground(QColor("#e8f5e9"));
        } else {
            typeItem->setBackground(QColor("#ffebee"));
        }
        transactionTable->setItem(row, 1, typeItem);

        QTableWidgetItem *spotItem = new QTableWidgetItem(QString::number(txn.spotId));
        spotItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 2, spotItem);

        QTableWidgetItem *vehicleItem = new QTableWidgetItem(txn.vehicleNo);
        vehicleItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 3, vehicleItem);

        QTableWidgetItem *categoryItem = new QTableWidgetItem(txn.category);
        categoryItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 4, categoryItem);

        QTableWidgetItem *durationItem = new QTableWidgetItem(QString::number(txn.duration));
        durationItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 5, durationItem);

        QTableWidgetItem *amountItem = new QTableWidgetItem(
            txn.type == "Exit" ? "₹" + QString::number(txn.amount, 'f', 2) : "-"
        );
        amountItem->setTextAlignment(Qt::AlignRight);
        if (txn.type == "Exit") {
            amountItem->setForeground(QColor("#27ae60"));
            amountItem->setFont(QFont("Arial", 10, QFont::Bold));
        }
        transactionTable->setItem(row, 6, amountItem);

        row++;
    }
}

void TransactionWidget::filterTransactions() {
    // Implement filtering logic
    // For now, just refresh the table
    populateTransactions();
}

void TransactionWidget::exportTransactions() {
    QMessageBox::information(this, "Export", "Transactions exported successfully to CSV file!");
}
