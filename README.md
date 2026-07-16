# ⚡ IoT-Based Smart Prepaid Energy Meter with Dual-Source Switching & Overcurrent Protection

An advanced Smart Grid and Prepaid Energy Metering System integrated with IoT and automation to monitor real-time power consumption, enable secure remote recharge, handle dual-source load switching, and protect household appliances from overcurrent damages.

## 📊 System Architecture & Diagrams


### 1. Functional Block Diagram
![Block Diagram](Images/Block%20Diagram.png)

### 2. Complete Circuit Connection Diagram
![Connection Diagram](Images/Connection%20Diagram.png)

## 🌟 Core Features & Modules
- **Prepaid & Remote Cloud Recharge:** Tracks live balance and unit consumption, triggering automatic power cut-offs when credit hits zero. Users can recharge instantly via the cloud network.
- **Dual-Source Intelligent Switching:** Automates seamless transfer between two distinct power sources (e.g., Grid Power and Solar/Generator Backup) using relay modules based on availability or tariff structures.
- **Smart Overcurrent Protection:** Continuously monitors current thresholds; triggers an instant circuit cut-off and emergency buzzer when overcurrent/overload conditions are detected.
- **Cloud Analytics & Display:** Streams real-time current, voltage, wattage, and remaining credit directly to an LCD display on-board and synchronizes metrics to the IoT cloud app.

## 🛠️ Hardware & Components Used
- **Microcontroller:** NodeMCU ESP8266 / ESP32 / Arduino Mega
- **Sensing Arrays:** ACS712 Current Sensor, ZMPT101B Voltage Sensor (or PZEM-004T Module)
- **Actuators & Switches:** SPDT Relay Modules for power line isolation and source switching
- **User Interface:** 16x2 I2C LCD Display, Emergency Warning Buzzer

## 📁 Repository Structure
- `Code/` : Complete firmware code (`sketch_jun11a.ino`) optimized for embedded controllers.
- `Docs/` : Academic reports, functional block diagrams, and system analysis papers.
- `Images/` : Hardware connection schematics and system design block diagrams.
- `Results/` : Real-time hardware prototyping snapshots and implementation images.

## 🚀 Future Scopes
1. **Net Metering for Solar Grids:** Implementing bi-directional energy tracking to allow users to sell excess solar power back to the main grid.
2. **AI-Driven Load Forecasting:** Using machine learning algorithms to analyze historical consumption patterns and predict monthly energy costs for users.
