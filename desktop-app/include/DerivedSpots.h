#ifndef DERIVEDSPOTS_H
#define DERIVEDSPOTS_H

#include "ParkingSpot.h"

using namespace std;

class StandardSpot : public ParkingSpot {
public:
    StandardSpot(int id) : ParkingSpot(id) {
        cout << "StandardSpot constructor called for spot " << spotId << endl;
    }

    ~StandardSpot() override {
        cout << "StandardSpot destructor called for spot " << spotId << endl;
    }

    double calculateFee(int hours) const override {
        return 20.0 * hours;
    }

    string getSpotTypeDescription() const override {
        return "Standard";
    }
};

class DisabledSpot : public ParkingSpot {
public:
    DisabledSpot(int id) : ParkingSpot(id) {
        cout << "DisabledSpot constructor called for spot " << spotId << endl;
    }

    ~DisabledSpot() override {
        cout << "DisabledSpot destructor called for spot " << spotId << endl;
    }

    double calculateFee(int hours) const override {
        return 10.0 * hours;
    }

    string getSpotTypeDescription() const override {
        return "Disabled";
    }
};

class VIPSpot : public ParkingSpot {
public:
    VIPSpot(int id) : ParkingSpot(id) {
        cout << "VIPSpot constructor called for spot " << spotId << endl;
    }

    ~VIPSpot() override {
        cout << "VIPSpot destructor called for spot " << spotId << endl;
    }

    double calculateFee(int hours) const override {
        return 100.0 + (50.0 * hours);
    }

    string getSpotTypeDescription() const override {
        return "VIP";
    }
};

#endif
