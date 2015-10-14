Released under GPLv3

This is a simple program designed for Raspberry Pi that reads frames from the FLIR Lepton and streams them to a v4l2loopback device. 
Code is borrowed from raspberrypi_video as well as the ondemandcam example provided with v4l2loopback.

To Compile:
    
    make
    

To Run:

    ./v4l2lepton /dev/videoX
