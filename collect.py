import os
import serial
from time import sleep
from datetime import datetime

ser = serial.Serial('/dev/ttyAMA0', 9600)

while True:

    print("collecting data...")

    line = "index, timestamp, data\n"
    for i in range(0,10):

        received_data = ser.read()
        sleep(0.03)
        data_left = ser.inWaiting()
        received_data += ser.read(data_left)

        now = datetime.now()
        timestamp = now.strftime("%H:%M:%S")

        datastr = str(received_data)

        entry = str(i) + ", " + timestamp + ", " + datastr + " \n"
        line += entry
        print(entry)
        
        #ser.write(received_data)

    print("writing to file...")

    with open("data/latest.csv", "w") as file:
        file.write(line)

    now = datetime.now()
    timestamp = now.strftime("%Y-%m-%d_%H-%M-%S")

    with open("data/"+timestamp+".csv", "w") as file:
        file.write(line)

    #os.system('cat data/latest.csv')

    # in order to make this possible we need to follow this
    # https://stackoverflow.com/questions/16709404/how-to-automate-the-commit-and-push-process-git
    # chmod +x pushtogit.sh

    print("pushing to git...")
    
    os.system('sh pushtogit.sh')
    sleep(30)
