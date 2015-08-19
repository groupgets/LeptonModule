This example is meant for Raspberry PI, and has been tested on Raspberry PI model B, I think.

It has been tested on the OS Raspbian.

In order to build this example, you'll have to first install the qt4-dev-tools package, which allows compiling of QT applications.

You'll also need to enable the SPI and I2C interfaces on the raspi.

If you wish to run this application without using sudo, you should add the user "pi" to the usergroup "i2c".


You'll need to build the Lepton SDK binary in order to issue commands to the lepton over I2C, such as performing flat-field calibration. To do this, cd into LeptonSDKEmb32PUB and run the command: `make`


Once everything's in place, cd into the raspberrypi_video folder, and run the command: `qmake && make`. This will compile the application.

In order for the application to run properly, a lepton camera must be attached in a specific way to the SPI, power, and ground pins of the Raspi's GPIO interface, as well as the I2C SDA/SCL pins.

Lepton's GND pin should be connected to RasPi's ground.
Lepton's CS pin should be connected to RasPi's CE1 pin.
Lepton's MISO pin should be connected to RasPI's MISO pin.
Lepton's CLK pin should be connected to RasPI's SCLK pin.
Lepton's VIN pin should be connected to RasPI's 3v3 pin.
Lepton's SDA pin should be connected to RasPI's SDA pin.
Lepton's SCL pin should be connected to RasPI's SCL pin.
