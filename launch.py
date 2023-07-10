import os
import subprocess
import serial
import math
from time import sleep
from datetime import datetime

ser = serial.Serial('/dev/ttyAMA0', 115200)

# get handshake with ESP32
def handshake():
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
            return
        else:
            print("no dice")

    print("loop exited. executing...")

handshake() # when successful, continue with execution

print("collecting data...")

line = "index, timestamp, temp, press, humid, alt, millis, ir, full, visible, lux, tvoc, eco2, h2, eth\n"
for i in range(0,10):

    received_data = ser.read()
    sleep(0.03)
    data_left = ser.inWaiting()
    received_data += ser.read(data_left)

    now = datetime.now()
    timestamp = now.strftime("%H:%M:%S")

    datastr = received_data.decode()
    #datastr = "testing"

    print(datastr)

    entry = str(i) + ", " + timestamp + ", " + datastr + " \n"
    line += entry
    print(entry)
    
    #ser.write(received_data)

print("writing to file...")

latest_path = "/home/sloth/esp32rpi_serialcomms_gitpush/data/latest.csv"
with open(latest_path, "w") as file:
    file.write(line)

now = datetime.now()
timestamp = now.strftime("%Y-%m-%d_%H-%M-%S")

archive_path = "/home/sloth/esp32rpi_serialcomms_gitpush/data/"+timestamp+".csv"
with open(archive_path, "w") as file:
    file.write(line)

#os.system('cat data/latest.csv')

# in order to make this possible we need to follow this
# https://stackoverflow.com/questions/16709404/how-to-automate-the-commit-and-push-process-git
# chmod +x pushtogit.sh

print("pushing to git...")

#os.system('sh pushtogit.sh')
#subprocess.run(['sh','/home/sloth/esp32rpi_serialcomms_gitpush/pushtogit.sh'])

#p1 = subprocess.Popen('scp '+ archive_path +' base@192.168.1.101:/home/base/slothdata/data', shell=True)
#sts1 = p1.wait()
#p2 = subprocess.Popen('scp '+ latest_path +' base@192.168.1.101:/home/base/slothdata/data', shell=True)
#sts2 = p2.wait()

f = open("/home/sloth/esp32rpi_serialcomms_gitpush/errorlog.txt", "w")

p1 = subprocess.Popen('scp '+ archive_path +' base@192.168.1.101:/home/base/slothdata/data', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
output, error = p1.communicate()

if p1.returncode != 0:
    f.write('p1 output - %d %s %s\n' % (p1.returncode, output.decode(), error.decode()))

sts1 = p1.wait()

p2 = subprocess.Popen('scp '+ latest_path +' base@192.168.1.101:/home/base/slothdata/data', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
output, error = p2.communicate()

if p2.returncode != 0:
    f.write('p2 output - %d %s %s\n' % (p2.returncode, output.decode(), error.decode()))

sts2 = p2.wait()

f.close()

#p = subprocess.Popen('sh /home/sloth/esp32rpi_serialcomms_gitpush/test.sh', shell=True)
#sts = p.wait()

handshake()
