import os
import serial
import math
from time import sleep
from datetime import datetime

ser = serial.Serial('/dev/ttyAMA0', 9600)

sleep(1)

while True:

    print("I'm awake now. Sending confirmation to ESP32.")
    ser.write(str.encode("startup"))

    sleep(0.03)

    received_data = ser.read()
    #data_left = ser.inWaiting()
    #received_data += ser.read(data_left)

    print(received_data)
    print("recieved data finish printing.")
    if(received_data == "all systems are green. starting up!"):
        print("proceeding with execution.")
        break

    #os.system('sh pushtogit.sh')
    #sleep(30)

print("loop exited. executing...")
