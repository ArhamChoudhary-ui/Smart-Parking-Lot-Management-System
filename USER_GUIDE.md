# User Guide - Smart Parking Lot Management System

## Table of Contents

1. [Getting Started](#getting-started)
2. [Dashboard Overview](#dashboard-overview)
3. [Parking a Vehicle](#parking-a-vehicle)
4. [Processing Vehicle Exit](#processing-vehicle-exit)
5. [Checking Lot Status](#checking-lot-status)
6. [Viewing Transactions](#viewing-transactions)
7. [Tips & Tricks](#tips--tricks)
8. [FAQs](#faqs)
9. [Troubleshooting](#troubleshooting)

---

## Getting Started

### Launching the Application

1. Double-click the `ParkingGUI` application icon
2. The main window opens to the Dashboard
3. You're ready to manage parking!

### Main Interface Components

**Toolbar (Top Navigation)**

- 📊 **Dashboard**: View overview statistics
- 🚗 **Park Vehicle**: Add new vehicle to parking lot
- 🚙 **Vehicle Exit**: Process vehicle departure and billing
- 📍 **Lot Status**: Check real-time parking spot availability
- 📋 **Transactions**: View transaction history

**Menu Bar**

- **File**: Exit application
- **View**: Navigate to different screens
- **Operations**: Quick access to parking operations
- **Help**: About and support information

**Status Bar (Bottom)**

- Current status and operations feedback
- Application ready state indicator

---

## Dashboard Overview

### What You'll See

The Dashboard provides a quick overview of your parking lot's current state:

| Metric              | Meaning                                    |
| ------------------- | ------------------------------------------ |
| **Total Spots**     | Total parking spaces available (e.g., 30)  |
| **Occupied Spots**  | Currently parked vehicles (red)            |
| **Available Spots** | Empty spots ready for new vehicles (green) |
| **Occupancy %**     | Percentage of lot capacity in use          |

### Spot Types

- 🚗 **Standard Spots** - Regular parking, ₹20/hour
- ♿ **Disabled Spots** - Special accessibility spots, ₹10/hour
- ⭐ **VIP Spots** - Premium parking, ₹50/hour

### Activity Stats

- **Total Parkings**: Number of vehicles parked today
- **Total Exits**: Number of vehicles that left today
- **Total Revenue**: Money collected today

### How to Use

1. Click "Dashboard" button in toolbar
2. View current statistics
3. Monitor occupancy rate with progress bar
4. Check spot type distribution

---

## Parking a Vehicle

### Step-by-Step Guide

#### 1. Click "Park Vehicle" Button

- In toolbar or "Operations" menu
- Form opens for vehicle entry

#### 2. Select Parking Spot Type

```
Options:
🚗 Standard Spot (₹20/hour) - Most common
♿ Disabled Spot (₹10/hour) - Accessibility spots
⭐ VIP Spot (₹50/hour) - Premium parking
```

**Choose based on**:

- Vehicle type and needs
- Customer status (VIP or disabled)
- Availability

#### 3. Enter Vehicle Registration Number

Example formats:

- `MH-02-AB-1234` (Maharashtra)
- `DL-01-XY-5678` (Delhi)
- `KA-03-CD-9876` (Karnataka)

**Rules**:

- Format: 2 letters - 2 numbers - 2 letters - 4 numbers
- Minimum 8 characters
- Only letters, numbers, and hyphens allowed

#### 4. Select Vehicle Category

```
Options:
2W - Two Wheeler (Bike/Scooter) - ₹20 initial charge
4W - Four Wheeler (Car/SUV) - ₹50 initial charge
```

**Choose based on**:

- Actual vehicle type
- Different rates for different categories

#### 5. Set Allotted Hours

- Use spinner control (up/down arrows)
- Or type directly (1-24 hours)
- Recommended: Match expected parking duration

#### 6. Review Fee Preview

- Estimated cost displays automatically
- Calculation: (Spot Rate × Hours) + Vehicle Charge
- Example: (₹20/hr × 2 hrs) + ₹50 = ₹90

#### 7. Click "Park Vehicle" Button

- Green button submits form
- Spot ID assigned immediately
- Confirmation message appears

#### 8. Save Spot ID

⚠️ **Important**: Note the assigned Spot ID

- Typically printed with ticket
- Customer needs this to exit
- Example: "Spot 12"

### Example Scenario

```
Scenario: Park a car in standard spot for 2 hours

1. Click "Park Vehicle"
2. Select "🚗 Standard Spot (₹20/hour)"
3. Enter "MH-02-AB-1234"
4. Select "4W"
5. Set "2" hours
6. Fee Preview shows: ₹90.00
7. Click "Park Vehicle"
8. Confirmation: "Successfully parked at Spot 5"
9. Give customer Spot 5 ID
```

### Common Errors & Fixes

| Error                         | Cause                     | Fix                                 |
| ----------------------------- | ------------------------- | ----------------------------------- |
| "Please enter vehicle number" | Field left empty          | Type registration number            |
| "Invalid registration format" | Wrong format              | Use format: XX-##-XX-####           |
| "No spots available"          | Lot is full               | Check available spots in Lot Status |
| "Invalid hours"               | Hours out of range (1-24) | Enter number between 1-24           |

### Tips

- ✓ Keep registration number handy
- ✓ Verify format before entering
- ✓ Choose appropriate spot type
- ✓ Set realistic hours
- ✓ Double-check fee before confirming

---

## Processing Vehicle Exit

### Step-by-Step Guide

#### 1. Click "Vehicle Exit" Button

- In toolbar or "Operations" menu
- Exit form opens

#### 2. Enter Parking Spot ID

- From the parking ticket/receipt
- Example: "5", "12", "23"
- Customer must provide this

#### 3. Enter Actual Parking Duration

**Actual Hours**: How many full hours vehicle stayed

- Click up/down arrows to adjust
- Or type directly
- Starting value: "2" (adjust as needed)

#### 4. Enter Extra Minutes

Minutes beyond the full hours

- Range: 0-59 minutes
- Example: Parked 2 hours 30 minutes → Hours: 2, Minutes: 30
- Use up/down arrows or type

#### 5. Click "Calculate Fee" Button

- Blue button processes entry
- System retrieves parking details
- Calculates charges including overages

#### 6. Review Billing Details

Displays:

- **Base Fee**: Hours × Hourly Rate
- **Overtime Charges**: Extra time penalty
- **Vehicle Initial Fee**: Category-based charge
- **Total Amount Due**: Sum of all charges

Example:

```
Spot ID: 5
Parking Duration: 3 hours 45 minutes
─────────────────
Base Fee: ₹120.00
Overtime: ₹25.00
Vehicle Charge: ₹50.00
─────────────────
Total: ₹195.00
```

#### 7. Enter Amount Paid

- How much customer is paying
- Must be ≥ Total Fee
- Accepts decimal values

#### 8. Click "Process Exit & Pay" Button

- Green button processes payment
- Generates receipt
- Automatically calculates change

#### 9. Provide Receipt to Customer

Receipt includes:

- Total fee charged
- Amount paid
- Change due
- Transaction details
- Exit confirmation

### Example Scenario

```
Scenario: Customer exits from Spot 5

Time in: 10:00 AM, Time out: 1:45 PM = 3h 45m

1. Click "Vehicle Exit"
2. Enter Spot ID: "5"
3. Enter Actual Hours: "3"
4. Enter Extra Minutes: "45"
5. Click "Calculate Fee"
6. Review billing:
   - Spot: Standard (₹20/hr)
   - Duration: 3h 45m = 4 billable hours
   - Base Fee: ₹80.00
   - Vehicle Fee (4W): ₹50.00
   - Overtime: ₹10.00
   - Total: ₹140.00
7. Enter paid amount: "150" (cash)
8. Click "Process Exit & Pay"
9. Receipt shows: Change = ₹10.00
10. Give receipt to customer
```

### Billing Rules

**Overtime Charges**:

- Any extra minutes = charge for full hour
- 2h 5m parked = charge for 3 hours
- Encourages prompt vehicle pickup

**Vehicle Category Charges**:

- 2W (Two-wheeler): ₹20 initial
- 4W (Four-wheeler): ₹50 initial
- Added to total bill

**Spot Type Rates**:

- Standard: ₹20/hour
- Disabled: ₹10/hour
- VIP: ₹50/hour

### Error Handling

| Error                  | Solution                                          |
| ---------------------- | ------------------------------------------------- |
| "Spot ID not found"    | Verify spot number with customer                  |
| "Insufficient payment" | Customer must pay more                            |
| "Invalid duration"     | Check hour/minute ranges                          |
| "Spot is empty"        | Mistake or different spot - confirm with customer |

### Tips

- ✓ Verify spot ID before processing
- ✓ Be clear about overtime charges
- ✓ Use calculator for accuracy
- ✓ Provide clear receipt
- ✓ Count payment carefully

---

## Checking Lot Status

### Step-by-Step Guide

#### 1. Click "Lot Status" Button

- In toolbar or "View" menu
- Status screen displays

#### 2. View Overall Status

Shows:

- Total parking spots (30)
- Occupied spots (with count)
- Available spots (with count)
- Occupancy percentage

#### 3. Check Detailed Table

Displays all 30 spots:

| Column               | Information                 | Color Coding |
| -------------------- | --------------------------- | ------------ |
| **Spot ID**          | 1-30                        | -            |
| **Type**             | Standard/Disabled/VIP       | With emoji   |
| **Status**           | 🟢 Available or 🔴 Occupied | Green/Red    |
| **Vehicle Number**   | Registration if occupied    | -            |
| **Vehicle Category** | 2W or 4W                    | -            |
| **Allotted Hours**   | Hours or "-" if empty       | -            |

#### 4. Use Legend

At bottom for quick reference:

- 🟢 Available - Empty spot
- 🔴 Occupied - Parked vehicle
- 🚗 Standard - Regular parking
- ♿ Disabled - Accessibility spot
- ⭐ VIP - Premium parking

#### 5. Find Available Spots

- Look for 🟢 Available status
- Check preferred spot type
- Offer customer suitable spot

### Reading the Table

**Example Row**:

```
Spot ID | Type | Status | Vehicle | Category | Hours
   12   | 🚗   | 🔴    | MH-02-  |   4W     |  2
```

Meaning: Spot 12 is occupied with a 4W vehicle for 2 hours

**Empty Spot Example**:

```
Spot ID | Type | Status | Vehicle | Category | Hours
   15   | ♿   | 🟢    |    -    |   -      |  -
```

Meaning: Spot 15 (disabled) is available

### Usage Scenarios

**Scenario 1: Customer Wants Specific Spot Type**

1. Open Lot Status
2. Scan Type column
3. Find available spot with needed type
4. Offer that spot to customer
5. Go to "Park Vehicle" to book

**Scenario 2: Lot Getting Full**

1. Check Lot Status
2. See occupancy percentage
3. If > 80%, notify management
4. Prepare to redirect customers

**Scenario 3: Find Customer's Vehicle**

1. Customer remembers spot number
2. Check Lot Status table
3. Verify vehicle number matches
4. Confirm they're at right spot

### Tips

- ✓ Check regularly for availability
- ✓ Plan for peak hours
- ✓ Monitor occupancy trends
- ✓ Use for customer assistance
- ✓ Refresh periodically for updates

---

## Viewing Transactions

### Accessing Transactions

1. Click "Transactions" button in toolbar
2. Or "View" menu → "Transactions"
3. Transaction history page loads

### What You'll See

#### Summary Statistics (Top Section)

Shows key metrics:

- **Total Transactions**: All parking activities
- **Total Revenue**: Money collected
- **Average Transaction**: Average per vehicle
- **Peak Hours**: Busiest times

#### Filter Options

```
Dropdown: All Transactions / Entry Transactions / Exit Transactions
Button: 📥 Export to CSV
```

#### Transaction Table

Columns display:

- **Timestamp**: Date and exact time
- **Type**: 🟢 Entry or 🔴 Exit
- **Spot ID**: Which parking spot
- **Vehicle Number**: Registration
- **Category**: 2W or 4W
- **Duration**: How long parked
- **Amount**: ₹ collected (exit only)

### Example Table Data

```
Timestamp          | Type  | Spot | Vehicle      | Cat | Duration | Amount
2024-04-04 10:45   | Entry |  1   | MH-02-AB-1234| 4W  |    2     |   -
2024-04-04 10:30   | Exit  |  5   | KA-03-CD-9876| 4W  |    3     | ₹120.00
2024-04-04 10:15   | Entry |  3   | DL-01-XY-5678| 2W  |    1     |   -
```

### Filtering Transactions

**Filter Options**:

1. **All Transactions**: Shows everything
2. **Entry Transactions**: Only parking entries
3. **Exit Transactions**: Only exits with payments

**How to Filter**:

```
1. Click filter dropdown
2. Select desired category
3. Table updates automatically
```

### Exporting Data

**Export to CSV**:

1. Click "📥 Export to CSV" button
2. Select save location
3. File named: `transactions_YYYY-MM-DD.csv`
4. Open in Excel for analysis

**CSV Format**:

```
Timestamp,Type,SpotID,Vehicle,Category,Duration,Amount
2024-04-04 10:45,Entry,1,MH-02-AB-1234,4W,2,0.00
2024-04-04 10:30,Exit,5,KA-03-CD-9876,4W,3,120.00
```

### Analysis & Insights

**Questions You Can Answer**:

- Which spots are most used? (Check frequency)
- Peak parking times? (Look at timestamps)
- Revenue per hour? (Sum entry-exit pairs)
- Vehicle category distribution? (Count types)
- Average parking duration? (Calculate from data)

**Common Reports**:

1. **Daily Revenue**
   - Sum all exit amounts
   - Identify profitable hours

2. **Spot Utilization**
   - Count entries per spot
   - Find popular spots
   - Identify underused spots

3. **Peak Hours**
   - Spot entries/exits by hour
   - Plan staff accordingly
   - Alert for high demand

### Tips

- ✓ Export regularly for backup
- ✓ Review transactions daily
- ✓ Identify patterns
- ✓ Plan improvements
- ✓ Track revenue trends

---

## Tips & Tricks

### Quick Operations

- **Park Vehicle**: Click green button twice for quick entry
- **Check Status**: Keyboard shortcut (Ctrl+L)
- **Exit Processing**: Calculate fee first, then payment
- **Spot Lookup**: Use Lot Status before parking

### Efficiency Tips

1. **Pre-verify Information**
   - Check lot status before promising spot
   - Verify vehicle number format
   - Calculate fees quickly

2. **Time Management**
   - Process exits during customer peak
   - Schedule batch operations off-peak
   - Use transaction log for daily reconciliation

3. **Error Prevention**
   - Always verify spot ID
   - Double-check vehicle number format
   - Confirm fee with customer before payment
   - Print receipt for customer records

4. **Customer Service**
   - Provide clear parking instructions
   - Explain fee structure upfront
   - Handle overstays gracefully
   - Keep customer informed

### Keyboard Shortcuts (Future)

- `Ctrl+P`: Park Vehicle
- `Ctrl+E`: Exit Vehicle
- `Ctrl+L`: Lot Status
- `Ctrl+T`: Transactions
- `Tab`: Next field
- `Enter`: Submit form

---

## FAQs

### General Questions

**Q: How many spots does the lot have?**
A: The system manages 30 parking spots (15 Standard, 8 Disabled, 7 VIP).

**Q: What are the parking rates?**
A:

- Standard: ₹20/hour + ₹50 vehicle fee
- Disabled: ₹10/hour + ₹50 vehicle fee
- VIP: ₹50/hour + ₹50 vehicle fee
- Two-wheeler initial: ₹20 (instead of ₹50)

**Q: Can I change the rates?**
A: Contact system administrator to modify fee structures.

### Parking Questions

**Q: How long can I park?**
A: Any duration from 1 to 24 hours. Month-long parking available on request.

**Q: What if I exceed allotted hours?**
A: Overtime charges apply (₹10-25 extra per hour depending on spot type).

**Q: Can I reserve a parking spot?**
A: Current system supports immediate parking. Pre-booking available upon request.

**Q: Is disabled spot parking free?**
A: No, it's ₹10/hour (discounted). Valid disability certificate required.

### Exit & Billing Questions

**Q: What if I lose my spot number?**
A: Check vehicle number in Lot Status or contact operator.

**Q: How is change calculated?**
A: Total fee subtracted from amount paid. Displayed on receipt.

**Q: Can I get a refund?**
A: Vehicle must exit before. Refunds handled per location policy.

**Q: Is the receipt important?**
A: Yes, keep it as proof of payment. Required for billing disputes.

### Technical Questions

**Q: How often updates the lot status?**
A: Real-time updates. Refresh manually for latest status.

**Q: Can I export transaction history?**
A: Yes, export to CSV format from Transactions tab.

**Q: What if system crashes during payment?**
A: Safe recovery mode. Contact operator for assistance.

**Q: Is data backed up?**
A: Yes, daily automatic backup. Retention: 1 year.

---

## Troubleshooting

### Common Issues & Solutions

### Issue: "Spot ID Not Found"

**Cause**: Entered wrong spot number

**Solution**:

```
1. Ask customer for spot ticket/receipt
2. Verify number exactly
3. Cross-check in Lot Status table
4. Re-enter correctly
```

**Prevention**:

- (system provides clear spot numbers
- Customer keeps receipt
- Double-check before processing

### Issue: "Vehicle Number Format Invalid"

**Cause**: Incorrect format entered

**Valid Format Examples**:

```
✓ MH-02-AB-1234 (Correct)
✓ DL-01-XY-5678 (Correct)
✗ MH02AB1234 (Missing hyphens)
✗ MH-2-AB-1234 (Single digit in number group)
✗ Maharashtra-2-AB-1234 (Full state name)
```

**Solution**:

1. Request correct format from customer
2. Ask for actual registration certificate
3. Type carefully: `XX-##-XX-####`
4. Where X = letter, # = number

### Issue: "No Spots Available"

**Cause**: Lot is at 100% capacity

**Solution**:

```
1. Inform customer lot is full
2. Offer waiting list option
3. Suggest alternative parking nearby
4. Check Lot Status for future availability
5. Customer contacts later for spot
```

**Prevention**:

- Monitor occupancy percentage
- Alert staff at 80% capacity
- Implement dynamic pricing
- Plan for peak hours

### Issue: "Insufficient Payment"

**Cause**: Customer didn't pay enough

**Solution**:

```
1. Display total amount due
2. Show current payment
3. Calculate difference needed
4. Politely ask for additional payment
5. Process once payment is sufficient
```

**Example**:

```
Total Due: ₹150.00
Paid: ₹100.00
Difference: ₹50.00 needed
```

### Issue: "Hours Out of Range"

**Cause**: Entered hours < 1 or > 24

**Solution**:

1. For short parking: Enter 1 hour (minimum)
2. For long parking: Contact operator
3. Use minutes field for additional time
4. System shows valid range: 1-24

### Issue: "Application Won't Start"

**Cause**: Missing dependencies or corruption

**Solution**:

```
1. Close any open instances
2. Wait 30 seconds
3. Try restarting
4. Restart computer if still fails
5. Contact IT support
```

### Issue: "Table Data Doesn't Update"

**Cause**: Display not refreshed

**Solution**:

1. Click the same tab/screen again
2. Use View menu to refresh
3. Close and reopen application
4. Check if internet connection active
5. Restart if issue persists

### Issue: "Transaction Export Fails"

**Cause**: File/disk issues or permissions

**Solution**:

```
1. Check disk has free space
2. Verify write permissions
3. Try different save location
4. Export to USB or cloud drive
5. Contact IT if still fails
```

### Issue: "Receipt Not Printing"

**Cause**: Printer not connected/configured

**Solution**:

1. Check printer is powered on
2. Verify printer connected to network
3. Test print from other application
4. Check system print queue
5. Restart printer if frozen

### Getting Help

**For Technical Issues**:

- Contact IT Department
- Provide error message details
- Describe steps to reproduce

**For Operational Questions**:

- Ask your supervisor
- Check system documentation
- Contact management

**Emergency Support**:

- Direct Line: [Number]
- Email: [Email]
- On-site Support: [Availability]

---

## Conclusion

This comprehensive guide covers all aspects of using the Smart Parking Lot Management System. For additional support, refer to:

- IT Department
- System Administrator
- On-site Training Sessions
- Video Tutorials (if available)

**Remember**:

- Verify information before processing
- Double-check fees with customers
- Keep records organized
- Report issues promptly
- Provide excellent customer service!

---

**Version**: 1.0
**Last Updated**: April 2024
**For Support**: Contact System Administrator
