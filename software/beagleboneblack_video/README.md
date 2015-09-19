This example is meant for BeagleBone Black.

It has been tested on the OS Debian.

BeagleBone Black Setup Steps
--------------
**NOTE: Removing power by unplugging the power cable can damage the board. Use the power button or terminal interface to power down the board.**

Steps to setup:

1.	If you’re starting with a brand new BeagleBone Black: follow the steps on http://beagleboard.org/getting-started to get started. 
 - **Note:** If you’re using a Mac, you can download and use Pi Filler from http://ivanx.com/raspberrypi/ instead of using 7-Zip

2.	The Physical Setup.
 - Connect USB Hub to BeagleBone Black 
 - Connect Mouse and Keyboard to Hub
 - Connect monitor to BeagleBone Black via micro HDMI cable
 - Do not connect to power yet

3.	Setup breadboard and wiring to BeagleBone
 - Insert the Lepton into the breakout board with the black triangle on the faceplate being on bottom right corner. 
 - Attach breakout board to the breadboard (you can skip this step if you wanted to directly wire the breakout board to the beaglebone).
 - Go to the following link to see the pinout of the BeagleBone that will be configured after the completion of step 7.
   - http://beagleboard.org/static/images/cape-headers.png
 - Wire the Lepton to the BeagleBone with the following connections:
   - CS to pin 17 (SPI0_CS0)
    - MISO to pin 21 (SPI_D0)
    - CLK to pin 22 (SPI0_CLK)
    - GND to pin 1 (DGND)
    - VIN to pin 3 (VDD_3V3)
    - SDA to pin 20 (I2C2_SDA)
    - SCL to pin 19 (I2C2_SCL)


4.	Connect 5V Power Cable to BeagleBone. 
 - **Note:** The board can be powered either by connecting the power cable or connecting to a computer via the USB cable. Connecting both can cause damage to the board. For this section of the setup, we are not connecting to a computer so only the power cable should be attached to the board and not the USB cable.

5.	Update BeagleBone Black’s packages by running the following commands
 - apt-get upgrade
 - apt-get update
 - apt-get install qt4-dev-tools

6.	Reboot BeagleBone Black 

7.	Configure BeagleBone Pins
 - Run the following command to check the board’s slots. There should be 5
   - cat /sys/devices/bone_capemgr.9/slots
 - Export SPI device tree overlay with the following command:
   - echo ADAFRUIT-SPI0 > /sys/devices/bone_capemgr.9/slots
 - When running the cat command from part a, you should now see a 6th slot being occupied 

8.	Clone BeagleBone Black Lepton repository
 - git clone this repo to your machine

9. Run the demo by entering the following commands exactly as they appear
 - cd LeptonModule
 - cd beagleboneblack_video
 - cd leptonSDKEmb32PUB
 - make
 - cd ..
 - qmake && make
 - ./bbb
 - Your demo should be running and a video feed should be appearing on your monitor!

10.	Troubleshooting
 - If the demo won't run, disattach and reattach the Lepton.
   - **IMPORTANT NOTE:** Remember to remove power first and then ground to power off. Reattach ground first and then power to power on.

Parts Needed:
 * USB Hub
 * USB Mouse
 * USB Keyboard
 * HDMI Monitor
 * Wires
 * BreadBoard
 * Micro SD Card (with SD card adapter)


*The following are available at www.digikey.com:*

**BeagleBone Black (comes with USB cable)**
 - Order from: http://www.digikey.com/product-detail/en/0/BB-BBLK-000-REVC-ND
 - Part#: BB-BBLK-000-REVC-ND

**Micro HDMI Cable**
 - Order from: http://www.digikey.com/product-detail/en/0687860006/WM1285-ND/2330698
 - Part#: WM1285-ND

**Power Supply**
 - Order from: http://www.digikey.com/product-detail/en/WSU050-2000/237-1385-ND/3094911
 - Part#: 237-1385-ND

**Flir Lepton**
 - Order from: http://www.digikey.com/product-search/en/sensors-transducers/image-sensors-camera/1966754?k=lepton

**Lepton Breakout Board**
 - Order from:  http://www.digikey.com/product-search/en/prototyping-products/adapter-breakout-boards/2360393?k=lepton
 - Part#: 250-0587-00-ND