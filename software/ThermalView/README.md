ThermalVew is a program used to read seial data from the stm32nucleo_401re example code. 

The program uses the MingGW toolchain for compiling the code. You can obtain a suitable toolchain from http://www.mingw.org/ or http://tdm-gcc.tdragon.net/ - install the 32 bit version for this purpose.

Install the toolchain and add the bin directory to your path, e.g., PATH=C:\MinGW\bin;%PATH%

Run Make (mingw32-make.exe) to build the code. Use the "clean" target to clean before building.

The program has a hardcoded serial (COM) port - you will need to update this accordingly before compiling.

