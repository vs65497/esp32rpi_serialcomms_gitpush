#!/bin/bash

sudo timedatectl set-ntp true
sudo timedatectl timesync-status

while ! timeout 0.2 ping -c 1 -n 192.168.1.101 
do
	sleep 1
done

sudo python3 /home/sloth/esp32rpi_serialcomms_gitpush/launch.py >> /home/sloth/esp32rpi_serialcomms_gitpush/syslog.txt

