# ⚡ Ampere Curve Grapher (Arduino)

This project displays and graphs the **current (A)** and **voltage (V)** in real time using an Arduino, a **3.5" TFT display**, and an **ACS712 current sensor**.  
It can be used for monitoring power consumption, testing circuits, and visualizing load behavior.  

![Demo Screenshot](/images/Demo.jpg)

## 📌 Overview

**Ampere Curve Grapher** is an Arduino-powered measurement tool that displays and graphs electrical current (A) and voltage (V) in real time. It provides a clean UI, dynamic graphing, and session logging—ideal for testing circuits, analyzing load changes, and monitoring power consumption.

---

## ⭐ Features

* **Live Amp & Volt Measurement**
  Continuously measures current (ACS712) and voltage via a divider.

* **Real-Time Curve Graph**
  Plots current values over time with smooth, automatic visualization.

* **Auto Y-Scaling**
  Graph height adjusts automatically depending on the range of measured values.

* **Digital Values Display**
  Shows exact readings of voltage and current alongside the graph.

* **Session Logging Mode**
  Records a measurement run and lets you review the entire amp curve after capturing it.

* **Dedicated Controls on TFT**

  * **Voltage Mode**: Show voltage reading alone
  * **Log Mode**: Switch to session history view
  * **Reset**: Clear graph + log and start fresh

* **Simple & Clean UI**
  Optimized layout with curve area, labels, buttons, and real-time refresh.

---

## 🧩 Hardware Requirements

* Arduino UNO / MEGA / compatible board
* 3.5″ TFT LCD (MCUFRIEND / ILI9486 supported)
* ACS712 current sensor (5A / 20A / 30A versions)
* Voltage divider resistors
* Wires / breadboard or PCB

---

## 📚 Software Requirements

Install these Arduino libraries:

* `Adafruit_GFX`
* `MCUFRIEND_kbv`
* `ACS712` library
* Arduino IDE (latest recommended)

---

## 🔧 How It Works

1. The ACS712 outputs an analog voltage proportional to current.
2. Arduino reads this + voltage readings from the divider.
3. Values are converted into amps/volts.
4. The graph is drawn continuously while showing digital readings.
5. The session log records data points, which can be replayed later on-screen.
6. Buttons allow toggling displays and clearing data.

---

## 📈 Applications

* Load testing for electronics
* Power consumption analysis
* Educational demonstrations
* Prototype debugging
* Visualizing current fluctuations
* Monitoring device behavior over time

---

## 🎛 Calibration

* **Current**: Adjust ACS712 offset & sensitivity in code
* **Voltage**: Set correct voltage divider ratio
* Perform calibration with a known multimeter reading for accuracy

---

## ⚠ Notes & Limitations

* ACS712 has limited ranges (5A/20A/30A depending on module)
* Accuracy improves with filtering and proper sensor calibration
* Very long logs may consume Arduino RAM
* Graph update speed is limited by MCU + TFT refresh rate

---

## 🧱 Schematic (Conceptual)

* ACS712 → Arduino analog pin
* Voltage Divider → Arduino analog pin
* TFT (parallel interface) → Arduino digital pins
* Power supply → Sensor + load (rated accordingly)

*---------------------------------------------------------*

---

## 🔮 Future Enhancements

* SD card logging (CSV)
* Multi-channel current tracking
* Wi-Fi or Bluetooth live streaming
* Threshold alerts (overcurrent alarms)
* Touch UI upgrades
* Built-in calibration wizard
* Smoother graph interpolation

---

## 📦 Project Structure

```
/Ampere-Curve-arduino
│
├── SerbiousVoltAmpFINREFINED.ino   # Main firmware
├── /libraries                      # Required libraries (optional copy)
└── README.md                       # Documentation
```

---

## 🤝 Contributions

Pull requests and feature suggestions are welcome.

---
