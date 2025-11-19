import serial

PORT = "/dev/ttyUSB0"
BAUD = 115200
FILE = "data/scanned_2_4GH_data.txt"

ser = serial.Serial(PORT, BAUD, timeout=1)

f = open(FILE, "a")

try:
    while True:
        line = ser.readline().decode(errors="replace").strip()
        if line:
            print(line)
            f.write(line + "\n")
except KeyboardInterrupt:
    print(f"Data have been saved to {FILE}")
finally:
    ser.close()
    f.close()
