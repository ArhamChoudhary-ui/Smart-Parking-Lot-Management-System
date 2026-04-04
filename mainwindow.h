#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLabel>
#include <QTimer>
#include "ParkingLot.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showDashboard();
    void showVehicleEntry();
    void showVehicleExit();
    void showLotStatus();
    void showTransactions();
    void updateClock();

private:
    void setupUI();
    void createMenuBar();
    void createToolBar();
    void createStatusBar();

    QStackedWidget *stackedWidget;
    QLabel *clockLabel;
    QTimer *clockTimer;
    ParkingLot *parkingLot;

    class DashboardWidget *dashboardWidget;
    class VehicleEntryWidget *vehicleEntryWidget;
    class VehicleExitWidget *vehicleExitWidget;
    class LotStatusWidget *lotStatusWidget;
    class TransactionWidget *transactionWidget;
};

#endif // MAINWINDOW_H
