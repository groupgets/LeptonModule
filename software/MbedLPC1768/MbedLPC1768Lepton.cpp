/*
Copyright (c) 2015, Josh Elsdon

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "mbed.h"
#include "MODSERIAL.h"
MODSERIAL pc(USBTX, USBRX, 1024,256);
SPI lepton_spi(p5, p6, p7);
DigitalOut spi_cs(p8);

#define VOSPI_FRAME_SIZE (164)

uint8_t image0[60][160];
uint8_t image1[60][160];

DigitalOut led(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
int frameAvailable = 0;
int bufferToReadFrom = 1;
int imageReading = 0;
int packet_number = 0;

Ticker checkCam; // interval timer to check the cam.

//ok: the plan there will be 2 buffers that are ping ponged
// one recieving the new data while the other one is outputted
// they then can swap at the end of the output.
// The high priority process is reading the lepton.
// The readout will be done as a background process.
// The lepton will be read continuously till there are scrap packets.
// a timer will make the check for new packets at 50%of the fps.(27).
//This will run at 5fps on LPC1768 mbed, TODO: use DMA to get it to 9Hz
//This will work well on any mbed with changes to the pin assignments above.


void LeptonCheck()
{
    led = 1;
    led2 = 0;
    bool junk = 0;
    while(!junk) {
        uint8_t header[4];
        spi_cs = 0;
        //check the header for junk marker, also take packet count.
        for(int i=0; i<4; i++) {
            header[i] = lepton_spi.write(0x00);
        }

        //decide junk
        if (header[0] &0xf == 0x0f) {
            junk = true;
        } else {
            junk = false;
        }

        if(!junk) {
            imageReading =1;
            packet_number = header[1];
        }


        if(bufferToReadFrom == 1) { //choose buffer to right in.
            for(int i=0; i<VOSPI_FRAME_SIZE-4; i++) {
                if(!junk) {
                    image0[packet_number][i] = lepton_spi.write(0x00);
                } else {
                    uint8_t waste = lepton_spi.write(0x00);// waste these they are junk
                }
            }
        } else {
            for(int i=0; i<VOSPI_FRAME_SIZE-4; i++) {
                if(!junk) {
                    image1[packet_number][i] = lepton_spi.write(0x00);
                } else {
                    uint8_t waste = lepton_spi.write(0x00);
                }
            }
        }
        spi_cs = 1;
        if(packet_number == 59 && !junk) {
            imageReading = 0;
            frameAvailable = 1;
        }
    }
    led = 0;
    led2 = 1;

}

void sendPic(int bufferNo)
{

    // tag the beginning of an image with 'deadbeef'
    pc.putc(0xDE);
    pc.putc(0xAD);
    pc.putc(0xBE);
    pc.putc(0xEF);
    if(bufferNo == 0) {//use the buffer that is not being written to.
        for(int i = 0; i < 60; i++) {
            for(int j = 0; j < 160; j++) {
                pc.putc(image0[i][j]);
            }
        }
    } else {
        for(int i = 0; i < 60; i++) {
            for(int j = 0; j < 160; j++) {
                pc.putc(image1[i][j]);
            }
        }
    }
}


int main()
{


    pc.baud(921600);
    lepton_spi.format(8,3);
    lepton_spi.frequency(17000000);  //upto 20M when not on a breadboard.
    spi_cs = 1;
    spi_cs = 0;
    spi_cs = 1;
    wait_ms(185);
    checkCam.attach(&LeptonCheck, (1.0/(27*1.6))); //check every half frame.

    while(1) {
        if(frameAvailable) {
            sendPic(bufferToReadFrom);
            while(imageReading) {} // block here till current grab is complete
            bufferToReadFrom = 1-bufferToReadFrom;
        }
    }
}
