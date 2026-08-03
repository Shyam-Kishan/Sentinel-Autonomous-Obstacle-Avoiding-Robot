import serial
import time

class Robot:
    def __init__(self, port='/dev/cu.usbserial-130', baud=9600):
        self.ser = serial.Serial(port, baud, timeout=1)
        time.sleep(2)  # allow Arduino to reset
        
        self.mode = "manual"
        self.last_distance = None
        self.valid_commands = [
            "forward", "backward", "left", "right",
            "stop", "auto", "manual"
        ]

    # 🔹 Send movement / mode commands
    def send_command(self, command):
        if command not in self.valid_commands:
            return "Invalid command"

        self.ser.write((command + "\n").encode())

        # Keep record of the mode
        if command == "auto":
            self.mode = "auto"
        elif command == "manual":
            self.mode = "manual"

        return f"Sent {command}"

    # 🔹 Read distance from Arduino
    def get_distance(self):
        try:
            while self.ser.in_waiting:
                line = self.ser.readline().decode('utf-8').strip()

                if line.startswith("DIST:"):
                    value = line.split("DIST:")[1].strip()
                    return float(value)

        except Exception as e:
            print("Sensor error:", e)

        return None

    def get_telemetry(self):
        new_distance = self.get_distance()

        if new_distance is not None:
            self.last_distance = new_distance

        telemetry = {
            "distance":self.last_distance,
            "mode":self.mode,
            "timestamp":time.time()
        }

        return telemetry
    # 🔹 Optional: stop robot (safety)
    def stop(self):
        self.ser.write(b"stop\n")

    # 🔹 Cleanup on exit
    def close(self):
        self.stop()
        self.ser.close()