# Line Following Robot with Ball-Putting Mechanism

## Author: Nagi Al-Yafaey

A semester project based on an Arduino-controlled Line Following Robot (LFR) with a custom mechanical design, PID-based movement control, obstacle detection, and a ball-putting mechanism.

This project was developed as a practical mechatronics system by combining mechanical design, electronics, embedded programming, circuit planning, fabrication, and testing.

---

## Project Overview

The robot is designed to follow a black line using an IR sensor array and control its movement through DC geared motors driven by an L298N motor driver. The system also includes an ultrasonic sensor for obstacle detection and a servo-based ball-putting mechanism.

The mechanical assembly was designed in Autodesk Fusion 360, while the circuit layout was prepared using Fritzing before final hardware implementation.

---

## Main Features

- Arduino Uno based control system
- PID-based line following logic
- IR sensor array for line detection
- L298N motor driver for DC motor control
- DC geared motors for movement
- Ultrasonic sensor for obstacle detection
- Servo motor for the ball-putting mechanism
- Custom laser-cut metal chassis
- 3D-printed mounts and component holders
- Fusion 360 mechanical design
- Fritzing circuit layout

---

## Mechanical Design

The main chassis of the robot was designed with dimensions of **35 × 30 cm** and fabricated using **laser-cut metal**. This gave the robot a strong and stable base for carrying the motors, battery pack, controller, driver, sensors, and mechanism.

Several supporting parts were designed separately and made using **3D printing**, including:

- L298N motor driver base
- 4-cell battery holder
- DC motor mounts
- Supporting parts for the ball-putting mechanism

The L298N motor driver base was 3D printed to isolate the driver from the metal chassis and reduce the risk of short circuits.

---

## Electronics and Components

| Component | Purpose |
|---|---|
| Arduino Uno | Main controller |
| L298N Motor Driver | Controls the DC motors |
| DC Geared Motors | Provides robot movement |
| IR Sensor Array | Detects the black line |
| Ultrasonic Sensor | Detects obstacles |
| Servo Motor | Operates the ball-putting mechanism |
| 4-Cell Battery Pack | Provides power |
| Laser-Cut Metal Chassis | Main robot structure |
| 3D-Printed Parts | Mounting and isolation support |

---

## Software and Tools Used

| Tool / Software | Purpose |
|---|---|
| Arduino IDE | Programming and uploading code |
| Autodesk Fusion 360 | Mechanical design and assembly |
| Fritzing | Circuit design and wiring layout |
| Laser Cutting | Fabrication of the metal chassis |
| 3D Printing | Manufacturing custom mounts and holders |

---

## Repository Structure

```text
Line-Following-Robot-Ball-Mechanism/
│
├── code/
│   └── LFR_Ball_Putting_Mechanism.ino
│
├── circuit/
│   └── fritzing_circuit_diagram.jpg
│
├── fusion360_design/
│   └── fusion360_design_images
│
├── images/
│   └── project_images
│
├── 3d_printing_parts/
│   └── stl_files
│
└── README.md
