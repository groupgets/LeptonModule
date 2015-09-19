This example is meant for Raspberry Pi and Pi2 and has been tested with Raspbian.

Enable the SPI and I2C interfaces on the Pi.

Install the 'qt4-dev-tools' package, which allows compiling of QT applications.

To build (will build any SDK dependencies as well):
qmake && make

To clean:
make sdkclean && make distclean

If you wish to run this application without using sudo, you should add the user "pi" to the usergroup "i2c".

----

This is a fairly minimal (but functional) live view/snapshot application for the Raspberry PI + FLIR Lepton module.  We had issues with some examples given that used the SDK, so we touched up the video program and did all of the SPI stuff without the SDK.  No extra scripts, no extra resistors, etc. (all of these we tried first, but they didn't help)!

The program runs a thread to pull data from the Lepton and signals when a fullimage is ready; the main window updates the image and stores both the RGB and raw (16-bit thermal) image, which can be saved to disk by clicking the Snapshot button at any time.  The files created are numbered sequentially per run:

  * rgb?.jpg contains a 80x60 rgb image, and
  * raw?.bin contains the raw data;
    it consists of 9604 bytes (4802 unsigned shorts), as follows:
      - Word 0: minimum value from the scene
      - Word 1: maximum value from the scene
      - The rest are 80x60 unsigned shorts, listed by row then column.

The project is set so that it can be worked with on a laptop (without Lepton) for
development, controlled by the #define's at the top of LeptonThread.h:

  * HAVE_LEPTON: when true, uses ioctl to pull data packets from the Lepton;
                 when false, a moving pattern is simulated instead.
  * DEBUG_LEPTON: when true, with each full frame is reported [via qDebug()]
                  the full sequence of packets that were read while pulling the image;
                  when everything is working properly, this typically consists of
                  some small number of *'s (bad packets) followed by 0-59.

Note that the SPI device is hardcoded as 0.0---change this to 0.1 if necessary!

Enjoy!

Frank Swenton, Middlebury College
fswenton@middlebury.edu

