Released under GPLv3

This is a simple program initially designed for Raspberry Pi that reads frames from the FLIR Lepton and streams them to a v4l2loopback device. This program would be very useful if you want to stream the Lepton like any other webcam. Maybe you want to process the Lepton frames using a library that can read standard v4l2 devices, like OpenCV. Anything you can do with a webcam, this program will allow you to do it with the Lepton + basic breakout board.

Code is borrowed from raspberrypi_video as well as the ondemandcam example provided with v4l2loopback.

Install v4l2loopback kernel module to a Raspberry Pi:

1. Install kernel headers with [rpi-source](https://github.com/notro/rpi-source) utility.
2. Compile [v4l2loopback](https://github.com/umlaeute/v4l2loopback)
3. If step 2 is successful, you should have v4l2loopback.ko in the source directory
4. Copy v4l2loopback.ko to /lib/modules (I believe just /lib/modules directory works, but if that doesn't work, put it in /lib/modules/$(uname -r) OR just put it in both places...).
5. Run `sudo depmod -a`

At this point, you should be able to modprobe v4l2loopback. This will create a new /dev/video device. It will be /dev/video0 if you have no other cameras attached. If you have other cameras, it will likely be the highest numbered /dev/video. To be sure, you can pass options to modprobe that will set it to a specific number (see v4l2loopback docs). You could also identify the proper video device with `v4l2-ctl`. 

Below, I refer to the new v4l2loopback device as `/dev/videoX` but you should replace X with the number on your system.

To Compile v4l2lepton:
    
    make
    

To Run v4l2lepton:

    ./v4l2lepton -v /dev/videoX -d /dev/spidevY.Z

If spidev device is not given /dev/spidev0.1 will be used by default.  
You can confirm that the stream is working by using something like VLC Media Player and opening /dev/videoX as a capture device. Anything that can interface with v4l2 devices can use it though.
