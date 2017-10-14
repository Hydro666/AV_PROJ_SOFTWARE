# This program is part of the Autonomous vehicle project created by Aquiles Gomez
# This program reads the serial port of an Arduino mega taking in sensor data and
# Stores it within a text file
# The values within the text file will be analyzed and used to determine the most
# Effective way to calculate accurate values utilizing analog sensors.

import serial
import time

# Open the serial port
ser = serial.Serial('COM4', 9600, timeout=0)
# Loop as long as we receive a serial value
while 1:
    try:
        data = ser.readline()
        print(data.decode('utf-8'))
        # Print values every second
        time.sleep(1)
    except ser.SerialTimeoutException:
        print('data could not be read')
    time.sleep(1)