import serial
import struct
import time

class Robot:
    def __init__(self, port='/dev/cu.usbserial-130', baud=115200):
        self.ser = serial.Serial(port, baud, timeout=1)
        self.ser.reset_input_buffer()
        time.sleep(2)  # allow Arduino to reset
        
        self.mode = "manual"
        self.last_distance = None
        self.valid_commands = [
            "forward", "backward", "left", "right",
            "stop", "auto", "manual"
        ]

    # Send movement / mode commands
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

    # Read distance from Arduino
    def get_distance(self):
        try:
            buffer = bytearray()

            if self.ser.in_waiting:
                buffer += self.ser.read(self.ser.in_waiting)
            
            # Keep buffer from growing forever
            if len(buffer) > 100:
                buffer = buffer[-50:]

            # Look for header inside buffer
            for i in range(len(buffer) - 5):
                if buffer[i] == 0xAA and buffer[i+1] == 0x55:
                    packet = buffer[i+2:i+6]

                    if len(packet) < 4:
                        continue

                    value = struct.unpack('<f', packet)[0]
                    print("RAW BYTES:", list(packet))
                    print("Distance:", value)

                    buffer = buffer[i+6:]
                    return round(value, 2)
                
        except Exception as e:
            print("Sensor error:", e)

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

    # Cleanup on exit
    def close(self):
        self.stop()
        self.ser.close()