# Requirements Coverage Matrix (Console C++)

This document maps the problem requirements to implemented code.

## 1) Parking Spot Class Hierarchy (OOP)

- Abstract base class with pure virtual functions:
  - `ParkingSpot` in `desktop-app/include/ParkingSpot.h`
  - Pure virtual methods:
    - `virtual double calculateFee(int hours) const = 0;`
    - `virtual string getSpotTypeDescription() const = 0;`

- Derived classes overriding virtual behavior:
  - `StandardSpot`, `DisabledSpot`, `VIPSpot` in `desktop-app/include/DerivedSpots.h`
  - Each overrides `calculateFee(...)` and `getSpotTypeDescription()`.

## 2) Polymorphic Array + STL + Initialization

- Polymorphic container:
  - `vector<ParkingSpot*> spots;` in `desktop-app/include/ParkingLot.h`

- Dynamic mixed initialization:
  - `new StandardSpot(...)`, `new DisabledSpot(...)`, `new VIPSpot(...)` in `ParkingLot` constructor (`desktop-app/include/ParkingLot.h`).

## 3) Simulation / Interaction

- Menu-driven console interface:
  - Main menu with Park/Exit/View Status/View Summary/View Log in `desktop-app/src/main.cpp`.

- Park vehicle (first available of selected type):
  - `ParkingLot::parkVehicle(...)` loops through `spots` and picks first free spot matching requested type.

- Exit by Spot ID with type-specific fee via polymorphism:
  - `ParkingLot::exitVehicle(...)` and `ParkingLot::exitVehicleWithPayment(...)`.
  - Core polymorphic call:
    - `spot->calculateFee(...)` inside billing flow (`desktop-app/include/ParkingLot.h`).

## 4) Reporting

- Full status report:
  - `ParkingLot::viewLotStatus()` lists spot ID, type, occupancy, vehicle, category, allotted hours.

- Summary report:
  - `ParkingLot::viewSummary()` prints total, occupied, free, occupancy percentage.

## 5) Dynamic Memory Management

- Allocation:
  - Uses `new` for each derived `ParkingSpot` object during lot initialization.

- Cleanup:
  - `ParkingLot` destructor iterates through polymorphic vector and calls `delete` for each pointer.

## 6) Example Execution Deliverable

- Real scripted VIP exit transcript generated at:
  - `tests/Example_Execution.txt`

This transcript demonstrates:

- Parking into a VIP spot
- Exiting by spot ID
- Type-specific VIP fee calculation through polymorphic behavior
- Receipt and occupancy flow
