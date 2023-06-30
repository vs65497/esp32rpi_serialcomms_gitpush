# esp32rpi_serialcomms_gitpush
Automatically get sensor data from esp32 then push up to the repo.

TODO: write documentation for startup scripts on ubuntu
https://www.howtogeek.com/687970/how-to-run-a-linux-program-at-startup-with-systemd/

TODO: write documentation for passwordless scp between raspberry pi's
https://www.linux.com/training-tutorials/ssh-scp-without-password-remote-host-look-ma-no-password/

TODO: finish cronjob tutorial. write documentation for it. this should push csv files up to github
https://www.digitalocean.com/community/tutorials/how-to-use-cron-to-automate-tasks-ubuntu-1804
use crontab -e

May have issues with locale
- https://lintut.com/how-to-set-up-system-locale-on-ubuntu-18-04/
- https://askubuntu.com/questions/599808/cannot-set-lc-ctype-to-default-locale-no-such-file-or-directory

--> Update 5/31/23, 3:14pm -- Testing automatic push
--> Update 5/31/23, 3:17pm -- Success!
