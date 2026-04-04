#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <cctype>

#include "ParkingLot.h"

using namespace std;

int readIntInput(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin, input);

    if (input.empty()) {
        throw invalid_argument("Input cannot be empty.");
    }

    size_t processed = 0;
    int value = stoi(input, &processed);

    if (processed != input.size()) {
        throw invalid_argument("Please enter only a number (no commas or text).");
    }

    return value;
}

double readDoubleInput(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin, input);

    if (input.empty()) {
        throw invalid_argument("Input cannot be empty.");
    }

    size_t processed = 0;
    double value = stod(input, &processed);

    if (processed != input.size()) {
        throw invalid_argument("Please enter a valid amount.");
    }

    return value;
}

string readTextInput(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin, input);

    if (input.empty()) {
        throw invalid_argument("Input cannot be empty.");
    }

    return input;
}

string toUpperText(const string& value) {
    string result = value;
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = static_cast<char>(toupper(static_cast<unsigned char>(result[i])));
    }
    return result;
}

int main() {
    ParkingLot lot;

    while (true) {
        try {
            cout << "\n===== SMART PARKING LOT MENU =====" << endl;
            cout << "1. Park Vehicle" << endl;
            cout << "2. Exit Vehicle" << endl;
            cout << "3. View Lot Status" << endl;
            cout << "4. View Summary" << endl;
            cout << "5. View Transaction Log" << endl;
            cout << "6. Exit" << endl;
            int choice = readIntInput("Enter choice: ");

            if (choice == 1) {
                int typeChoice = readIntInput("Select Spot Type (1-Standard, 2-Disabled, 3-VIP): ");
                string vehicleNo = readTextInput("Enter Vehicle Number: ");
                string vehicleCategory = toUpperText(readTextInput("Enter Vehicle Category (2W/4W): "));
                int allottedHours = readIntInput("Enter Allotted Hours: ");

                lot.parkVehicle(typeChoice, vehicleNo, vehicleCategory, allottedHours);
            } else if (choice == 2) {
                int spotId = readIntInput("Enter Spot ID: ");
                int actualHours = readIntInput("Enter Actual Hours Used: ");
                int extraMinutes = readIntInput("Enter Extra Minutes (0-59): ");
                double totalFee = lot.getExitFeePreview(spotId, actualHours, extraMinutes);
                cout << "Total Fee = " << fixed << setprecision(2) << totalFee << endl;

                double paidAmount = readDoubleInput("Enter Paid Amount: ");
                ParkingLot::Receipt receipt = lot.exitVehicleWithPayment(spotId, actualHours, extraMinutes, paidAmount);

                cout << "\n========== RECEIPT ==========" << endl;
                cout << "Time          : " << receipt.timestamp << endl;
                cout << "Spot ID       : " << receipt.spotId << endl;
                cout << "Vehicle       : " << receipt.vehicleNumber << endl;
                cout << "Category      : " << receipt.vehicleCategory << endl;
                cout << "Allowed Time  : " << receipt.allottedHours << " hours" << endl;
                cout << "Used Time     : " << receipt.actualHours << "h " << receipt.actualExtraMinutes << "m" << endl;
                cout << "Overtime      : " << receipt.overtimeMinutes << " minutes" << endl;
                cout << "" << endl;
                cout << "Base Charge   : " << fixed << setprecision(2) << receipt.vehicleInitialCharge << endl;
                cout << "Overtime Fine : " << fixed << setprecision(2) << receipt.overtimeFine << endl;
                cout << "Total Fee     : " << fixed << setprecision(2) << receipt.totalFee << endl;
                cout << "Paid Amount   : " << fixed << setprecision(2) << receipt.paidAmount << endl;
                cout << "Change        : " << fixed << setprecision(2) << receipt.change << endl;
                cout << "Payment successful. Vehicle exited." << endl;
            } else if (choice == 3) {
                lot.viewLotStatus();
            } else if (choice == 4) {
                lot.viewSummary();
            } else if (choice == 5) {
                lot.viewTransactionLog();
            } else if (choice == 6) {
                cout << "Exiting program..." << endl;
                break;
            } else {
                throw invalid_argument("Invalid menu choice. Enter 1 to 6.");
            }
        }
        catch (const invalid_argument& ex) {
            cout << "Input Error: " << ex.what() << endl;
        }
        catch (const runtime_error& ex) {
            cout << "Operation Error: " << ex.what() << endl;
        }
    }

    return 0;
}
