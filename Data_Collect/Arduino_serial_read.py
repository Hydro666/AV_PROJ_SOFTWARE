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


# Read the incoming data from the Serial Port every 400 ms.
# Takes in serial port, duration, constant value, and file name
# Writes the Serial output to specified file name and appends the
# Constant specified
def data_read(port, dur, const, file):
    t_end = time.time() + dur
    while time.time() < t_end:
        try:
            data = port.readline()
            decoded_data = data.decode('utf-8')
            file.write(const + decoded_data)
            print(const, decoded_data)
            time.sleep(.4)
        except port.SerialTimeoutException:
            print('Data could not be read.')
    file.close()


# Retrieve the data from the Arduino
def data_retrieve_prox():
    # Determine the user input: Name, Duration, distance
    name = input("What is the test/file name?: \n")
    sec = int(input("Test duration(s): \n"))
    dis = input("Distance (cm): \n")
    summary = input("Please write a small description of the test."
                    "(Leave blank if appending to existing file):  \n")

    # Create or open the specified file
    with open("%s" % name + ".txt", "a+") as file:
        ser = serial.Serial('COM4', 9600, timeout=2)
        # Give Arduino time to warm up -> Resets every time serial line is open
        print("Loading...")
        time.sleep(1.5)
        file.write(summary + "\n")
        file.write("File name: %s, Duration: %i, Distance: %s \n" % (name, sec, dis))
        print("Gathering data now!")
        # Gather data
        data_read(ser, sec, dis, file)
    # Ask User if they want to see data
    nxt = input("Do you want to see the results? (Y/N)")
    if nxt == "Y":
        f = open("%s" % name + ".txt", "r")
        fl = f.readlines()
        for x in fl:
            print(x)
        f.close()
        return main()
    elif nxt == "N":
        return main()


# Home screen that asks the user what they want to do
def main():
    print("What do you want to do?\n"
          "1. Gather data\n" 
          "2. Analyze data")
    res = input("(1,2): ")
    if res == "1":
        res = input("For which sensor?\n1. Proximity\n"
                    "2. Accelerometer\n")
        if res == "1":
            data_retrieve_prox()
        elif res == "2":
            print("sorry this isn't ready yet!")
            return main()
    if res == "2":
        print("Sorry this isn't ready yet!")
        return main()


main()
