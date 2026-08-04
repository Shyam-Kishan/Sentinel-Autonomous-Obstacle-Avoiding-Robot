# 🤖 Robot Telemetry & Control API

A full-stack embedded systems project that enables real-time monitoring and control of an Arduino-based mobile robot through a custom web dashboard.

The system bridges embedded hardware and software by creating a communication pipeline between an Arduino robot controller, a Python backend API, and a browser-based user interface.

The robot supports both **manual control** and **autonomous obstacle avoidance**, while continuously streaming telemetry data such as distance measurements, operating mode, and timestamps.

---

# 📌 Features

## 🎮 Real-Time Robot Control

Control robot movement through a browser interface:

- Forward movement
- Reverse movement
- Left/right turning
- Emergency stop
- Manual/autonomous mode switching

Commands are sent from the frontend to a Flask API, which communicates with the Arduino over serial communication.

---

## 📡 Live Robot Telemetry

The dashboard displays real-time robot information:

- Ultrasonic distance measurements
- Current operating mode
    - Manual
    - Autonomous
- Timestamped sensor updates

Telemetry data is continuously requested from the backend and displayed dynamically.

---

## 🧠 Autonomous Obstacle Avoidance

The robot includes an autonomous navigation mode using:

- Ultrasonic distance sensing
- Servo-based sensor scanning
- Distance filtering
- Exponential moving average (EMA) smoothing
- Reactive decision making

The robot evaluates surrounding distances and adjusts movement behavior to avoid obstacles.

---

## 🖥️ Web-Based Dashboard

A custom HTML/CSS/JavaScript interface provides:

- Robot status monitoring
- Live telemetry updates
- Interactive control buttons
- Mode selection
- Human-readable timestamps

The dashboard communicates with the Flask backend through REST endpoints.

---

# 🏗️ System Architecture

```
                  Browser Dashboard
                (HTML/CSS/JavaScript)
                         |
                         |
                    REST API
                     Flask
                         |
                         |
                Python Robot Controller
                         |
                         |
                  Serial Communication
                         |
                         |
                  Arduino Robot
                         |
        --------------------------------
        |                              |
 Motor Control                  Sensor Processing
        |                              |
 DC Motors                  Ultrasonic + Servo Sensor
```

---

# 🛠️ Technologies Used

## Embedded Systems
- Arduino
- C/C++
- Servo motor control
- Ultrasonic distance sensing
- PWM motor control
- Serial communication

## Backend
- Python
- Flask
- REST API design
- PySerial

## Frontend
- HTML
- CSS
- JavaScript
- Fetch API

## Development Tools
- VS Code
- Git/GitHub
- Serial debugging

---

# 🔌 API Endpoints

## Health Check

### GET `/health`

Checks whether the backend server is running.

Example response:

```json
{
    "status": "running"
}
```

---

## Robot Control

### POST `/move`

Sends commands to the robot.

Example request:

```json
{
    "command": "forward"
}
```

Supported commands:

```
forward
backward
left
right
stop
auto
manual
```

---

## Distance Telemetry

### GET `/distance`

Returns the latest ultrasonic sensor measurement.

Example:

```json
{
    "distance": 28.66
}
```

---

## Robot Telemetry

### GET `/telemetry`

Returns complete robot status information.

Example:

```json
{
    "distance": 28.66,
    "mode": "manual",
    "timestamp": 1785726182
}
```

---

# 📂 Project Structure

```
Robot-Telemetry-Control-API/
│
├── Arduino/
│   └── sentinel_robot.ino
│
├── Backend/
│   ├── app.py
│   └── mainRobot.py
│
├── Frontend/
│   └── index.html
│
└── README.md
```

---

# 🚀 How It Works

1. The Arduino continuously reads ultrasonic sensor measurements.
2. Sensor data is transmitted through serial communication.
3. The Python backend receives sensor data and maintains robot state.
4. Flask exposes REST API endpoints for:
    - Sending movement commands
    - Retrieving telemetry information
5. The browser dashboard periodically requests updated telemetry data.
6. User commands are sent back through the API and executed by the robot.

---

# 🧩 Key Engineering Concepts Learned

## Embedded-to-Software Communication

Learned how to bridge hardware and software systems using serial communication between an Arduino microcontroller and a Python application.

---

## REST API Development

Designed backend endpoints that allow external applications to interact with robot hardware through structured HTTP requests.

---

## Real-Time Data Handling

Implemented continuous telemetry updates by managing sensor streams, API polling, and frontend updates.

---

## Hardware Abstraction

Created a Python robot controller layer that separates high-level commands from low-level hardware implementation.

---

## Sensor Filtering

Implemented techniques such as:

- Data validation
- Sensor averaging
- Exponential moving average filtering

to reduce noise from ultrasonic sensor readings.

---

## State Management

Designed a system that tracks robot operating states:

- Manual mode
- Autonomous mode

and ensures commands are routed appropriately.

---

# 🔮 Future Improvements

## Wireless Robot Communication

Replace the USB serial connection with wireless communication using:

- ESP32 WiFi module
- MQTT messaging
- WebSocket communication

allowing remote robot control over a network.

---

## Cloud Deployment

Deploy the backend API to a cloud service to allow robot monitoring from anywhere.

Potential improvements:

- Remote authentication
- Cloud telemetry storage
- User dashboards

---

## Real-Time Streaming

Replace HTTP polling with WebSockets for faster telemetry updates and lower latency.

---

## Improved Autonomous Navigation

Enhance obstacle avoidance with:

- More advanced path planning
- Sensor fusion
- Mapping algorithms
- Computer vision integration

---

## Frontend Improvements

Potential UI upgrades:

- Live distance graphs
- Battery monitoring
- Robot camera integration
- Mobile-friendly controls

---

# 🎯 Project Goals

This project demonstrates the integration of:

- Embedded systems
- Backend software engineering
- API development
- Hardware communication
- Real-time data processing

by creating a complete end-to-end robot control platform.
