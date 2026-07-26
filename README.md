# 🚗 BrillyPark — Smart IoT Parking System

[![ESP32](https://img.shields.io/badge/Hardware-ESP32-blue?style=for-the-badge&logo=espressif)](https://www.espressif.com/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)

> An IoT-powered real-time parking space monitoring system built with **ESP32**, **C++**, and **ultrasonic sensors**. Designed for efficient parking management and real-time occupancy tracking.

---

## ⚡ Key Features

* 📡 **Real-Time Space Tracking:** Uses IR sensors to detect vehicle presence in real-time.
* ⚡ **Low Power & Fast Execution:** Native C++ firmware optimized for ESP32 microcontrollers.
* 📊 **Live Status Updates:** Transmits parking occupancy status dynamically across configured nodes.
* 🚦 **Visual Indicators:** LED/Display integration for instant local status feedback.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Description |
| :--- | :---: | :--- |
| **ESP32 NodeMCU** | 1 | Main microcontroller board |
| **IR Sensor** | 2+ | Infrared detection module |
| **LED Indicators** | 2+ | Red/Green status indicator lights |
| **Breadboard & Wires** | — | Prototyping connection cables |

---

## 🔌 Circuit & Pinout Setup

```text
  [ ESP32 ] ─────── (Trig / Echo) ─────── [ HC-SR04 Sensor ]
     │
     └─── GPIO 2  ───> [ Green LED ] (Space Available)
     └─── GPIO 4  ───> [ Red LED ]   (Space Occupied)
