LeptonModule
============


raspberrypi_capture
--------------
This is for the raspberry pi, 
you have to enable the spi and i2c ports first for this code to work. 

1. sudo vi /etc/modules
2. add # in front of spi-bcm2708 and ic2-dev
3. to compile the code just run "gcc raspberry_pi_capture.c"
4. to capture an image run "sudo ./a.out"
5. an file called image.pgm will be created, you can use GIMP to view the image
