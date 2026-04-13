# Problem Description: Smart Parking Lot Management System

## Context

Modern urban environments face continuous challenges with parking space management, including congestion, driver delay, and inefficient space utilization. A smart parking lot simulation helps evaluate allocation and occupancy behavior while reinforcing core C++ OOP concepts.

## Objective

Design and implement a C++ console application that manages a mixed parking lot with:

- Standard spots
- Disabled spots
- VIP spots

The solution must use polymorphism with a common base class pointer container (e.g., `std::vector<ParkingSpot*>`) and demonstrate inheritance, runtime dispatch, and dynamic memory management.

## Scope of Console Automation

1. Spot management (initialize and track mixed spot types)
2. Allocation and de-allocation (park and exit vehicle simulation)
3. Polymorphic interaction (type-specific fee behavior through base pointer calls)
4. Reporting (lot status and occupancy summary)

## Required Functional Behavior

1. Implement abstract base class `ParkingSpot` with pure virtual method(s)
2. Implement derived classes `StandardSpot`, `DisabledSpot`, `VIPSpot`
3. Use `std::vector<ParkingSpot*>` for polymorphic storage
4. Initialize the lot by dynamically allocating mixed spot objects
5. Park vehicle in first available spot for selected type
6. Exit vehicle by Spot ID and compute fee through polymorphic call
7. Provide status report and occupancy summary

## Technical Requirements

- OOP: encapsulation, inheritance, constructors/destructors
- Polymorphism: base pointer/reference + pure virtual function(s)
- Dynamic memory management: explicit `new`/`delete` (or smart pointers)
- STL usage: `std::vector`
- Menu-driven console interface

## Deliverables

1. Source code (`.cpp`, `.h`)
2. Problem description document (this file)
3. Example execution showing VIP exit and type-specific fee behavior
