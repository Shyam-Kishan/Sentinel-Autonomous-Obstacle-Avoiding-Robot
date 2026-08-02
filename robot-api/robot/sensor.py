import serial

ser = serial.Serial('/dev/cu.usbserial-130', 9600, timeout=1)


def get_distance():
    try: 
        line = ser.readline().decode('utf-8').strip()
        if line.startswith("DIST: "):
            value = line.split("DIST: ")[1].strip()
            return float(value)
    except:
        pass

    return None

while True:
    dist = get_distance()
    if dist is not None:
        print(f"Distance: {dist}")