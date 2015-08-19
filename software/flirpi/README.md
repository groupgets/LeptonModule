Released under the GPLv3

These are miscellaneous utilities that work with the lepton module on the raspberry pi.

You normally need to use sudo (until I or someone else does V4L2 for the module).

leptsci.c is the code to access the flir module over SCI and fill an image buffer.

(Note: there seems to be an occasional hiccup with the SCI, maybe it is just my jumper cables or something, but it occasionally gets out of sync, so there is some experimental code to address this)

leptcam is work in progress, but using the V4L2 loopback module which must be loaded - it may be in your distro or is avaialble as source at:
	git clone https://github.com/umlaeute/v4l2loopback
This will create a new /dev/videoN device.
it will eventually turn the Lepton into a video-for-linux-2 device.
Right now, when run in the background, e.g.
	leptcam /dev/videoN & # replace N with the V4L2loopback device number
it just displays colored squares with
	vlc v4l2:///dev/videoN # replace N with the V4L2loopback device number

fblept will display a false color image to the framebuffer (command line normally), updating about 10x/sec.  Add "-c" to see a "contour" version that should show more detail in high dynamic range (e.g. a stove and refrigerator).

leptbmp will write out a BMP with HTTP header.  Most desktop browsers understand bmp, so you can animate by just including it via CGI in an <img src="getflir.sh"...> tag and with a meta-refresh or javascript it will continually update

leptgraypng is so you can use ImageMagic, typically convert.  It takes the raw data and writes it to a PNG file with 16 bit grayscale.  If you view it, it will look flat, but "convert x.png -equalize y.png" will show something much more normal.  You can enhance it further.  See http://www.imagemagick.org

I get good images with "./leptgraypng >x.png ; convert x.png -normalize -contrast -sharpen 5 y.png; fbi -a y.png"

The lepton sensor has a lot of range, but if you simply threshold, you won't see much if there is both a hot and cold object in the view.  convert has more optons that can do things like high-pass filtering, so a hot handprint on the wall can be visible at the same time, but it requires some tweaking.  GIMP tends to flatten to 8 bits so can't do much.

