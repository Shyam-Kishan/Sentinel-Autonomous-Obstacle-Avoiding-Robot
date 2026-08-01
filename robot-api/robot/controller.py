import serial

ser = serial.Serial('/dev/usb.serial-130', 9600)

def move_robot(command):
    valid_commands = ["forward", "backward", "left", "right", "stop"]

    if command not in valid_commands:
        return "Invalid command"

    # Replace this later with Arduino serial communication
    # print(f"Robot moving: {command}")
    # return f"Executed {command}"
    
    ser.write(command.encode())
    return f"Sent {command} to robot"