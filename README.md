# Embedded-Autonomous-RC-Car-System

## 🚀 Overview
This repository documents the step-by-step evolution of a bare-metal embedded system built around the Texas Instruments MSP430 microcontroller. Over the course of 10 stages, this project evolved from a basic power supply circuit into a fully autonomous, Wi-Fi-enabled rover capable of network-based waypoint navigation, closed-loop black line following, and remote gamepad control. 

**Final Demo Score: 110/105 (First-Try Completion Bonus Achieved)**

## 🛠 Hardware Stack
* **Microcontroller:** TI MSP430 
* **Networking:** ESP8266 Wi-Fi Module
* **Sensors:** Dual IR Emitter/Detector pairs (Line Tracking), Thumbwheel (ADC calibration)
* **Actuators:** Dual DC Motors driven by a custom H-Bridge circuit
* **Peripherals:** Onboard LCD Display, Custom PCB

## 💻 Software Stack
* **Language:** C (Bare-metal MCU), Python (Remote Host)
* **Protocols:** UART, Wi-Fi (AT Commands), TCP/IP
* **Internal MCU Systems:** PWM Motor Control, Hardware Interrupts, Timer Modules, 12-bit ADC, Software Debouncing
* **Toolchain:** Code Composer Studio (CCS)

## 📈 The Evolution (Projects 1 - 10)
This repository is structured to show the iterative design process, where each project builds directly upon the hardware and software of the last:

* **Project 1-2 (Power & Display):** Bootstrapping the battery power system and initializing the LCD display via SPI.
* **Project 3-5 (H-Bridge & PWM):** Developing the motor control logic. Progressed from basic forward-only movement to full PWM-driven directional control (Forward, Reverse, Pivot) utilizing hardware timers.
* **Project 6-7 (ADC & Line Tracking):** Integrating the IR sensors. Implemented continuous ADC polling and logic to intercept, align with, and track a circular black line autonomously. 
* **Project 8-9 (Serial & IoT):** Establishing UART communication. Programmed the MSP430 to initialize an ESP8266 module, connect to an enterprise Wi-Fi network, and act as a TCP server.
* **Project 10 (The Final Course):** Integrated all subsystems. The rover successfully navigated a busy, multi-node Wi-Fi waypoint course before transitioning seamlessly into autonomous sensor-driven line tracking. 

## 🎮 Bonus: "Rocket League" Mode
As a post-demo hardware challenge, I developed a Python host script (`xbox_drive.py`) that acts as a real-time bridge between a Bluetooth Xbox controller and the rover's ESP8266 module. 

Using the `pygame` library and network sockets, the script reads analog stick and trigger data, translates it into strict serial command strings, and fires them over the local network to the MSP430's open parser for zero-latency, RC-style control.

## 🤝 Acknowledgements
* **NC State University:** Developed as part of ECE 306 (Introduction to Embedded Systems).

## 👨‍💻 About the Author
**Ezana**
* B.S. Electrical and Computer Engineering @ NC State University (Class of 2028)

