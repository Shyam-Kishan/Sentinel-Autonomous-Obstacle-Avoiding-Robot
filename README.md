# Autonomous Obstacle-Avoiding Robot with Web Dashboard

## Overview

This project is a **full-stack robotics system** that enables real-time control and monitoring of an autonomous obstacle-avoiding robot through a web-based dashboard.

It integrates **hardware (Arduino + sensors)**, a **Python backend (Flask API)**, and a **frontend UI (HTML/CSS/JS + Chart.js)** to create a seamless interface for both **manual control** and **autonomous navigation**.

---

## Key Features

* **Manual & Autonomous Modes**

  * Switch between user-controlled navigation and onboard obstacle avoidance logic
* **Real-Time Telemetry**

  * Live distance readings streamed from ultrasonic sensors
* **Dynamic Data Visualization**

  * Distance-over-time graph using Chart.js
* **Serial Communication Layer**

  * Python interface for reliable communication with Arduino firmware
* **REST API**

  * Control and monitor the robot via HTTP endpoints

---

## System Architecture

```
Frontend (Dashboard UI)
        ↓
Flask API (app.py)
        ↓
Robot Interface (mainRobot.py)
        ↓
Serial Communication (USB)
        ↓
Arduino Firmware (C++)
        ↓
Sensors + Motors
```

---

## Tech Stack

### Frontend

* HTML, CSS, JavaScript
* Chart.js (real-time graphing)

### Backend

* Python (Flask)
* REST API design
* Serial communication (pyserial)

### Hardware

* Arduino (C++)
* Ultrasonic sensor
* Motor driver + wheels

---

## API Endpoints

| Endpoint     | Method | Description                                      |
| ------------ | ------ | ------------------------------------------------ |
| `/health`    | GET    | Check server status                              |
| `/move`      | POST   | Send movement commands (`forward`, `left`, etc.) |
| `/distance`  | GET    | Retrieve current distance                        |
| `/telemetry` | GET    | Get full robot state (distance, mode, timestamp) |

---

## Dashboard Features

* Directional controls (forward, backward, left, right, stop)
* Mode switching (auto/manual)
* Live telemetry display:

  * Distance
  * Mode
  * Status
  * Timestamp
* Real-time distance graph

---

## How It Works

1. User interacts with the **web dashboard**
2. Commands are sent to the **Flask backend**
3. Backend communicates with the robot via **serial (USB)**
4. Arduino executes movement or autonomous logic
5. Sensor data is sent back and visualized in real-time

---

## Notable Implementation Details

* **Custom binary packet parsing** for ultrasonic sensor data (`0xAA 0x55` header)
* **State tracking** for robot mode (manual vs auto)
* **Buffered serial reads** to ensure reliable data extraction
* **Graceful shutdown handling** using `atexit` to safely stop the robot

---

## Future Improvements

* Add camera-based navigation (OpenCV)
* Deploy dashboard remotely (cloud hosting)
* Improve obstacle avoidance with SLAM/path planning
* Add authentication for secure control

---

## Demo

> (Add GIF/video here of robot + dashboard in action)

---

## Conclusion

This project demonstrates the integration of **embedded systems, backend engineering, and frontend development** to build a complete, real-time robotic control platform.

It highlights skills in:

* Systems design
* API development
* Hardware/software integration
* Real-time data processing

---