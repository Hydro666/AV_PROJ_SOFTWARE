"""
 This program is part of the Autonomous vehicle project created by Aquiles Gomez
 This program reads the serial port of an Arduino mega taking in sensor data and
 Stores it within a text file
 The values within the text file will be analyzed and used to determine the most
 Effective way to calculate accurate values utilizing analog sensors and a digital
 accelerometer. This program also allows for multiple testing sessions with the ability to choose
 the sensor that will be measured.

 Author: Aquiles Gomez
 Date: 10/15/2017
 Created for solely educational purposes
 """

import serial
import time
import sys


# Read the incoming data from the Serial Port every 400 ms.
# Takes in serial port, duration, constant value, and file name
# Writes the Serial output to specified file name and appends the
# Constant specified
def data_read(dur, const, file):
    ser = serial.Serial('COM4', 9600, timeout=2)
    # Give Arduino time to warm up -> Resets every time serial line is opened
    print("Loading...")
    time.sleep(1.5)
    t_end = time.time() + dur
    print("Gathering data now!")
    while time.time() < t_end:
        try:
            data = ser.readline()
            decoded_data = data.decode('utf-8')
            file.write(const + decoded_data)
            print(const, decoded_data)
            time.sleep(.4)
        except ser.SerialTimeoutException:
            print('Data could not be read.')
    # Close the file and port after use
    file.close()
    ser.close()


# Function that occurs after the test is finished; returns response of the user if they want to
# Repeat the test or end it. Returns True if rerun. Retruns False if they want to end the test
def next_action(file_name):
    response = input("Do you want to see the results? (Y/N)")
    if response == "Y":
        f = open("%s" % file_name + ".txt", "r")
        fl = f.readlines()
        for x in fl:
            print(x)
        f.close()
        response = input("Do you want to rerun this script? (Y/N)")
        if response == "Y":
            return True
        else:
            return main()
    elif response == "N":
        response = input("Do you want to rerun this script? (Y/N)")
        if response == "Y":
            return True
        else:
            return main()


# Retrieve the data from the Arduino for the Sensors and store it to a text file
def data_retrieve():
    user = input("Do you want to test the proximity sensor or accelerometer? (1/2): \n")
    # Determine the user input: Test name, Duration, distance, and a short summary of the test
    name = input("What is the test/file name?: \n")
    sec = int(input("Test duration(s): \n"))

    if user == "1":
        const = input("Distance (cm): \n")
    elif user == "2":
        const = input("Resolution of the sensor (2,4,8): \n")

    summary = input("Please write a small description of the test."
                    "Leave blank if appending to existing file):  \n")

    # Create or open the specified file
    with open("%s" % name + ".txt", "a+") as file:
        file.write(summary + "\n")
        if user == "1":
            file.write("File name: %s, Duration: %i, Distance: %s \n" % (name, sec, const))
        elif user == "2":
            file.write("File name: %s, Duration: %i, Resolution: %s \n" % (name, sec, const))
        # Gather data
        data_read(sec, const, file)
    # Ask User if they want to see data / repeat the test
    if next_action(name) is True:
        return data_retrieve()


# Home screen that asks the user what they want to do
def main():
    print("What do you want to do?\n"
          "1. Gather data\n" 
          "2. Analyze data\n"
          "3. Exit")
    res = input("(1,2,3): ")
    if res == "1":
        data_retrieve()
    elif res == "2":
        print("Sorry this isn't ready yet!")
        return main()
    elif res == "3":
        sys.exit()


main()
