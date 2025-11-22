import serial
import time


def main():
    # Change this to your Arduino serial port!
    # On Windows: "COM3", "COM5", etc.
    # On Linux / macOS: "/dev/ttyACM0" or "/dev/ttyUSB0"
    port = "/dev/ttyACM0"

    print(f"Connecting to Arduino on {port}...")
    ser = serial.Serial(port, 115200, timeout=1)

    # give Arduino time to reset after opening the port
    time.sleep(2)

    print("Connected! Type something to send to Arduino.\n")

    while True:
        msg = input("> ")

        if msg.lower() == "exit":
            break

        ser.write((msg + "\n").encode())

    ser.close()


if __name__ == "__main__":
    main()
