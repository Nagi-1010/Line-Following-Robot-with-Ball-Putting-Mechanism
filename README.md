# Line Following Robot with Ball-Putting Mechanism

A semester project based on an Arduino-controlled Line Following Robot (LFR) with a custom mechanical design, PID-based movement control, obstacle detection, and an additional ball-putting mechanism.

This project was designed as a practical mechatronics system by combining mechanical design, electronics, embedded programming, circuit planning, fabrication, and testing.

---

## Project Overview

The robot is designed to follow a black line using an IR sensor array and control its movement through DC geared motors driven by an L298N motor driver. The system also includes an ultrasonic sensor for obstacle detection and a servo-based ball-putting mechanism.

The mechanical structure was designed in Autodesk Fusion 360, while the circuit layout was prepared using Fritzing before the final hardware assembly.

---

## Main Features

- Arduino Uno based control system
- PID-based line following logic
- IR sensor array for line detection
- L298N motor driver for DC motor control
- Ultrasonic sensor for obstacle detection
- Servo motor for ball-putting mechanism
- Custom laser-cut metal chassis
- 3D-printed motor mounts and component holders
- Fritzing circuit design
- Fusion 360 mechanical assembly design

---

## Mechanical Design

The main chassis of the robot was designed with a size of **35 × 30 cm** and fabricated using **laser-cut metal**.  
The metal base gives the robot a strong and stable structure.

Several parts were designed separately and manufactured using **3D printing**, including:

- L298N motor driver base  
- 4-cell battery holder  
- DC motor mounts  
- Supporting parts for the mechanism  

The L298N base was 3D printed to isolate the motor driver from the metal chassis and reduce the risk of electrical short circuits.

---

## Electronics and Components

The main components used in this project include:

| Component | Purpose |
|---|---|
| Arduino Uno | Main controller |
| L298N Motor Driver | Controls DC motors |
| DC Geared Motors | Robot movement |
| IR Sensor Array | Line detection |
| Ultrasonic Sensor | Obstacle detection |
| Servo Motor | Ball-putting mechanism |
| 4-Cell Battery Pack | Power supply |
| Custom Chassis | Mechanical base |

---

## Software and Tools Used

| Tool / Software | Use |
|---|---|
| Arduino IDE | Programming and code upload |
| Autodesk Fusion 360 | Mechanical design and assembly |
| Fritzing | Circuit planning and wiring diagram |
| Laser Cutting | Main metal chassis fabrication |
| 3D Printing | Custom mounts and holders |

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
