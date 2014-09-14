LeptonModule
============
The FLIR Lepton™ is the most compact longwave infrared (LWIR) sensor available as an OEM product. It packs a resolution of 80 × 60 pixels into a camera body that is smaller than a dime. This revolutionary camera core is poised to equip a new generation of mobile and handheld devices, as well as small fixed-mount camera systems, with thermal imaging capabilities never seen before.Lepton contains a breakthrough lens fabricated in wafer form, along with a microbolometer focal plane array (FPA) and advanced thermal image processing. 


raspberrypi_capture
--------------
This is for the raspberry pi, 
you have to enable the spi and i2c ports first for this code to work. 

1. sudo vi /etc/modules
2. add # in front of spi-bcm2708 and ic2-dev
3. to compile the code just run "gcc raspberry_pi_capture.c"
4. to capture an image run "sudo ./a.out"
5. a file called image.pgm will be created, you can use GIMP to view the image


arduino_i2c
--------------
This example shows how to read the i2c ports using an Arduino. 
Note that most Arduino hardware does not have enough memory to buffer the thermal image. 80*60*2 = 9600 bytes. Some of the Arm based units will work. 


STM32F3Discovery_ChibiOS
--------------
Download and install ChibiOS_2.6.5 into the same directory first. 
This example takes the SPI data stream from the Lepton module, buffers it and send it out the USB VCP device. 
On the PC the binary stream of data can be parsed easily by looking for the 0xdeadbeef header on each frame. 
