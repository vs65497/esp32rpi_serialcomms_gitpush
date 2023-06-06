import os
import serial
import math
from time import sleep
from datetime import datetime

ser = serial.Serial('/dev/ttyAMA0', 9600)

sleep(1)

go_execute = False
while True and go_execute == False:

    print("I'm awake now. Sending confirmation to ESP32.")

    # clear buffers
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    ser.write(str.encode("RECEIVED\n"))

    sleep(0.1)

    received_data = ser.read()
    data_left = ser.inWaiting()
    received_data += ser.read(data_left)

    data = received_data.decode()
    print(data)

    if(data == 'EXECUTE\r\n'):
        print("proceeding with execution.")
        go_execute = True
    else:
        print("no dice")

    #os.system('sh pushtogit.sh')
    #sleep(30)

print("loop exited. executing...")
