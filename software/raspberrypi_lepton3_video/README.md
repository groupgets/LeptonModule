This example is meant for Raspberry Pi and Pi2 and has been tested with Raspbian.

Enable the SPI and I2C interfaces on the Pi.

Install the 'qt4-dev-tools' package, which allows compiling of QT applications.

To build (will build any SDK dependencies as well):
qmake && make

To clean:
make sdkclean && make distclean

If you wish to run this application without using sudo, you should add the user "pi" to the usergroup "i2c".

----

In order for the application to run properly, a Lepton camera must be attached in a specific way to the SPI, power, and ground pins of the Raspi's GPIO interface, as well as the I2C SDA/SCL pins:

Lepton's GND pin should be connected to RasPi's ground.
Lepton's CS pin should be connected to RasPi's CE1 pin.
Lepton's MISO pin should be connected to RasPI's MISO pin.
Lepton's CLK pin should be connected to RasPI's SCLK pin.
Lepton's VIN pin should be connected to RasPI's 3v3 pin.
Lepton's SDA pin should be connected to RasPI's SDA pin.
Lepton's SCL pin should be connected to RasPI's SCL pin.

