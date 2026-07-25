# 🚗 Sentinel: Autonomous Obstacle-Avoiding Robot !! WORK IN PROGRESS !!

## 📌 Overview
**Sentinel** is an embedded robotics project focused on building a mobile robotic platform capable of **autonomous navigation**. The system integrates low-level motor control with sensor-driven decision-making, allowing the robot to react dynamically to its environment in real time.

This project demonstrates core principles in **embedded systems**, **robotics**, and **control logic design** using an Arduino-based architecture.

---

## 🎯 Features

- 🔧 **PWM Motor Control**  
  Fine-grained speed control of left and right motors using PWM signals

- 🤖 **Autonomous Obstacle Avoidance**  
  Uses an ultrasonic sensor to detect obstacles and adjust movement accordingly

- 🔄 **State-Based Control System**  
  Maintains and executes movement states (forward, backward, turning, stop)

- ⚡ **Dual Power System**  
  Separate power handling for logic (USB) and motors (battery pack)

---

## 🧠 System Architecture

```
        Sensor Data (Ultrasonic)
                ↓
        Decision Logic (Arduino)
                ↓
        Motor Control (PWM Signals)
                ↓
        Robot Movement
```

---

## 🧰 Hardware Components

- Arduino Uno (or compatible)
- SmartCar Shield V1.1
- DC Motors (x4)
- Battery Pack (for motor power)
- Ultrasonic Sensor (HC-SR04)
- Robot chassis kit

---

## ⚙️ Software Components

- Arduino (C/C++)
- Serial communication interface
- PWM-based motor control
- Sensor data processing
- State machine logic

---

## 🤖 Autonomous Mode

When enabled, the robot:

1. Moves forward  
2. Detects obstacles using ultrasonic sensor  
3. Stops if object is too close  
4. Turns and continues navigating  

---

## 🧪 Calibration

Turning accuracy is achieved through **empirical calibration**:

- Measure time for full 360° rotation  
- Divide by 4 to approximate 90° turn  
- Adjust for real-world conditions (surface, battery level)

---

## 📈 Future Improvements

- 🔵 Bluetooth / WiFi control  
- 🧭 Path planning algorithms  
- 🧠 Sensor fusion (multiple sensors)  
- 🎥 Computer vision integration  
- 📍 Mapping & localization  

---

## 🧠 Key Concepts Learned

- Embedded systems programming  
- PWM and motor control  
- Real-time input handling  
- Sensor integration  
- Reactive robotics  
- Debugging hardware/software systems  

---

## 📸 Demo 
_Photos will be added soon_
