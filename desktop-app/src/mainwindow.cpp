#include "mainwindow.h"
#include "dashboardwidget.h"
#include "vehicleentrywidget.h"
#include "vehicleexitwidget.h"
#include "lotstatuswidget.h"
#include "transactionwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QPushButton>
#include <QLabel>
#include <QDateTime>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), clockLabel(nullptr), clockTimer(nullptr), parkingLot(nullptr) {
    // Initialize parking lot
    parkingLot = new ParkingLot();

    // Setup window
    setWindowTitle("Smart Parking Lot Management System");
    setWindowIcon(QIcon(":/icons/parking.png"));
    setGeometry(100, 100, 1200, 800);

    // Create stacked widget for different pages
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Create widgets for each page
    dashboardWidget = new DashboardWidget(parkingLot, this);
    vehicleEntryWidget = new VehicleEntryWidget(parkingLot, this);
    vehicleExitWidget = new VehicleExitWidget(parkingLot, this);
    lotStatusWidget = new LotStatusWidget(parkingLot, this);
    transactionWidget = new TransactionWidget(parkingLot, this);

    // Add widgets to stacked widget
    stackedWidget->addWidget(dashboardWidget);      // 0
    stackedWidget->addWidget(vehicleEntryWidget);   // 1
    stackedWidget->addWidget(vehicleExitWidget);    // 2
    stackedWidget->addWidget(lotStatusWidget);      // 3
    stackedWidget->addWidget(transactionWidget);    // 4

    // Setup UI elements
    setupUI();
    createMenuBar();
    createToolBar();
    createStatusBar();

    // Setup clock update
    clockTimer = new QTimer(this);
    connect(clockTimer, &QTimer::timeout, this, &MainWindow::updateClock);
    clockTimer->start(1000);
    updateClock();

    // Show dashboard on startup
    showDashboard();

    setStyleSheet(R"(
        QMainWindow {
            background-color: #f0f2f5;
        }
        QToolBar {
            background-color: #2c3e50;
            border: none;
        }
        QToolBar QToolButton {
            color: white;
            padding: 8px 15px;
            min-width: 120px;
            border: none;
            border-radius: 4px;
        }
        QToolBar QToolButton:hover {
            background-color: #34495e;
        }
        QToolBar QToolButton:pressed {
            background-color: #1a252f;
        }
        QStatusBar {
            background-color: #2c3e50;
            color: white;
        }
        QMenuBar {
            background-color: #2c3e50;
            color: white;
        }
        QMenuBar::item:selected {
            background-color: #34495e;
        }
        QMenu {
            background-color: #2c3e50;
            color: white;
        }
        QMenu::item:selected {
            background-color: #34495e;
        }
    )");
}

MainWindow::~MainWindow() {
    if (clockTimer) {
        clockTimer->stop();
    }
    if (parkingLot) {
        delete parkingLot;
    }
}

void MainWindow::setupUI() {
    // Central widget is already set to stackedWidget
}

void MainWindow::createMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // File menu
    QMenu *fileMenu = menuBar->addMenu(tr("&File"));
    
    QAction *exitAction = fileMenu->addAction(tr("E&xit"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // View menu
    QMenu *viewMenu = menuBar->addMenu(tr("&View"));
    
    QAction *dashboardAction = viewMenu->addAction(tr("&Dashboard"));
    connect(dashboardAction, &QAction::triggered, this, &MainWindow::showDashboard);
    
    QAction *lotStatusAction = viewMenu->addAction(tr("&Parking Lot Status"));
    connect(lotStatusAction, &QAction::triggered, this, &MainWindow::showLotStatus);
    
    QAction *transactionAction = viewMenu->addAction(tr("&Transactions"));
    connect(transactionAction, &QAction::triggered, this, &MainWindow::showTransactions);

    // Operations menu
    QMenu *operationsMenu = menuBar->addMenu(tr("&Operations"));
    
    QAction *parkAction = operationsMenu->addAction(tr("&Park Vehicle"));
    connect(parkAction, &QAction::triggered, this, &MainWindow::showVehicleEntry);
    
    QAction *exitAction2 = operationsMenu->addAction(tr("&Vehicle Exit"));
    connect(exitAction2, &QAction::triggered, this, &MainWindow::showVehicleExit);

    // Help menu
    QMenu *helpMenu = menuBar->addMenu(tr("&Help"));
    
    QAction *aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, &QAction::triggered, this, [this]() {
        // About dialog would go here
    });
}

void MainWindow::createToolBar() {
    QToolBar *toolBar = addToolBar(tr("Main Toolbar"));
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(24, 24));

    QPushButton *dashboardBtn = new QPushButton(tr("📊 Dashboard"));
    connect(dashboardBtn, &QPushButton::clicked, this, &MainWindow::showDashboard);
    toolBar->addWidget(dashboardBtn);

    toolBar->addSeparator();

    QPushButton *parkBtn = new QPushButton(tr("🚗 Park Vehicle"));
    connect(parkBtn, &QPushButton::clicked, this, &MainWindow::showVehicleEntry);
    toolBar->addWidget(parkBtn);

    QPushButton *exitBtn = new QPushButton(tr("🚙 Vehicle Exit"));
    connect(exitBtn, &QPushButton::clicked, this, &MainWindow::showVehicleExit);
    toolBar->addWidget(exitBtn);

    toolBar->addSeparator();

    QPushButton *statusBtn = new QPushButton(tr("📍 Lot Status"));
    connect(statusBtn, &QPushButton::clicked, this, &MainWindow::showLotStatus);
    toolBar->addWidget(statusBtn);

    QPushButton *transactionBtn = new QPushButton(tr("📋 Transactions"));
    connect(transactionBtn, &QPushButton::clicked, this, &MainWindow::showTransactions);
    toolBar->addWidget(transactionBtn);

    toolBar->addStretch();

    clockLabel = new QLabel(this);
    clockLabel->setMinimumWidth(150);
    clockLabel->setStyleSheet("color: white; font-weight: bold; padding: 5px;");
    toolBar->addWidget(clockLabel);
}

void MainWindow::createStatusBar() {
    QLabel *statusLabel = new QLabel(tr("Ready"), this);
    statusBar()->addWidget(statusLabel);
    statusBar()->setSizeGripEnabled(false);
}

void MainWindow::showDashboard() {
    stackedWidget->setCurrentIndex(0);
    dashboardWidget->refresh();
}

void MainWindow::showVehicleEntry() {
    stackedWidget->setCurrentIndex(1);
    vehicleEntryWidget->resetForm();
}

void MainWindow::showVehicleExit() {
    stackedWidget->setCurrentIndex(2);
    vehicleExitWidget->resetForm();
}

void MainWindow::showLotStatus() {
    stackedWidget->setCurrentIndex(3);
    lotStatusWidget->refresh();
}

void MainWindow::showTransactions() {
    stackedWidget->setCurrentIndex(4);
    transactionWidget->refresh();
}

void MainWindow::updateClock() {
    if (clockLabel) {
        QDateTime now = QDateTime::currentDateTime();
        clockLabel->setText(now.toString("hh:mm:ss\ndddd"));
    }
}
