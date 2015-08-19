Links

Compiler
https://developer.mbed.org/


ST-LINK  used for downloading firmware file to board. 
http://www.st.com/web/en/catalog/tools/PF258168

driver
http://www.st.com/web/catalog/tools/FM147/SC1887/PF260219

How to use ST-Link
http://www.st.com/st-web-ui/static/active/en/resource/technical/document/user_manual/CD00262073.pdf


workflow:
create an mbed account, start a nucleo_401 project then copy and paste the main.cpp code into it.
compile the code, it will will be a downloaded file. The 401 code can be used on the 411 board as well. 
Use ST-link to flash the code to the nucleo board

The Data comes out the serial port, you can use the device manager to find the correct serial port. 
The lepton and breakout board can be pluged directly into the nucleo board. 
any serial program can be used to view the raw data to verify that it is working. 
An example program to convert the raw data stream is called ThermalView. This is a window only program. 
