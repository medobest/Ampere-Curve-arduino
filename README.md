# ⚡ Ampere Curve Grapher (Arduino)

This project displays and graphs the **current (A)** and **voltage (V)** in real time using an Arduino, a **3.5" TFT display**, and an **ACS712 current sensor**.  
It can be used for monitoring power consumption, testing circuits, and visualizing load behavior.  

![Demo Screenshot](/images/Demo.jpg)


A real-time current and voltage grapher based on Arduino, designed for visualizing electrical load behavior in a live, intuitive way.

Overview

The Ampere Curve Grapher is a lightweight, embedded measurement system that displays and graphs electric current (A) and voltage (V) in real time using an Arduino microcontroller, a 3.5″ TFT screen, and an ACS712 current sensor. It’s especially useful for monitoring power consumption, analyzing circuit dynamics, and visualizing how current changes over time under varying loads.

Key Features

Real-Time Monitoring: Instantaneous measurement of current and voltage, updated continuously on the TFT display.

Graphical Plotting: A dynamically updating curve plots current (amperes) over time, allowing you to see how the load is behaving moment-to-moment.

Digital Readouts: In addition to the graph, the system shows numerical values of current (A) and voltage (V) for precise readings.

Auto-Scaling Y-Axis: The graph’s vertical scale automatically adjusts to the range of measured values, optimizing visualization without manual adjustments.

Session Logging: You can record a “session” of measurements and review historical data (current curve) on-screen.

Simple, Clean UI: A user interface with clear labels, a graph area, and navigational buttons ensures the experience is intuitive.

Control Buttons:

Voltage Button: Toggle to display the voltage reading only, useful for focusing on voltage independently.

Session Log Mode: Switch from real-time view to a stored session graph to inspect data history.

Reset Button: Clear both live data and saved session log to begin a fresh measurement run.

Hardware Components

Microcontroller: Arduino UNO, Arduino MEGA, or any compatible board.

Display: 3.5″ TFT LCD (MCUFRIEND or ILI9486 driver).

Current Sensor: ACS712 (supported versions: 5 A, 20 A, 30 A).

Voltage Sensing: A voltage divider circuit to safely scale down high voltages for measurement by the Arduino.

Other: Jumper wires, optionally a breadboard or custom PCB.

Software / Libraries Required

Adafruit GFX — for basic graphics primitives.

MCUFRIEND_kbv — to drive the 3.5″ TFT display.

ACS712 Current Sensor Library — to interface with the current sensor and convert analog readings to amperes.

How It Works

The ACS712 sensor produces an analog voltage proportional to current passing through it.

The Arduino reads that analog value, converts it to a current reading using calibration, and simultaneously measures voltage via the voltage divider.

The firmware plots these measurements over time on the TFT as a curve, while also updating digital readouts.

When you hit the “Session Log” button, the system switches to log mode, replaying the recorded curve so you can analyze how current has evolved during that recording session.

Hitting “Reset” clears existing data, letting you start fresh.

Use Cases / Applications

Power Consumption Monitoring: Track how much current a device draws over time under different operating conditions.

Circuit Testing: Analyze transient behavior and load response when switching loads on/off or changing load magnitude.

Educational Tool: Demonstrate to students or hobbyists how current varies, how a sensor like ACS712 works, and how to plot data in embedded systems.

Prototyping & Debugging: Use in hardware development to validate current draw and verify that designs stay within safe electrical limits.

Calibration & Accuracy

The system supports calibration of the ACS712 sensor to improve measurement accuracy (by adjusting offset and sensitivity values).

Voltage divider must be designed carefully: resistor values and division ratio directly affect measured accuracy, especially at higher voltages.

Sampling rate and filtering: The code includes a smoothing or filtering mechanism (if implemented) to reduce noise in readings, improving visual and numerical stability.

Limitations / Considerations

Sensor Limits: The ACS712 has a limited current range depending on the model (5A, 20A, 30A). Exceeding this may damage the sensor or distort readings.

Voltage Range: The voltage measurement is limited by the divider design and Arduino’s analog input voltage range (0–5V for many boards).

Memory Constraints: Depending on the Arduino model and how you store session data, very long sessions may run out of RAM.

Display Refresh Rate: The TFT screen and MCU speed impose practical limits on how fast the graph can update.

Noise & Accuracy: Without filtering, analog readings may be noisy. Proper calibration and smoothing help, but there are inherent analog-to-digital conversion limitations.

Future Enhancements

Here are some possible directions to expand or improve the project:

Logging to External Storage: Save session data to an SD card or external memory for later analysis.

Exporting Data: Enable CSV export via serial, USB, or over Wi-Fi / Bluetooth.

Higher Resolution Graphing: Use more sampling, oversampling, or temporal interpolation to get smoother curves.

Multiple Channels: Support multiple current sensors to graph several loads simultaneously.

Wireless Monitoring: Integrate a Wi-Fi / Bluetooth module (e.g., ESP8266 / ESP32) to stream measurements to a PC or mobile app.

Alert System: Add threshold-based alerts (e.g., when current exceeds a safety value).

Improved UI / UX: Add menus, touch support (if TFT is touch-enabled), or more visual themes.

Calibration Wizard: Provide a built-in calibration routine that tests sensor offset and gain automatically.

Bill of Materials (BOM)
Item	Description
Arduino Board	UNO, MEGA, or compatible
TFT Display	3.5″ MCUFRIEND / ILI9486
Current Sensor	ACS712 (5A / 20A / 30A)
Resistors	For the voltage divider (values depend on input voltage)
Wires	Jumper cables or PCB traces
Power Supply	Appropriate to your load and Arduino
