#ifndef PARKINGLOT_H
#define PARKINGLOT_H

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "DerivedSpots.h"

using namespace std;

class ParkingLot {
private:
    vector<ParkingSpot*> spots;
    string logFileName;
    int totalParks;
    int totalExits;

    struct BillingDetails {
        int allottedHours;
        int actualHours;
        int actualExtraMinutes;
        int overtimeMinutes;
        double spotFee;
        double vehicleInitialCharge;
        double overtimeFine;
        double totalFee;
    };

    string getCurrentTime() const {
        time_t now = time(nullptr);
        tm* local = localtime(&now);
        char buffer[64];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local);
        return string(buffer);
    }

    void writeLog(const string& entry) const {
        ofstream outFile(logFileName.c_str(), ios::app);
        if (outFile) {
            outFile << "[" << getCurrentTime() << "] " << entry << endl;
        }
    }

    ParkingSpot* findSpotById(int spotId) const {
        for (size_t i = 0; i < spots.size(); ++i) {
            if (spots[i]->getSpotId() == spotId) {
                return spots[i];
            }
        }
        return nullptr;
    }

    double getVehicleInitialCharge(const string& vehicleCategory) const {
        if (vehicleCategory == "2W") {
            return 20.0;
        }
        if (vehicleCategory == "4W") {
            return 50.0;
        }
        throw invalid_argument("Vehicle category must be 2W or 4W.");
    }

    double getSpotHourlyRate(const ParkingSpot* spot) const {
        string type = spot->getSpotTypeDescription();
        if (type == "Standard") {
            return 20.0;
        }
        if (type == "Disabled") {
            return 10.0;
        }
        if (type == "VIP") {
            return 50.0;
        }
        return 0.0;
    }

    BillingDetails calculateBilling(ParkingSpot* spot, int actualHours, int actualExtraMinutes) const {
        if (actualHours < 0) {
            throw invalid_argument("Hours cannot be negative.");
        }
        if (actualExtraMinutes < 0 || actualExtraMinutes > 59) {
            throw invalid_argument("Extra minutes must be between 0 and 59.");
        }
        if (actualHours == 0 && actualExtraMinutes == 0) {
            throw invalid_argument("Parking duration must be greater than 0.");
        }

        BillingDetails bill;
        bill.allottedHours = spot->getAllottedHours();
        bill.actualHours = actualHours;
        bill.actualExtraMinutes = actualExtraMinutes;

        int billableHours = actualHours;
        if (actualExtraMinutes > 0) {
            billableHours += 1;
        }

        bill.spotFee = spot->calculateFee(billableHours);
        bill.vehicleInitialCharge = getVehicleInitialCharge(spot->getVehicleCategory());

        int allottedMinutes = bill.allottedHours * 60;
        int actualMinutes = (actualHours * 60) + actualExtraMinutes;
        bill.overtimeMinutes = 0;
        if (actualMinutes > allottedMinutes) {
            bill.overtimeMinutes = actualMinutes - allottedMinutes;
        }

        bill.overtimeFine = 0.0;
        if (bill.overtimeMinutes > 0) {
            int tenMinuteBlocks = (bill.overtimeMinutes + 9) / 10;
            bill.overtimeFine += tenMinuteBlocks * (0.5 * getSpotHourlyRate(spot));

            if (bill.overtimeMinutes > 60) {
                bill.overtimeFine += 0.5 * bill.vehicleInitialCharge;
            }
        }

        bill.totalFee = bill.vehicleInitialCharge + bill.overtimeFine;
        return bill;
    }

public:
    struct Receipt {
        int spotId;
        string spotType;
        string vehicleNumber;
        string vehicleCategory;
        int allottedHours;
        int actualHours;
        int actualExtraMinutes;
        int overtimeMinutes;
        double spotFee;
        double vehicleInitialCharge;
        double overtimeFine;
        double totalFee;
        double paidAmount;
        double change;
        string timestamp;
    };

    ParkingLot(int standardCount = 4, int disabledCount = 2, int vipCount = 2,
               const string& logFile = "parking_log.txt")
        : logFileName(logFile), totalParks(0), totalExits(0) {
        int nextId = 1;

        for (int i = 0; i < standardCount; ++i) {
            spots.push_back(new StandardSpot(nextId++));
        }

        for (int i = 0; i < disabledCount; ++i) {
            spots.push_back(new DisabledSpot(nextId++));
        }

        for (int i = 0; i < vipCount; ++i) {
            spots.push_back(new VIPSpot(nextId++));
        }

        cout << "ParkingLot constructor called. Total spots created: " << spots.size() << endl;
    }

    ~ParkingLot() {
        for (size_t i = 0; i < spots.size(); ++i) {
            delete spots[i];
        }
        cout << "ParkingLot destructor called" << endl;
    }

    void parkVehicle(int spotTypeChoice, const string& vehicleNumber, const string& vehicleCategory, int allottedHours) {
        if (vehicleNumber.empty()) {
            throw invalid_argument("Vehicle number cannot be empty.");
        }
        if (allottedHours <= 0) {
            throw invalid_argument("Allotted hours must be greater than 0.");
        }
        if (vehicleCategory != "2W" && vehicleCategory != "4W") {
            throw invalid_argument("Vehicle category must be 2W or 4W.");
        }

        ParkingSpot* freeSpot = nullptr;

        for (size_t i = 0; i < spots.size(); ++i) {
            if (spots[i]->isOccupied()) {
                continue;
            }

            if (spotTypeChoice == 1 && dynamic_cast<StandardSpot*>(spots[i]) != nullptr) {
                freeSpot = spots[i];
                break;
            }
            if (spotTypeChoice == 2 && dynamic_cast<DisabledSpot*>(spots[i]) != nullptr) {
                freeSpot = spots[i];
                break;
            }
            if (spotTypeChoice == 3 && dynamic_cast<VIPSpot*>(spots[i]) != nullptr) {
                freeSpot = spots[i];
                break;
            }
        }

        if (spotTypeChoice < 1 || spotTypeChoice > 3) {
            throw invalid_argument("Invalid spot type. Enter 1, 2, or 3.");
        }

        if (freeSpot == nullptr) {
            throw runtime_error("Selected spot type is full.");
        }

        freeSpot->parkVehicle(vehicleNumber, vehicleCategory, allottedHours);
        totalParks++;

        string logEntry = "PARK   Spot " + to_string(freeSpot->getSpotId()) + 
                          "   Vehicle: " + vehicleNumber + 
                          "   Category: " + vehicleCategory + 
                          "   Allotted: " + to_string(allottedHours) + " hrs";
        writeLog(logEntry);

        cout << "Vehicle parked successfully at Spot ID " << freeSpot->getSpotId() << endl;
    }

    double exitVehicle(int spotId, int hours) {
        if (hours <= 0) {
            throw invalid_argument("Hours must be greater than 0.");
        }

        ParkingSpot* spot = findSpotById(spotId);
        if (spot == nullptr) {
            throw runtime_error("Wrong spot ID. No such spot found.");
        }

        if (!spot->isOccupied()) {
            throw runtime_error("Selected spot is already empty.");
        }

        string vehicleNo = spot->getVehicleNumber();
        double fee = spot->calculateFee(hours);
        spot->removeVehicle();
        totalExits++;

        string logEntry = "EXIT | SpotID=" + to_string(spotId) +
                               " | Type=" + spot->getSpotTypeDescription() +
                               " | Vehicle=" + vehicleNo +
                               " | Hours=" + to_string(hours) +
                               " | Fee=" + to_string(fee);
        writeLog(logEntry);

        return fee;
    }

    double getExitFeePreview(int spotId, int actualHours, int actualExtraMinutes) const {
        ParkingSpot* spot = findSpotById(spotId);
        if (spot == nullptr) {
            throw runtime_error("Wrong spot ID. No such spot found.");
        }

        if (!spot->isOccupied()) {
            throw runtime_error("Selected spot is already empty.");
        }

        BillingDetails bill = calculateBilling(spot, actualHours, actualExtraMinutes);
        return bill.totalFee;
    }

    Receipt exitVehicleWithPayment(int spotId, int actualHours, int actualExtraMinutes, double paidAmount) {
        if (paidAmount <= 0) {
            throw invalid_argument("Paid amount must be greater than 0.");
        }

        ParkingSpot* spot = findSpotById(spotId);
        if (spot == nullptr) {
            throw runtime_error("Wrong spot ID. No such spot found.");
        }

        if (!spot->isOccupied()) {
            throw runtime_error("Selected spot is already empty.");
        }

        string vehicleNo = spot->getVehicleNumber();
        string spotType = spot->getSpotTypeDescription();
        string vehicleCategory = spot->getVehicleCategory();
        BillingDetails bill = calculateBilling(spot, actualHours, actualExtraMinutes);

        if (paidAmount < bill.totalFee) {
            throw runtime_error("Insufficient payment amount.");
        }

        double change = paidAmount - bill.totalFee;
        spot->removeVehicle();
        totalExits++;

        string logExit = "EXIT   Spot " + to_string(spotId) + 
                         "   Vehicle: " + vehicleNo + 
                         "   Used: " + to_string(bill.actualHours) + "h " + to_string(bill.actualExtraMinutes) + "m" + 
                         "   Base: " + to_string(bill.vehicleInitialCharge) + 
                         "   Fine: " + to_string(bill.overtimeFine) + 
                         "   Total: " + to_string(bill.totalFee);
        writeLog(logExit);

        Receipt receipt;
        receipt.spotId = spotId;
        receipt.spotType = spotType;
        receipt.vehicleNumber = vehicleNo;
        receipt.vehicleCategory = vehicleCategory;
        receipt.allottedHours = bill.allottedHours;
        receipt.actualHours = bill.actualHours;
        receipt.actualExtraMinutes = bill.actualExtraMinutes;
        receipt.overtimeMinutes = bill.overtimeMinutes;
        receipt.spotFee = bill.spotFee;
        receipt.vehicleInitialCharge = bill.vehicleInitialCharge;
        receipt.overtimeFine = bill.overtimeFine;
        receipt.totalFee = bill.totalFee;
        receipt.paidAmount = paidAmount;
        receipt.change = change;
        receipt.timestamp = getCurrentTime();

        return receipt;
    }

    void viewLotStatus() const {
        cout << "\n========== LOT STATUS ==========" << endl;
        cout << left << setw(10) << "SpotID"
             << setw(12) << "Type"
             << setw(12) << "Occupied"
               << setw(15) << "Vehicle"
               << setw(10) << "Cat"
               << setw(10) << "AllowH" << endl;
           cout << "---------------------------------------------------------------" << endl;

        for (size_t i = 0; i < spots.size(); ++i) {
            cout << left << setw(10) << spots[i]->getSpotId()
                 << setw(12) << spots[i]->getSpotTypeDescription()
                 << setw(12) << (spots[i]->isOccupied() ? "Yes" : "No")
                  << setw(15) << (spots[i]->isOccupied() ? spots[i]->getVehicleNumber() : "-")
                  << setw(10) << (spots[i]->isOccupied() ? spots[i]->getVehicleCategory() : "-")
                  << setw(10) << (spots[i]->isOccupied() ? to_string(spots[i]->getAllottedHours()) : "-")
                 << endl;
        }
    }

    void viewSummary() const {
        int total = static_cast<int>(spots.size());
        int occupied = 0;

        for (size_t i = 0; i < spots.size(); ++i) {
            if (spots[i]->isOccupied()) {
                occupied++;
            }
        }

        int free = total - occupied;
        double percent = (total == 0) ? 0.0 : (occupied * 100.0 / total);

        cout << "\n========== SUMMARY ==========" << endl;
        cout << "Total Spots      : " << total << endl;
        cout << "Occupied Spots   : " << occupied << endl;
        cout << "Free Spots       : " << free << endl;
        cout << "Occupancy Percent: " << fixed << setprecision(2) << percent << "%" << endl;
    }

    void viewTransactionLog() const {
        ifstream inFile(logFileName.c_str());
        if (!inFile) {
            cout << "No log file found yet." << endl;
            return;
        }

        cout << "\n========== TRANSACTION LOG ==========" << endl;
        string line;
        bool hasData = false;

        while (getline(inFile, line)) {
            hasData = true;
            cout << line << endl;
        }

        if (!hasData) {
            cout << "Log is empty." << endl;
        }
    }
};

#endif
