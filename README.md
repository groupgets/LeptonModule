LeptonModule
============
The FLIR Lepton™ is the most compact longwave infrared (LWIR) sensor available as an OEM product. The Lepton 2.x camera cores has a resolution of 80 × 60 pixels and the newer Lepton 3.x camera cores have a resolution of 160 x 120 pixels respectively. The Lepton 2.5 and 3.5 are both also radiometric which means they can output a factory-calibrated temperature value for all pixels in a frame irrespective of the camera temperature with an accuracy of +/-5˚C. Lepton contains a breakthrough lens fabricated in wafer form, along with a microbolometer focal plane array (FPA) and advanced thermal image processing.

More Information
https://groupgets.com/manufacturers/flir/products/lepton-3-5


beagleboneblack_video
--------------
This example is for the Beagle Bone Black board and runs a basic video feed.
Requires the device's SPI0 tree to be exported to a slot before running.
Steps are outlined within this directory.


raspberrypi_capture
--------------
This is for the Raspberry Pi,
you have to enable the spi and i2c ports first for this code to work.

1. sudo vi /etc/modules
2. add # in front of spi-bcm2708 and ic2-dev
3. to compile the code just run "gcc raspberry_pi_capture.c"
4. to capture an image run "sudo ./a.out"
5. a file called image.pgm will be created, you can use GIMP to view the image

See the wiki page for more information: https://github.com/groupgets/LeptonModule/wiki

raspberrypi_video
--------------
This is for the Raspberry Pi, see this page for more information https://github.com/groupgets/LeptonModule/tree/master/software/raspberrypi_video


Software modules:

arduino_i2c
--------------
This example shows how to read the i2c ports using an Arduino.
Note that most Arduino hardware does not have enough memory to buffer the thermal image. 80*60*2 = 9600 bytes. Some of the Arm based units will work.


STM32F3Discovery_ChibiOS
--------------
Download and install ChibiOS_2.6.5 into the same directory first.
This example takes the SPI data stream from the Lepton module, buffers it and send it out the USB VCP device.
On the PC the binary stream of data can be parsed easily by looking for the 0xdeadbeef header on each frame.


stm32nucleo_401re
--------------
This example shows how to read stream photos at the rate of about 1 every 2 seconds from the $10 STM32nucleo 401 RE development board. This example uses the mbed toolchain, an extremely simple to use web based IDE.

ThermalView
--------------
This super simple win32 example shows how draw the images onto the screen of a windows computer. This software is compiled using MinGW toolchain.  

edison_capture
--------------
This example shows how to capture a still image from the lepton using Intel Edison.

v4l2lepton
----------
This simple program will stream frames from the Lepton to a v4l2loopback device. Very useful for interacting with the Lepton in the same way you would a webcam. This was created mainly for use on Raspberry Pi with the original breakout board.

Python
--------------
If you'd like to use the breakout board with Python on Linux, check out the pylepton project: https://github.com/groupgets/pylepton
