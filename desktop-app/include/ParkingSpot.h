#ifndef PARKINGSPOT_H
#define PARKINGSPOT_H

#include <iostream>
#include <string>

using namespace std;

class ParkingSpot {
protected:
    int spotId;
    bool occupied;
    string vehicleNumber;
    string vehicleCategory;
    int allottedHours;

public:
    ParkingSpot(int id) : spotId(id), occupied(false), vehicleNumber(""), vehicleCategory(""), allottedHours(0) {
        cout << "ParkingSpot constructor called for spot " << spotId << endl;
    }

    virtual ~ParkingSpot() {
        cout << "ParkingSpot destructor called for spot " << spotId << endl;
    }

    int getSpotId() const {
        return spotId;
    }

    bool isOccupied() const {
        return occupied;
    }

    string getVehicleNumber() const {
        return vehicleNumber;
    }

    string getVehicleCategory() const {
        return vehicleCategory;
    }

    int getAllottedHours() const {
        return allottedHours;
    }

    void parkVehicle(const string& vehicleNo, const string& category, int allowedHours) {
        occupied = true;
        vehicleNumber = vehicleNo;
        vehicleCategory = category;
        allottedHours = allowedHours;
    }

    void removeVehicle() {
        occupied = false;
        vehicleNumber = "";
        vehicleCategory = "";
        allottedHours = 0;
    }

    virtual double calculateFee(int hours) const = 0;
    virtual string getSpotTypeDescription() const = 0;
};

#endif
