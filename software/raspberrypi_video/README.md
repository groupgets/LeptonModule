This example is meant for Raspberry Pi, Pi2, Pi3, PiZero, & Pi4 and has been tested with Raspbian.

First enable the SPI and I2C interfaces on the Pi.
```
sudo raspi-config
```

Install the 'qt4-dev-tools' package, which allows compiling of QT applications.
```
sudo apt-get install qt4-dev-tools
```

To build (will build any SDK dependencies as well, cd to the *LeptonModule/sofware/raspberrypi_video* folder, then run:
```
qmake && make
```

To clean:
```
make sdkclean && make distclean
```

## Raspberry Pi 1,2,3, & Zero

### Lepton 2.x
To run the program while still in the raspberrypi_video directory, using a FLIR Lepton 2.x camera core use the following code:
```
./raspberrypi_video
```

### Lepton 3.x
To run the program while still in the raspberrypi_video directory, using a FLIR Lepton 3.x camera core use the following code:
```
./raspberrypi_video -tl 3
```

## Raspberry Pi 4

### Lepton 2.x
To run the program while still in the raspberrypi_video directory, using a FLIR Lepton 2.x camera core first use the following code:
```
sudo sh -c "echo performance > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor"
```
Then run this to start the program:
```
./raspberrypi_video
```

### Lepton 3.x
To run the program while still in the raspberrypi_video directory, using a FLIR Lepton 3.x camera core first use the following code:
```
sudo sh -c "echo performance > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor"
```
Then run this to start the program:
```
./raspberrypi_video -tl 3
```

----

In order for the application to run properly, a Lepton camera must be attached in a specific way to the SPI, power, and ground pins of the Raspi's GPIO interface, as well as the I2C SDA/SCL pins:

Lepton's GND pin should be connected to RasPi's ground.
Lepton's CS pin should be connected to RasPi's CE1 pin.
Lepton's MISO pin should be connected to RasPI's MISO pin.
Lepton's CLK pin should be connected to RasPI's SCLK pin.
Lepton's VIN pin should be connected to RasPI's 3v3 pin.
Lepton's SDA pin should be connected to RasPI's SDA pin.
Lepton's SCL pin should be connected to RasPI's SCL pin.
