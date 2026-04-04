# Smart Parking Lot Management System - GUI

A modern, professional, and user-friendly GUI application for managing parking lots built with Qt Framework (C++).

## Features

### 📊 Dashboard

- **Real-time Statistics**: View total, occupied, and available parking spots
- **Occupancy Rate**: Visual progress bar showing parking lot occupancy percentage
- **Spot Distribution**: Breakdown of Standard, Disabled, and VIP spots
- **Activity Metrics**: Total parkings, exits, and total revenue

### 🚗 Vehicle Entry Management

- **Spot Type Selection**: Choose from Standard (₹20/hr), Disabled (₹10/hr), or VIP (₹50/hr) spots
- **Vehicle Registration**: Enter vehicle number with validation
- **Category Selection**: Support for 2W (Two-wheeler ₹20) and 4W (Four-wheeler ₹50)
- **Time Allocation**: Set allotted parking hours (1-24 hours)
- **Fee Preview**: Real-time estimated fee display
- **Input Validation**: Comprehensive form validation with error messages

### 🚙 Vehicle Exit & Billing

- **Spot Information Lookup**: Retrieve parked vehicle details by spot ID
- **Duration Tracking**: Record actual hours and extra minutes used
- **Fee Calculation**: Automatic billing with overtime charges
- **Detailed Billing**: View itemized charges breakdown
- **Payment Processing**: Handle payment amount with automatic change calculation
- **Receipt Generation**: Professional receipt with all transaction details

### 📍 Parking Lot Status

- **Real-time Spot Status**: View all 30 spots with current occupancy
- **Detailed Table View**:
  - Spot ID
  - Spot Type (Standard/Disabled/VIP)
  - Status (Occupied/Available)
  - Vehicle Number (if occupied)
  - Vehicle Category
  - Allotted Hours
- **Color-coded Display**: Visual indicators for available/occupied spots
- **Type Legend**: Quick reference for spot types

### 📋 Transaction Log

- **Comprehensive History**: All entry and exit transactions
- **Filter Options**: View by transaction type (All/Entry/Exit)
- **Summary Statistics**:
  - Total transactions
  - Total revenue
  - Average transaction value
  - Peak hours
- **Export Functionality**: Export transactions to CSV format
- **Detailed Records**: Timestamp, type, spot ID, vehicle info, duration, amount

### 🎨 User Interface Features

- **Modern Design**: Clean, professional interface with intuitive layout
- **Color-coded System**:
  - Blue: Information & Primary actions
  - Green: Success & Available spots
  - Red: Exit & Occupied spots
  - Orange: Disabled spots
  - Purple: VIP spots
- **Responsive Layout**: Dynamic resizing and optimal spacing
- **Live Clock**: Current time and date display in toolbar
- **Navigation Menu**: Top menu bar for all main features
- **Quick Access Toolbar**: One-click access to all major features
- **Status Bar**: Contextual status information

## Architecture

### Project Structure

```
ParkingGUI.pro           # Qt Project File
├── main_gui.cpp         # Application Entry Point
├── mainwindow.h/cpp    # Main Window Container
├── dashboardwidget.h/cpp        # Dashboard Screen
├── vehicleentrywidget.h/cpp     # Vehicle Entry Screen
├── vehicleexitwidget.h/cpp      # Vehicle Exit Screen
├── lotstatuswidget.h/cpp        # Parking Lot Status Screen
├── transactionwidget.h/cpp      # Transaction Log Screen
├── ParkingLot.h         # Core Parking Logic
├── ParkingSpot.h        # Spot Definition
├── DerivedSpots.h       # Spot Types
└── resources.qrc        # Resource File
```

### Design Patterns

- **Model-View Pattern**: Separation of UI and business logic
- **Stacked Widget Pattern**: Multiple screens managed in single window
- **Signal/Slot Pattern**: Qt's event handling mechanism
- **Utility Functions**: Input validation and formatting helpers

## Setup & Installation

### Prerequisites

- Qt 5.12 or higher (with Qt Creator)
- C++17 or later
- CMake or qmake

### Building the Application

#### Using Qt Creator

1. Open `ParkingGUI.pro` in Qt Creator
2. Configure project with desired Qt version
3. Build: `Build > Build All` or `Ctrl+B`
4. Run: `Build > Run` or `Ctrl+R`

#### Using Command Line

```bash
# Using qmake
qmake ParkingGUI.pro
make

# Using cmake (if CMakeLists.txt is provided)
cmake .
make
```

### Running the Application

```bash
# After building
./ParkingGUI (Linux/macOS)
# or
ParkingGUI.exe (Windows)
```

## Usage Guide

### Dashboard

1. Click "Dashboard" in toolbar or menu
2. View real-time parking statistics
3. Monitor occupancy percentage
4. Check activity metrics

### Parking a Vehicle

1. Click "Park Vehicle" or use toolbar
2. Select parking spot type (Standard/Disabled/VIP)
3. Enter vehicle registration number
4. Select vehicle category (2W/4W)
5. Set allotted hours
6. Review estimated fee
7. Click "Park Vehicle" button
8. Confirm successful parking with receipt

### Exiting with Payment

1. Click "Vehicle Exit" or use toolbar
2. Enter parking spot ID
3. Enter actual hours and extra minutes used
4. Click "Calculate Fee" to view billing
5. Enter payment amount
6. Click "Process Exit & Pay"
7. Print/save receipt

### Checking Parking Lot Status

1. Click "Lot Status" in toolbar or menu
2. View all 30 parking spots
3. See vehicle details for occupied spots
4. Use legend to understand spot types
5. Identify available spots for new vehicles

### Viewing Transactions

1. Click "Transactions" in toolbar or menu
2. Apply filters (All/Entry/Exit)
3. View summary statistics
4. Click on any transaction for details
5. Export to CSV using "Export" button

## UI/UX Considerations

### Input Validation

- ✓ Empty field detection
- ✓ Format validation (registration numbers)
- ✓ Range validation (hours, minutes, amount)
- ✓ Type checking (numeric vs text)
- ✓ Clear error messages

### Visual Feedback

- ✓ Color-coded status indicators
- ✓ Progress bars for occupancy
- ✓ Message boxes for confirmations
- ✓ Status updates after operations
- ✓ Disabled state for invalid actions

### Accessibility

- ✓ Clear labeling of all inputs
- ✓ Keyboard-friendly navigation
- ✓ Large font sizes
- ✓ High contrast color scheme
- ✓ Emoji icons for visual clarity

### Error Handling

- ✓ User-friendly error messages
- ✓ Specific error descriptions
- ✓ Suggestions for correction
- ✓ Graceful degradation
- ✓ Exception handling

## Possible Enhancements

1. **Database Integration**
   - SQLite for persistent storage
   - Real transaction history
   - Backup and recovery

2. **Advanced Features**
   - Pre-booking system
   - Monthly passes
   - Floating parking
   - Queue management

3. **Reporting**
   - Peak hour analysis
   - Revenue reports
   - Utilization trends
   - Custom date ranges

4. **Integration**
   - Payment gateway (PayPal, Stripe)
   - SMS/Email notifications
   - License plate recognition
   - Mobile app companion

5. **Security**
   - User authentication
   - Role-based access control
   - Audit logging
   - Encrypted transactions

6. **Internationalization**
   - Multi-language support
   - Regional date/time formats
   - Currency conversion

## Styling

### Color Palette

- **Primary Blue**: #3498db (Information, Primary Actions)
- **Success Green**: #27ae60 (Available, Success Operations)
- **Danger Red**: #e74c3c (Occupied, Critical Actions)
- **Warning Orange**: #f39c12 (Disabled Spots)
- **Premium Purple**: #9b59b6 (VIP Spots)
- **Dark Theme**: #2c3e50 (Headers, Main Text)

### Typography

- **Title**: 28pt Bold, #2c3e50
- **Labels**: 12pt Bold, #2c3e50
- **Values**: 14-18pt Bold (varies by context)
- **Descriptions**: 12-13pt Regular

### Spacing

- **Margins**: 20px standard
- **Gaps**: 15-20px between elements
- **Padding**: 8-10px for inputs
- **Border Radius**: 5-8px for modern look

## Testing

### Test Cases

- ✓ Valid vehicle entry and exit
- ✓ Invalid input handling
- ✓ Fee calculation accuracy
- ✓ Change calculation
- ✓ Status updates
- ✓ Filter functionality
- ✓ Export functionality

### Edge Cases

- ✓ Spot ID not found
- ✓ Insufficient payment
- ✓ Zero duration parking
- ✓ Max capacity reached
- ✓ Invalid time format
- ✓ Duplicate vehicle number

## Troubleshooting

### Build Issues

- Ensure Qt is properly installed
- Verify Qt version compatibility
- Check compiler version (C++17)
- Use clean build if issues persist

### Runtime Issues

- Verify file paths are correct
- Check parking log file permissions
- Ensure sufficient disk space
- Review error messages carefully

## Future Development

This GUI is designed to be scalable and maintainable. Future versions can include:

- Cloud synchronization
- Mobile app integration
- Advanced analytics dashboard
- IoT sensor integration
- AI-powered space optimization

## License

This project is part of an OOPS (Object-Oriented Programming System) educational initiative.

---

**Version**: 1.0
**Last Updated**: April 2024
**Framework**: Qt 5.12+
**Language**: C++17
