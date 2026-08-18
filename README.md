# Smart Parking System using Ultrasonic Sensors

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: Arduino](https://img.shields.io/badge/Platform-Arduino-00979C.svg)](https://www.arduino.cc/)
[![Status: Complete](https://img.shields.io/badge/Status-Complete-brightgreen.svg)]()

## Overview

This project presents a **Smart Parking System** designed to optimize parking space utilization and enhance user experience. It uses **HC-SR04 ultrasonic sensors** to monitor the occupancy status of individual parking slots in real time. An Arduino UNO microcontroller processes sensor data, controls visual indicators (LEDs), and provides a live status report on a 16x2 I2C LCD. An alert system (buzzer) activates when the parking lot is full.

## Problem Statement

Finding an available parking space in busy areas like malls, airports, or office complexes is a significant challenge. This leads to wasted time, increased fuel consumption, traffic congestion, and driver frustration. Traditional parking management methods are often inefficient and do not provide real-time, slot-specific information.

## Objectives

- **Detect** the presence or absence of a vehicle in each parking slot in real-time.
- **Display** the status of each slot (FREE/OCCUPIED) on an LCD screen.
- **Indicate** the total number of available slots in the parking lot.
- **Provide visual feedback** using Green (FREE) and Red (OCCUPIED) LEDs for each slot.
- **Generate an alert** (buzzer) when the parking lot is completely full.
- **Demonstrate** core embedded systems concepts, including sensor interfacing, data processing, and output control.

## Industry Relevance

Smart parking systems are crucial for modern urban mobility and smart city initiatives. They are deployed by companies like **Siemens, Bosch, and Cisco** and are widely used in:
- **Shopping Malls & Retail Centers**
- **Airports & Railway Stations**
- **Hospitals & Medical Facilities**
- **Office Buildings & Corporate Campuses**
- **Residential Societies**
- **University Campuses**

These systems provide immense business value by reducing traffic congestion, improving space utilization, enhancing customer experience, and enabling automated monitoring.

## Features

- **Real-Time Slot Detection:** Monitors up to 4 parking slots using ultrasonic sensors.
- **Intuitive Visual Indicators:** Red and Green LEDs for each slot provide immediate status.
- **Interactive LCD Display:** Shows the status of all slots and the total number of available spots.
- **Audible Full Alert:** A buzzer sounds when all slots are occupied.
- **Modular Code:** Clean, well-commented, and modular C++/Arduino code for easy understanding and modification.
- **Virtual Simulation:** Fully tested and functional on the Wokwi simulation platform, making it accessible without physical hardware.
- **Debugging Support:** Comprehensive Serial Monitor output for testing and calibration.

## Components Used

| Component | Quantity | Purpose |
| :--- | :--- | :--- |
| Arduino UNO | 1 | Microcontroller for processing and control |
| HC-SR04 Ultrasonic Sensor | 4 | Detects vehicle presence by measuring distance |
| Red LEDs | 4 | Indicates an occupied slot |
| Green LEDs | 4 | Indicates a free slot |
| 220Ω Resistors | 8 | Current limiting for LEDs |
| 16x2 I2C LCD | 1 | Displays status messages |
| Active Buzzer | 1 | Provides audible full alert |
| Breadboard | 1 | For prototyping connections |
| Jumper Wires | - | To build the circuit |

## Embedded Systems Concepts Used

- **Microcontroller (Arduino UNO):** The central processing unit.
- **GPIO (General Purpose Input/Output):** Used to interface sensors, LEDs, and the buzzer.
- **Ultrasonic Sensor:** Non-contact distance measurement.
- **Timer:** Used implicitly by `pulseIn()` for precise echo pulse measurement.
- **Distance Calculation:** Formula `Distance = (EchoTime * 0.0343) / 2`.
- **Threshold Logic:** Decision-making rule to classify slots as free or occupied.
- **LEDs & Buzzer:** Actuators for visual and audible output.
- **LCD/OLED:** A more complex output device for information display.
- **Serial Communication:** Used for debugging and monitoring via the Serial Monitor.
- **State Tracking:** Uses a flag (`isParkingFull`) to maintain the system's state.

## System Architecture

### Block Diagram
```
+---------------------------------------------------+
| MICROCONTROLLER |
| (Arduino UNO) |
| +--------------------------------------------+ |
| | | |
| | ┌──────────┐ ┌──────────┐ ┌────────┐ | |
| | │ Sensor 1 │ │ Sensor 2 │ │ Sensor N│ | |
| | └────┬─────┘ └────┬─────┘ └────┬───┘ | |
| | │ │ │ | |
| | └─────────────┴─────────────┘ | |
| | │ | |
| | ┌───────────┴──────────────┐ | |
| | │ PROCESSING & DECISION │ | |
| | │ - Threshold Logic │ | |
| | │ - Slot Status Update │ | |
| | │ - Available Count │ | |
| | └─────────────┬────────────┘ | |
| | ┌───────┴────────┐ | |
| | │ OUTPUTS │ | |
| | ┌──────┴──────┐ ┌─────┴─────┐ | |
| | │ LEDs │ │ LCD │ | |
| | └─────────────┘ └───────────┘ | |
| +--------------------------------------------+ |
+---------------------------------------------------+
```

### Input/Output Table

| Condition | Input (Sensor Distance) | Slot Status | LED Status | Buzzer | LCD Message |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Vehicle Present** | < 30 cm | OCCUPIED | Red ON, Green OFF | OFF | S1: OCC |
| **Slot Empty** | > 30 cm (or no echo) | FREE | Red OFF, Green ON | OFF | S1: FREE |
| **At Least One Free** | (Mixed) | (Mixed) | (Mixed) | OFF | Available: X/4 |
| **All Slots Occupied** | < 30 cm (All) | OCCUPIED (All) | All Red ON | ON | PARKING FULL |


### Pin Configuration

| Component | Function | Arduino Pin |
| :--- | :--- | :--- |
| **Sensor 1** | TRIG | 2 |
| | ECHO | 3 |
| **Sensor 2** | TRIG | 4 |
| | ECHO | 5 |
| **Sensor 3** | TRIG | 6 |
| | ECHO | 7 |
| **Sensor 4** | TRIG | 8 |
| | ECHO | 9 |
| **Slot 1** | Green LED | 10 (220Ω) |
| | Red LED | 11 (220Ω) |
| **Slot 2** | Green LED | 12 (220Ω) |
| | Red LED | 13 (220Ω) |
| **Slot 3** | Green LED | A0 (220Ω) |
| | Red LED | A1 (220Ω) |
| **Slot 4** | Green LED | A2 (220Ω) |
| | Red LED | A3 (220Ω) |
| **Buzzer** | Signal | A4 |
| **I2C LCD** | SDA | A4 (shared) |
| | SCL | A5 |
| **Servo (Optional)** | Signal | 9 |

## Distance Calculation

The HC-SR04 sensor works by sending a sound pulse and waiting for its echo. The time taken for the pulse to return is measured by the microcontroller.

The distance is calculated using the formula:
Distance (cm) = (Echo Pulse Duration (µs) * 0.0343) / 2

text
The division by 2 is essential because the sound wave travels to the object and back.

## Parking Slot Logic

The system uses a simple threshold to determine the occupancy of a slot.

```cpp
if (distance < THRESHOLD_DISTANCE) {
  Slot = OCCUPIED;
} else {
  Slot = FREE;
}
The THRESHOLD_DISTANCE is set to 30 cm to detect any object within that range as a vehicle.
```

## Folder Structure
```
Smart-Parking-Ultrasonic-Embedded-System/
│
├── src/                       # Main source code
│   └── smart_parking_system.ino
│
├── simulation/                # Virtual simulation files
│   └── wokwi_simulation.json
│
├── circuit_diagram/           # Circuit diagrams
│
├── screenshots/               # Screenshots for proof
│
├── README.md                  # Main project README
```

## Installation & Setup
Hardware Setup
Assemble the circuit on a breadboard as per the circuit diagram.
Connect the Arduino UNO to your computer via USB.
Open the Arduino IDE and select the correct board and COM port.
Install the LiquidCrystal I2C library from the Library Manager.

Software & Code
Clone this repository or download the .ino file.
Open the .ino file in the Arduino IDE.
Click the Upload button to compile and flash the code to your Arduino.
Open the Serial Monitor (baud rate: 9600) to view debug information.

## How to Run
Real Hardware: Power the Arduino. Place an object (e.g., a book) in front of a sensor. Observe the corresponding LEDs and the LCD changing state.

# Virtual Simulation (Wokwi):
Open the project on Wokwi using the provided wokwi_simulation.json file.
Start the simulation.
Click on an HC-SR04 sensor in the diagram and set its distance to a low value (e.g., 5 cm) to simulate a car.
Observe the outputs on the virtual LCD, LEDs, and Serial Monitor.

# Virtual Simulation
The project has been thoroughly tested on Wokwi, a powerful online simulation platform. This allows the project to be built, demonstrated, and tested without any physical hardware.

# Steps to Run on Wokwi:
Go to wokwi.com.
Create a new "Arduino UNO" project.
Add the components (HC-SR04 sensors, LEDs, LCD, Buzzer).
Wire the circuit as per the pin mapping table.
Paste the provided source code into the code editor.
Click "Start Simulation".
Change sensor distances to simulate vehicle parking and leaving.

## Limitations
- Number of Slots: The current implementation is designed for 4 slots. Scaling to a larger number of slots might require a more powerful microcontroller or a different system architecture.
- Sensor Range: The HC-SR04 is limited to a range of a few meters, making it suitable for smaller parking lots.
- Environmental Factors: The ultrasonic sensors can be affected by temperature, humidity, and acoustic noise, which may require periodic recalibration.
- No User Authentication: The system only manages occupancy and does not integrate with a ticketing or user management system.

## Future Improvements
- IoT Integration: Using an ESP32 to send real-time parking data to a cloud server for monitoring via a web or mobile application.
- Database Integration: Storing historical data to analyze parking patterns and generate reports.
- Automatic Gate Control: Integrating a servo motor to create a barrier that opens only when a slot is available.
- Advanced User Interface: Developing a more interactive GUI or mobile app.
- Edge AI: Using a camera and machine learning for more robust vehicle detection and to recognize license plates.
- Smart Lighting: Integrating the system with energy-efficient lighting controls.

## Learning Outcomes
- Through this project, the developer gained practical experience in:
- Designing and building a complete embedded system from scratch.
- Interfacing with various hardware components (sensors, actuators, display).
- Writing structured, modular, and well-documented C++/Arduino code.
- Applying fundamental concepts like threshold logic and state tracking.
- Using simulation tools for design and testing.
- Creating professional project documentation and a GitHub portfolio.

## Author
Shikha

## License
This project is licensed under the MIT License - see the LICENSE file for details.


---