#!/usr/bin/env python3
"""
Smart Parking Lot Management System - PyQt5 GUI
Professional user interface with dashboard, vehicle entry/exit, lot status, and transactions
"""

import sys
from datetime import datetime
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QStackedWidget, QWidget, QVBoxLayout, QHBoxLayout,
    QLabel, QPushButton, QLineEdit, QSpinBox, QComboBox, QTableWidget, QTableWidgetItem,
    QGroupBox, QGridLayout, QProgressBar, QMessageBox, QTabWidget
)
from PyQt5.QtCore import Qt, QTimer, QSize
from PyQt5.QtGui import QFont, QColor, QIcon
from PyQt5.QtCore import pyqtSignal, QObject


class ParkingLotManager(QObject):
    """Core parking lot management logic"""
    
    def __init__(self):
        super().__init__()
        self.spots = {
            'standard': {'total': 15, 'occupied': 8},
            'disabled': {'total': 8, 'occupied': 3},
            'vip': {'total': 7, 'occupied': 2}
        }
        self.transactions = []
        self.parked_vehicles = {}
        self.total_revenue = 0
    
    def get_occupancy(self):
        total = sum(s['total'] for s in self.spots.values())
        occupied = sum(s['occupied'] for s in self.spots.values())
        return occupied, total
    
    def get_available_spot(self, spot_type):
        if self.spots[spot_type]['occupied'] < self.spots[spot_type]['total']:
            return True
        return False
    
    def park_vehicle(self, spot_type, vehicle_no, category, hours):
        """Park a vehicle"""
        if not self.get_available_spot(spot_type):
            return False, "No spots available for this type"
        
        occupied, total = self.get_occupancy()
        if occupied >= total:
            return False, "Parking lot is full"
        
        spot_id = occupied + 1
        self.parked_vehicles[spot_id] = {
            'vehicle': vehicle_no,
            'category': category,
            'spot_type': spot_type,
            'hours': hours,
            'entry_time': datetime.now()
        }
        
        self.spots[spot_type]['occupied'] += 1
        self.transactions.append({
            'timestamp': datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
            'type': 'Entry',
            'spot_id': spot_id,
            'vehicle': vehicle_no,
            'category': category,
            'duration': hours,
            'amount': 0
        })
        
        return True, f"Successfully parked at Spot {spot_id}"
    
    def calculate_fee(self, spot_type, hours, minutes, category):
        """Calculate parking fee"""
        rates = {'standard': 20, 'disabled': 10, 'vip': 50}
        vehicle_fees = {'2W': 20, '4W': 50}
        
        billable_hours = hours + (1 if minutes > 0 else 0)
        base_fee = rates[spot_type] * billable_hours
        vehicle_fee = vehicle_fees.get(category, 50)
        overtime = (billable_hours - hours) * 10 if minutes > 0 else 0
        
        total = base_fee + vehicle_fee + overtime
        return {
            'base': base_fee,
            'vehicle': vehicle_fee,
            'overtime': overtime,
            'total': total
        }
    
    def exit_vehicle(self, spot_id, hours, minutes, paid_amount, vehicle_no):
        """Process vehicle exit"""
        if spot_id not in self.parked_vehicles:
            return False, f"Spot {spot_id} not found or empty"
        
        vehicle = self.parked_vehicles[spot_id]
        fees = self.calculate_fee(vehicle['spot_type'], hours, minutes, vehicle['category'])
        
        if paid_amount < fees['total']:
            return False, f"Insufficient payment. Need ₹{fees['total']:.2f}, Got ₹{paid_amount:.2f}"
        
        change = paid_amount - fees['total']
        self.total_revenue += fees['total']
        
        self.spots[vehicle['spot_type']]['occupied'] -= 1
        del self.parked_vehicles[spot_id]
        
        self.transactions.append({
            'timestamp': datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
            'type': 'Exit',
            'spot_id': spot_id,
            'vehicle': vehicle_no,
            'category': vehicle['category'],
            'duration': f"{hours}h {minutes}m",
            'amount': fees['total']
        })
        
        return True, fees, change


class MainWindow(QMainWindow):
    """Main application window"""
    
    def __init__(self):
        super().__init__()
        self.lot_manager = ParkingLotManager()
        self.init_ui()
    
    def init_ui(self):
        """Initialize UI"""
        self.setWindowTitle("Smart Parking Lot Management System")
        self.setGeometry(100, 100, 1200, 800)
        
        # Create stacked widget
        self.stacked_widget = QStackedWidget()
        
        # Create screens
        self.dashboard_widget = DashboardScreen(self.lot_manager)
        self.entry_widget = VehicleEntryScreen(self.lot_manager)
        self.exit_widget = VehicleExitScreen(self.lot_manager)
        self.status_widget = LotStatusScreen(self.lot_manager)
        self.transaction_widget = TransactionScreen(self.lot_manager)
        
        # Add screens to stacked widget
        self.stacked_widget.addWidget(self.dashboard_widget)      # 0
        self.stacked_widget.addWidget(self.entry_widget)          # 1
        self.stacked_widget.addWidget(self.exit_widget)           # 2
        self.stacked_widget.addWidget(self.status_widget)         # 3
        self.stacked_widget.addWidget(self.transaction_widget)    # 4
        
        # Create toolbar
        toolbar_widget = QWidget()
        toolbar_layout = QHBoxLayout(toolbar_widget)
        toolbar_layout.setContentsMargins(0, 0, 0, 0)
        
        self.dashboard_btn = QPushButton("📊 Dashboard")
        self.dashboard_btn.clicked.connect(self.show_dashboard)
        
        self.park_btn = QPushButton("🚗 Park Vehicle")
        self.park_btn.clicked.connect(self.show_park_vehicle)
        
        self.exit_btn = QPushButton("🚙 Vehicle Exit")
        self.exit_btn.clicked.connect(self.show_vehicle_exit)
        
        self.status_btn = QPushButton("📍 Lot Status")
        self.status_btn.clicked.connect(self.show_lot_status)
        
        self.transaction_btn = QPushButton("📋 Transactions")
        self.transaction_btn.clicked.connect(self.show_transactions)
        
        self.clock_label = QLabel()
        self.clock_label.setMinimumWidth(150)
        
        # Style toolbar
        for btn in [self.dashboard_btn, self.park_btn, self.exit_btn, self.status_btn, self.transaction_btn]:
            btn.setMinimumHeight(40)
            btn.setMinimumWidth(150)
            btn.setFont(QFont('Arial', 11, QFont.Bold))
        
        toolbar_layout.addWidget(self.dashboard_btn)
        toolbar_layout.addWidget(self.park_btn)
        toolbar_layout.addWidget(self.exit_btn)
        toolbar_layout.addWidget(self.status_btn)
        toolbar_layout.addWidget(self.transaction_btn)
        toolbar_layout.addStretch()
        toolbar_layout.addWidget(self.clock_label)
        
        toolbar_widget.setStyleSheet("""
            QWidget { background-color: #2c3e50; }
            QPushButton { 
                background-color: #3498db; color: white; border: none; 
                border-radius: 5px; padding: 5px; 
            }
            QPushButton:hover { background-color: #2980b9; }
            QPushButton:pressed { background-color: #1f618d; }
            QLabel { color: white; font-weight: bold; }
        """)
        
        # Create main layout
        main_widget = QWidget()
        main_layout = QVBoxLayout(main_widget)
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)
        main_layout.addWidget(toolbar_widget)
        main_layout.addWidget(self.stacked_widget)
        
        self.setCentralWidget(main_widget)
        
        # Setup style
        self.setStyleSheet("""
            QMainWindow { background-color: #f0f2f5; }
            QLabel { color: #2c3e50; }
        """)
        
        # Update clock
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_clock)
        self.timer.start(1000)
        self.update_clock()
        
        # Show dashboard on startup
        self.show_dashboard()
    
    def update_clock(self):
        """Update clock display"""
        now = datetime.now()
        self.clock_label.setText(now.strftime("%H:%M:%S\n%A"))
    
    def show_dashboard(self):
        self.stacked_widget.setCurrentIndex(0)
        self.dashboard_widget.refresh()
    
    def show_park_vehicle(self):
        self.stacked_widget.setCurrentIndex(1)
        self.entry_widget.reset_form()
    
    def show_vehicle_exit(self):
        self.stacked_widget.setCurrentIndex(2)
        self.exit_widget.reset_form()
    
    def show_lot_status(self):
        self.stacked_widget.setCurrentIndex(3)
        self.status_widget.refresh()
    
    def show_transactions(self):
        self.stacked_widget.setCurrentIndex(4)
        self.transaction_widget.refresh()


class DashboardScreen(QWidget):
    """Dashboard screen showing statistics"""
    
    def __init__(self, lot_manager):
        super().__init__()
        self.lot_manager = lot_manager
        self.init_ui()
    
    def init_ui(self):
        layout = QVBoxLayout(self)
        layout.setSpacing(20)
        layout.setContentsMargins(20, 20, 20, 20)
        
        # Title
        title = QLabel("Dashboard")
        title.setFont(QFont('Arial', 28, QFont.Bold))
        title.setStyleSheet("color: #2c3e50;")
        layout.addWidget(title)
        
        # Overall Statistics
        overall_group = QGroupBox("Overall Statistics")
        overall_layout = QGridLayout(overall_group)
        overall_layout.setSpacing(20)
        
        self.total_label = QLabel("30")
        self.total_label.setFont(QFont('Arial', 18, QFont.Bold))
        self.total_label.setStyleSheet("color: #3498db;")
        
        self.occupied_label = QLabel("13")
        self.occupied_label.setFont(QFont('Arial', 18, QFont.Bold))
        self.occupied_label.setStyleSheet("color: #e74c3c;")
        
        self.available_label = QLabel("17")
        self.available_label.setFont(QFont('Arial', 18, QFont.Bold))
        self.available_label.setStyleSheet("color: #27ae60;")
        
        self.rate_label = QLabel("43%")
        self.rate_label.setFont(QFont('Arial', 18, QFont.Bold))
        self.rate_label.setStyleSheet("color: #3498db;")
        
        overall_layout.addWidget(QLabel("Total Spots:"), 0, 0)
        overall_layout.addWidget(self.total_label, 0, 1)
        overall_layout.addWidget(QLabel("Occupied:"), 0, 2)
        overall_layout.addWidget(self.occupied_label, 0, 3)
        overall_layout.addWidget(QLabel("Available:"), 0, 4)
        overall_layout.addWidget(self.available_label, 0, 5)
        
        overall_layout.addWidget(QLabel("Occupancy Rate:"), 1, 0)
        self.occupancy_bar = QProgressBar()
        self.occupancy_bar.setValue(43)
        self.occupancy_bar.setMaximumHeight(25)
        overall_layout.addWidget(self.occupancy_bar, 1, 1, 1, 4)
        overall_layout.addWidget(self.rate_label, 1, 5)
        
        overall_group.setStyleSheet(self.get_group_style("#3498db"))
        layout.addWidget(overall_group)
        
        # Spot Types
        types_group = QGroupBox("Parking Spot Types")
        types_layout = QGridLayout(types_group)
        
        self.standard_label = QLabel("15")
        self.standard_label.setFont(QFont('Arial', 16, QFont.Bold))
        self.standard_label.setStyleSheet("color: #27ae60;")
        
        self.disabled_label = QLabel("8")
        self.disabled_label.setFont(QFont('Arial', 16, QFont.Bold))
        self.disabled_label.setStyleSheet("color: #f39c12;")
        
        self.vip_label = QLabel("7")
        self.vip_label.setFont(QFont('Arial', 16, QFont.Bold))
        self.vip_label.setStyleSheet("color: #9b59b6;")
        
        types_layout.addWidget(QLabel("🚗 Standard:"), 0, 0)
        types_layout.addWidget(self.standard_label, 0, 1)
        types_layout.addWidget(QLabel("♿ Disabled:"), 0, 2)
        types_layout.addWidget(self.disabled_label, 0, 3)
        types_layout.addWidget(QLabel("⭐ VIP:"), 0, 4)
        types_layout.addWidget(self.vip_label, 0, 5)
        
        types_group.setStyleSheet(self.get_group_style("#27ae60"))
        layout.addWidget(types_group)
        
        # Activity
        activity_group = QGroupBox("Activity Statistics")
        activity_layout = QGridLayout(activity_group)
        
        self.parkings_label = QLabel("245")
        self.parkings_label.setFont(QFont('Arial', 18, QFont.Bold))
        self.parkings_label.setStyleSheet("color: #2980b9;")
        
        self.exits_label = QLabel("227")
        self.exits_label.setFont(QFont('Arial', 18, QFont.Bold))
        self.exits_label.setStyleSheet("color: #8e44ad;")
        
        self.revenue_label = QLabel("₹18,550.00")
        self.revenue_label.setFont(QFont('Arial', 18, QFont.Bold))
        self.revenue_label.setStyleSheet("color: #16a085;")
        
        activity_layout.addWidget(QLabel("Total Parkings:"), 0, 0)
        activity_layout.addWidget(self.parkings_label, 0, 1)
        activity_layout.addWidget(QLabel("Total Exits:"), 0, 2)
        activity_layout.addWidget(self.exits_label, 0, 3)
        activity_layout.addWidget(QLabel("💰 Revenue:"), 0, 4)
        activity_layout.addWidget(self.revenue_label, 0, 5)
        
        activity_group.setStyleSheet(self.get_group_style("#2980b9"))
        layout.addWidget(activity_group)
        
        layout.addStretch()
    
    def refresh(self):
        occupied, total = self.lot_manager.get_occupancy()
        self.occupied_label.setText(str(occupied))
        self.available_label.setText(str(total - occupied))
        rate = int((occupied / total) * 100) if total > 0 else 0
        self.rate_label.setText(f"{rate}%")
        self.occupancy_bar.setValue(rate)
        self.revenue_label.setText(f"₹{self.lot_manager.total_revenue:,.2f}")
    
    @staticmethod
    def get_group_style(color):
        return f"""
            QGroupBox {{
                border: 2px solid {color};
                border-radius: 8px;
                margin-top: 10px;
                padding-top: 10px;
                font-weight: bold;
                color: #2c3e50;
            }}
            QGroupBox::title {{
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 3px 0 3px;
            }}
        """


class VehicleEntryScreen(QWidget):
    """Vehicle entry screen"""
    
    def __init__(self, lot_manager):
        super().__init__()
        self.lot_manager = lot_manager
        self.init_ui()
    
    def init_ui(self):
        layout = QVBoxLayout(self)
        layout.setSpacing(20)
        layout.setContentsMargins(20, 20, 20, 20)
        
        title = QLabel("Park Vehicle")
        title.setFont(QFont('Arial', 28, QFont.Bold))
        title.setStyleSheet("color: #2c3e50;")
        layout.addWidget(title)
        
        # Form group
        form_group = QGroupBox("Vehicle Details")
        form_layout = QGridLayout(form_group)
        form_layout.setSpacing(15)
        
        form_layout.addWidget(QLabel("Spot Type:"), 0, 0)
        self.spot_combo = QComboBox()
        self.spot_combo.addItems(["🚗 Standard (₹20/hr)", "♿ Disabled (₹10/hr)", "⭐ VIP (₹50/hr)"])
        form_layout.addWidget(self.spot_combo, 0, 1)
        
        form_layout.addWidget(QLabel("Vehicle Number:"), 1, 0)
        self.vehicle_input = QLineEdit()
        self.vehicle_input.setPlaceholderText("e.g., MH-02-AB-1234")
        form_layout.addWidget(self.vehicle_input, 1, 1)
        
        form_layout.addWidget(QLabel("Category:"), 2, 0)
        self.category_combo = QComboBox()
        self.category_combo.addItems(["2W (₹20)", "4W (₹50)"])
        form_layout.addWidget(self.category_combo, 2, 1)
        
        form_layout.addWidget(QLabel("Allotted Hours:"), 3, 0)
        self.hours_spin = QSpinBox()
        self.hours_spin.setMinimum(1)
        self.hours_spin.setMaximum(24)
        self.hours_spin.setValue(2)
        form_layout.addWidget(self.hours_spin, 3, 1)
        
        form_group.setStyleSheet(self.get_group_style("#3498db"))
        layout.addWidget(form_group)
        
        # Fee preview
        preview_group = QGroupBox("Estimated Fee")
        preview_layout = QVBoxLayout(preview_group)
        self.fee_label = QLabel("₹0.00")
        self.fee_label.setFont(QFont('Arial', 32, QFont.Bold))
        self.fee_label.setStyleSheet("color: #27ae60; text-align: center;")
        self.fee_label.setAlignment(Qt.AlignCenter)
        preview_layout.addWidget(self.fee_label)
        preview_group.setStyleSheet(self.get_group_style("#27ae60"))
        layout.addWidget(preview_group)
        
        # Buttons
        button_layout = QHBoxLayout()
        self.park_btn = QPushButton("✓ Park Vehicle")
        self.park_btn.setMinimumHeight(45)
        self.park_btn.setMinimumWidth(200)
        self.park_btn.setStyleSheet(self.get_button_style("#27ae60"))
        self.park_btn.clicked.connect(self.park_vehicle)
        
        self.clear_btn = QPushButton("↻ Clear")
        self.clear_btn.setMinimumHeight(45)
        self.clear_btn.setMinimumWidth(150)
        self.clear_btn.setStyleSheet(self.get_button_style("#95a5a6"))
        self.clear_btn.clicked.connect(self.reset_form)
        
        button_layout.addWidget(self.park_btn)
        button_layout.addWidget(self.clear_btn)
        button_layout.addStretch()
        layout.addLayout(button_layout)
        
        self.status_label = QLabel()
        self.status_label.setWordWrap(True)
        layout.addWidget(self.status_label)
        
        layout.addStretch()
    
    def park_vehicle(self):
        vehicle_no = self.vehicle_input.text().strip()
        if not vehicle_no:
            QMessageBox.critical(self, "Error", "Please enter vehicle number")
            return
        
        spot_type = ['standard', 'disabled', 'vip'][self.spot_combo.currentIndex()]
        category = ['2W', '4W'][self.category_combo.currentIndex()]
        hours = self.hours_spin.value()
        
        success, message = self.lot_manager.park_vehicle(spot_type, vehicle_no, category, hours)
        
        if success:
            QMessageBox.information(self, "Success", message)
            self.reset_form()
        else:
            QMessageBox.critical(self, "Error", message)
    
    def reset_form(self):
        self.vehicle_input.clear()
        self.spot_combo.setCurrentIndex(0)
        self.category_combo.setCurrentIndex(0)
        self.hours_spin.setValue(2)
        self.fee_label.setText("₹0.00")
        self.status_label.clear()
    
    @staticmethod
    def get_group_style(color):
        return f"""
            QGroupBox {{
                border: 2px solid {color};
                border-radius: 8px;
                margin-top: 10px;
                padding-top: 10px;
                font-weight: bold;
                color: #2c3e50;
            }}
            QGroupBox::title {{
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 3px 0 3px;
            }}
        """
    
    @staticmethod
    def get_button_style(color):
        return f"""
            QPushButton {{
                background-color: {color};
                color: white;
                border: none;
                border-radius: 5px;
                font-weight: bold;
                font-size: 14px;
                padding: 10px;
            }}
            QPushButton:hover {{ background-color: rgba(0,0,0,0.2); }}
            QPushButton:pressed {{ background-color: rgba(0,0,0,0.3); }}
        """


class VehicleExitScreen(QWidget):
    """Vehicle exit and billing screen"""
    
    def __init__(self, lot_manager):
        super().__init__()
        self.lot_manager = lot_manager
        self.init_ui()
    
    def init_ui(self):
        layout = QVBoxLayout(self)
        layout.setSpacing(20)
        layout.setContentsMargins(20, 20, 20, 20)
        
        title = QLabel("Vehicle Exit & Billing")
        title.setFont(QFont('Arial', 28, QFont.Bold))
        title.setStyleSheet("color: #2c3e50;")
        layout.addWidget(title)
        
        # Exit form
        form_group = QGroupBox("Exit Details")
        form_layout = QGridLayout(form_group)
        form_layout.setSpacing(15)
        
        form_layout.addWidget(QLabel("Spot ID:"), 0, 0)
        self.spot_id_input = QLineEdit()
        form_layout.addWidget(self.spot_id_input, 0, 1)
        
        form_layout.addWidget(QLabel("Actual Hours:"), 1, 0)
        self.actual_hours = QSpinBox()
        self.actual_hours.setMinimum(0)
        self.actual_hours.setMaximum(24)
        form_layout.addWidget(self.actual_hours, 1, 1)
        
        form_layout.addWidget(QLabel("Extra Minutes:"), 1, 2)
        self.extra_minutes = QSpinBox()
        self.extra_minutes.setMinimum(0)
        self.extra_minutes.setMaximum(59)
        form_layout.addWidget(self.extra_minutes, 1, 3)
        
        form_group.setStyleSheet(self.get_group_style("#3498db"))
        layout.addWidget(form_group)
        
        # Billing
        billing_group = QGroupBox("Billing Details")
        billing_layout = QVBoxLayout(billing_group)
        self.billing_label = QLabel("Click 'Calculate Fee' to view details")
        self.billing_label.setStyleSheet("color: #7f8c8d; font-style: italic;")
        billing_layout.addWidget(self.billing_label)
        
        self.fee_display = QLabel("₹0.00")
        self.fee_display.setFont(QFont('Arial', 32, QFont.Bold))
        self.fee_display.setStyleSheet("color: #e74c3c; text-align: center;")
        self.fee_display.setAlignment(Qt.AlignCenter)
        billing_layout.addWidget(self.fee_display)
        billing_group.setStyleSheet(self.get_group_style("#e74c3c"))
        layout.addWidget(billing_group)
        
        # Payment
        payment_group = QGroupBox("Payment")
        payment_layout = QHBoxLayout(payment_group)
        payment_layout.addWidget(QLabel("Amount Paid:"))
        self.paid_input = QLineEdit()
        self.paid_input.setPlaceholderText("0.00")
        payment_layout.addWidget(self.paid_input)
        payment_group.setStyleSheet(self.get_group_style("#27ae60"))
        layout.addWidget(payment_group)
        
        # Buttons
        button_layout = QHBoxLayout()
        self.calc_btn = QPushButton("🧮 Calculate Fee")
        self.calc_btn.setMinimumHeight(45)
        self.calc_btn.clicked.connect(self.calculate_fee)
        
        self.exit_btn = QPushButton("✓ Process Exit")
        self.exit_btn.setMinimumHeight(45)
        self.exit_btn.clicked.connect(self.process_exit)
        
        button_layout.addWidget(self.calc_btn)
        button_layout.addWidget(self.exit_btn)
        button_layout.addStretch()
        layout.addLayout(button_layout)
        
        layout.addStretch()
    
    def calculate_fee(self):
        try:
            hours = self.actual_hours.value()
            minutes = self.extra_minutes.value()
            
            if hours == 0 and minutes == 0:
                QMessageBox.critical(self, "Error", "Duration must be > 0")
                return
            
            # Sample calculation
            fees = self.lot_manager.calculate_fee('standard', hours, minutes, '4W')
            self.fee_display.setText(f"₹{fees['total']:.2f}")
            
            details = f"Base: ₹{fees['base']:.2f} | Vehicle: ₹{fees['vehicle']:.2f} | Overtime: ₹{fees['overtime']:.2f}"
            self.billing_label.setText(details)
            self.billing_label.setStyleSheet("color: #2c3e50;")
        except Exception as e:
            QMessageBox.critical(self, "Error", str(e))
    
    def process_exit(self):
        try:
            spot_id = int(self.spot_id_input.text())
            hours = self.actual_hours.value()
            minutes = self.extra_minutes.value()
            paid = float(self.paid_input.text() or "0")
            
            fee = float(self.fee_display.text().replace("₹", ""))
            
            if paid < fee:
                QMessageBox.critical(self, "Error", f"Need ₹{fee:.2f}, Got ₹{paid:.2f}")
                return
            
            change = paid - fee
            QMessageBox.information(self, "Receipt",
                f"Exit Successful!\nFee: ₹{fee:.2f}\nChange: ₹{change:.2f}")
            self.reset_form()
        except ValueError:
            QMessageBox.critical(self, "Error", "Invalid input")
    
    def reset_form(self):
        self.spot_id_input.clear()
        self.actual_hours.setValue(0)
        self.extra_minutes.setValue(0)
        self.paid_input.clear()
        self.fee_display.setText("₹0.00")
        self.billing_label.setText("Click 'Calculate Fee' to view details")
    
    @staticmethod
    def get_group_style(color):
        return f"""
            QGroupBox {{
                border: 2px solid {color};
                border-radius: 8px;
                margin-top: 10px;
                padding-top: 10px;
                font-weight: bold;
                color: #2c3e50;
            }}
            QGroupBox::title {{
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 3px 0 3px;
            }}
        """


class LotStatusScreen(QWidget):
    """Lot status with parking spots table"""
    
    def __init__(self, lot_manager):
        super().__init__()
        self.lot_manager = lot_manager
        self.init_ui()
    
    def init_ui(self):
        layout = QVBoxLayout(self)
        layout.setSpacing(20)
        layout.setContentsMargins(20, 20, 20, 20)
        
        title = QLabel("Parking Lot Status")
        title.setFont(QFont('Arial', 28, QFont.Bold))
        title.setStyleSheet("color: #2c3e50;")
        layout.addWidget(title)
        
        # Table
        table_group = QGroupBox("Detailed Spot Status")
        table_layout = QVBoxLayout(table_group)
        
        self.table = QTableWidget()
        self.table.setColumnCount(6)
        self.table.setHorizontalHeaderLabels(["Spot ID", "Type", "Status", "Vehicle", "Category", "Hours"])
        self.table.setRowCount(7)
        self.table.setStyleSheet("""
            QTableWidget {
                border: 2px solid #bdc3c7;
                background-color: white;
            }
            QHeaderView::section {
                background-color: #2c3e50;
                color: white;
                padding: 5px;
                border: none;
            }
            QTableWidget::item {
                padding: 5px;
                border-bottom: 1px solid #ecf0f1;
            }
            QTableWidget::item:selected {
                background-color: #3498db;
                color: white;
            }
        """)
        
        table_layout.addWidget(self.table)
        table_group.setStyleSheet("""
            QGroupBox {
                border: 2px solid #3498db;
                border-radius: 8px;
                margin-top: 10px;
                padding-top: 10px;
                font-weight: bold;
                color: #2c3e50;
            }
        """)
        layout.addWidget(table_group)
        
        layout.addStretch()
        self.refresh()
    
    def refresh(self):
        sample_data = [
            (1, "🚗 Standard", "🟢 Available", "-", "-", "-"),
            (2, "🚗 Standard", "🔴 Occupied", "MH-02-AB-1234", "4W", "2"),
            (3, "🚗 Standard", "🟢 Available", "-", "-", "-"),
            (4, "♿ Disabled", "🔴 Occupied", "KA-03-CD-9876", "4W", "3"),
            (5, "⭐ VIP", "🟢 Available", "-", "-", "-"),
            (6, "🚗 Standard", "🔴 Occupied", "DL-01-XY-5678", "2W", "1"),
            (7, "⭐ VIP", "🔴 Occupied", "TN-04-EF-2345", "4W", "2"),
        ]
        
        for i, (spot_id, type_, status, vehicle, category, hours) in enumerate(sample_data):
            items = [
                QTableWidgetItem(str(spot_id)),
                QTableWidgetItem(type_),
                QTableWidgetItem(status),
                QTableWidgetItem(vehicle),
                QTableWidgetItem(category),
                QTableWidgetItem(hours)
            ]
            
            for j, item in enumerate(items):
                item.setFont(QFont('Arial', 10))
                if "🔴" in status:
                    item.setBackground(QColor("#ffebee"))
                else:
                    item.setBackground(QColor("#e8f5e9"))
                self.table.setItem(i, j, item)


class TransactionScreen(QWidget):
    """Transaction history screen"""
    
    def __init__(self, lot_manager):
        super().__init__()
        self.lot_manager = lot_manager
        self.init_ui()
    
    def init_ui(self):
        layout = QVBoxLayout(self)
        layout.setSpacing(20)
        layout.setContentsMargins(20, 20, 20, 20)
        
        title = QLabel("Transaction Log")
        title.setFont(QFont('Arial', 28, QFont.Bold))
        title.setStyleSheet("color: #2c3e50;")
        layout.addWidget(title)
        
        # Summary
        summary_group = QGroupBox("Summary")
        summary_layout = QVBoxLayout(summary_group)
        self.summary_label = QLabel("Total: 25 | Revenue: ₹18,550.00 | Average: ₹742.00")
        summary_layout.addWidget(self.summary_label)
        summary_group.setStyleSheet("""
            QGroupBox {
                border: 2px solid #27ae60;
                border-radius: 8px;
                margin-top: 10px;
                padding-top: 10px;
                font-weight: bold;
                color: #2c3e50;
            }
        """)
        layout.addWidget(summary_group)
        
        # Table
        table_group = QGroupBox("All Transactions")
        table_layout = QVBoxLayout(table_group)
        
        self.table = QTableWidget()
        self.table.setColumnCount(7)
        self.table.setHorizontalHeaderLabels(["Timestamp", "Type", "Spot", "Vehicle", "Category", "Duration", "Amount"])
        self.table.setRowCount(8)
        self.table.setStyleSheet("""
            QTableWidget {
                border: 2px solid #bdc3c7;
                background-color: white;
            }
            QHeaderView::section {
                background-color: #2c3e50;
                color: white;
                padding: 5px;
                border: none;
            }
        """)
        
        table_layout.addWidget(self.table)
        table_group.setStyleSheet("""
            QGroupBox {
                border: 2px solid #3498db;
                border-radius: 8px;
                margin-top: 10px;
                padding-top: 10px;
                font-weight: bold;
                color: #2c3e50;
            }
        """)
        layout.addWidget(table_group)
        
        layout.addStretch()
        self.refresh()
    
    def refresh(self):
        transactions = [
            ("2024-04-04 10:45", "🟢 Entry", "1", "MH-02-AB-1234", "4W", "2h", "-"),
            ("2024-04-04 10:30", "🔴 Exit", "5", "KA-03-CD-9876", "4W", "3h", "₹120"),
            ("2024-04-04 10:15", "🟢 Entry", "3", "DL-01-XY-5678", "2W", "1h", "-"),
            ("2024-04-04 09:50", "🔴 Exit", "7", "TN-04-EF-2345", "4W", "5h", "₹250"),
            ("2024-04-04 09:20", "🟢 Entry", "2", "AP-05-GH-6789", "4W", "4h", "-"),
            ("2024-04-04 08:45", "🔴 Exit", "4", "HR-06-IJ-1011", "2W", "2h", "₹60"),
            ("2024-04-04 08:10", "🟢 Entry", "6", "PB-07-KL-1213", "4W", "3h", "-"),
            ("2024-04-04 07:55", "🔴 Exit", "2", "MH-08-MN-1415", "4W", "4h", "₹180"),
        ]
        
        for i, (ts, type_, spot, vehicle, category, duration, amount) in enumerate(transactions):
            items = [
                QTableWidgetItem(ts),
                QTableWidgetItem(type_),
                QTableWidgetItem(spot),
                QTableWidgetItem(vehicle),
                QTableWidgetItem(category),
                QTableWidgetItem(duration),
                QTableWidgetItem(amount)
            ]
            
            for j, item in enumerate(items):
                item.setFont(QFont('Arial', 10))
                if "Exit" in type_:
                    item.setBackground(QColor("#ffebee"))
                else:
                    item.setBackground(QColor("#e8f5e9"))
                self.table.setItem(i, j, item)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
