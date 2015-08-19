STM32F3Discovery_ChibiOS
-----------
Download and install ChibiOS_2.6.5 into the same directory first. 
This example takes the SPI data stream from the Lepton module, buffers it and send it out the USB VCP device. On the PC the binary stream of data can be parsed easily by looking for the 0xdeadbeef header on each frame.

