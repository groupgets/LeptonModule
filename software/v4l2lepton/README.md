Released under GPLv3

This is a simple program designed for Raspberry Pi that reads frames from the FLIR Lepton and streams them to a v4l2loopback device. 
Code is borrowed from raspberrypi_video as well as the ondemandcam example provided with v4l2loopback.

Install v4l2loopback kernel module to a Raspberry Pi:

1. Install kernel headers with [rpi-source](https://github.com/notro/rpi-source) utility.
2. Compile [v4l2loopback](https://github.com/umlaeute/v4l2loopback)
3. If step 2 is successful, you should have v4l2loopback.ko in the source directory
4. Copy v4l2loopback.ko to /lib/modules (I believe just /lib/modules directory works, but if that doesn't work, put it in /lib/modules/$(uname -r) ).
5. You might find insmod works too, I just copied the file like step 4 says
6. At this point, you should be able to modprobe v4l2loopback

To Compile v4l2lepton:
    
    make
    

To Run v4l2lepton:

    ./v4l2lepton /dev/videoX
