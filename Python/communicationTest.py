import serial
import time

arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.01)


def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data


while True:
    num = input("Enter a command: ")
    value = write_read(num).decode('utf-8')
    print(value)