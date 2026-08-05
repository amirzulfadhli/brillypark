# 🚗 BrillyPark — Smart IoT Parking System

##  Intro
BrillyPark is a real-time smart parking space monitoring system designed for commercial and urban spaces. Utilizing microcontroller-level infrared sensing, it detects vehicle occupancy in real time, processes data with low power consumption, and updates driver interface displays instantaneously.

##  Technologies I Used
- **Languages:** Native C++ (Arduino Framework / PlatformIO)
- **Hardware:** ESP32 NodeMCU, Ultrasonic / IR Sensors, OLED Display, LED Status Indicators
- **Protocols:** I2C, Wi-Fi, MQTT

##  Features
- **Real-Time Space Tracking:** Sub-second detection of vehicle presence across multiple monitored slots.
- **Low-Power C++ Firmware:** Native firmware optimized for minimal latency and high ESP32 memory efficiency.
- **Dynamic Visual Feedback:** On-site OLED visual displays and local LED indicators for instant driver feedback.

##b The Process
1. **Circuit Architecture:** Designed the breadboard layout and pin mapping connecting IR sensors and display units to the ESP32.
2. **Firmware Architecture:** Wrote asynchronous C++ routines using non-blocking delays (`millis()`) to continuously poll sensors while rendering updates.
3. **State Management:** Created a centralized occupancy manager to calculate available spaces and format status strings.
4. **Hardware Verification:** Stress-tested sensor distance thresholds under varied lighting and physical conditions.

##  What I Learned
- Learned non-blocking embedded programming techniques to avoid processor stalls during display updates.
- Understood signal bouncing and debouncing logic needed for reliable infrared sensor readings.
- Mastered hardware component allocation and GPIO pin management on microcontrollers.

##  How It Could Be Improved
- **Cloud Dashboard:** Add a WebSockets or Firebase web dashboard for remote slot booking and historical occupancy trends.
- **License Plate Recognition:** Integrate an ESP32-CAM module to track specific registered vehicles.
- **Solar Power:** Optimize firmware for ultra-low-power deep sleep modes powered by small solar modules.

##  Running the Project

### Prerequisites
- [PlatformIO](https://platformio.org/) installed in VS Code (or Arduino IDE).

