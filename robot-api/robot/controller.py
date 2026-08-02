import serial

ser = serial.Serial('/dev/cu.usbserial-130', 9600)

def move_robot(command):
    valid_commands = ["forward", "backward", "left", "right", "stop", "auto", "manual"]

    if command not in valid_commands:
        return "Invalid command"

    # print(f"[DEBUG] Received command: {command}")
    ser.write(command.encode())
    return f"Sent {command} to robot"