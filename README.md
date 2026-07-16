# ⚡ IoT-Based Smart Prepaid Energy Meter with Dual-Source Switching & Overcurrent Protection

An advanced Smart Grid and Hybrid Prepaid Energy Metering System integrated with IoT automation to monitor real-time power consumption, enable secure cloud-based recharge, handle automated dual-source load switching, and enforce protective system safeguards.

## 📊 System Architecture & Schematics
The system flow and physical wiring schematics are fully documented below:

### 1. Functional System Block Diagram
![Block Diagram](Images/Block%20Diagram.png)

### 2. Physical Circuit Connection Diagram
![Connection Diagram](Images/Connection%20Diagram.png)

## 🌟 Core Features & Functional Logic
- **Cloud-Enabled Prepaid System:** Tracks dynamic balance in Taka (configured at 7 Tk/Unit). Automatically triggers a total power cut-off via high-voltage relay units the moment the balance hits zero.
- **Intelligent Dual-Source Auto Switching:** Continuously polls a dedicated DC/Solar sense line (`D7`). Automatically shifts the load from AC Main Supply (220V) to Backup Solar/DC Source (12V Battery) seamlessly during outages or zero-balance status.
- **Non-Blocking Power Telemetry:** Executes true RMS non-blocking current sampling using the **ACS712 sensor** mapped to analog node `A0` via a custom voltage divider circuit.
- **Live Local & Cloud Sync:** Streams real-time Wattage, Current, Source Type, and remaining Taka Balance concurrently to an on-board **16x2 I2C LCD screen** and the **Blynk IoT Cloud Dashboard**.

## 🛠️ Hardware Interfacing Profile (NodeMCU ESP8266)

| Sub-Block | Target Component | Component Pin | NodeMCU Node | Operational Logic |
| :--- | :--- | :---: | :---: | :--- |
| **Control & UI** | I2C LCD Display (1602) | SDA / SCL | `D2` / `D1` | Local status telemetry matrix |
| **Power Switching**| 2-Channel Relay Module | IN1 (AC) / IN2 (DC)| `D3` / `D6` | Active LOW switching loop |
| **Telemetry Node** | ACS712 Current Sensor | OUT (Analog) | `A0` | Linked via 10k/20k divider array |
| **Source Sensor**  | Voltage Divider Circuit| Solar/DC Sense | `D7` | Monitored via absolute digital read |

## 🌐 Blynk IoT Virtual Pin Mapping
The cloud framework is configured under Template ID `TMPL6_CLaR165` using the following data streams:
- **`V0` (Input):** Remote Credit Balance Recharge Channel.
- **`V1` (Output):** Live Credit Balance Stream (Tk).
- **`V2` (Output):** Real-Time Calculated Power (Watts).
- **`V3` (Output):** Real-Time Measured Load Current (Amps).
- **`V4` (Output):** Current Active Power Source Status (`AC MAIN` / `SOLAR` / `CUT-OFF`).

## 📁 Repository Directory Structure
- `Code/` : Production-ready firmware configuration file (`sketch_jun11a.ino`).
- `Docs/` : Full theoretical system documentation, parameters, and structural design blueprints.
- `Images/` : System architecture block diagrams and clean operational schematics.
- `Results/` : High-resolution physical hardware prototyping snapshots and performance metrics.

## 🚀 Engineering Future Scopes
- **Hardware Anti-Tampering System:** Integration of physical limit switches or specialized IR sensor fields to trigger hard lockouts and send automated theft alerts to the utility network upon illegal case opening.
- **True RMS AC Voltage Monitoring:** Implementation of a **ZMPT101B Voltage Sensor** layer to move away from fixed 220V software assumptions and prevent hardware damage from overvoltage/undervoltage fluctuations.
- **Time-of-Use (ToU) Tariff Billing:** Addition of a hardware **DS3231 Real-Time Clock (RTC)** to support variable algorithmic tariff calculation structures between peak and off-peak utility periods.
- **Predictive AI Load Forecasting:** Training edge machine learning regression models to evaluate household consumption parameters and forecast balance depletion lifecycles dynamically.
