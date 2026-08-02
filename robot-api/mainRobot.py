import serial
import time

class Robot:
    def __init__(self, port='/dev/cu.usbserial-130', baud=9600):
        self.ser = serial.Serial(port, baud, timeout=1)
        time.sleep(2)  # allow Arduino to reset

        self.valid_commands = [
            "forward", "backward", "left", "right",
            "stop", "auto", "manual"
        ]

    # 🔹 Send movement / mode commands
    def send_command(self, command):
        if command not in self.valid_commands:
            return "Invalid command"

        self.ser.write((command + "\n").encode())
        return f"Sent {command}"

    # 🔹 Read distance from Arduino
    def get_distance(self):
        try:
            line = self.ser.readline().decode('utf-8').strip()

            if line.startswith("DIST:"):
                value = line.split("DIST:")[1].strip()
                return float(value)

        except Exception as e:
            print("Sensor error:", e)

        return None

    # 🔹 Optional: stop robot (safety)
    def stop(self):
        self.ser.write(b"stop\n")

    # 🔹 Cleanup on exit
    def close(self):
        self.stop()
        self.ser.close()