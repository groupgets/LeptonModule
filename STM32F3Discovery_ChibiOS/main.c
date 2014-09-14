/*
Copyright (c) 2014, Pure Engineering LLC
All rights reserved.

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

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "spi.h"
#include "i2c.h"
#include "usbcfg.h"
#include "chprintf.h"

#define usb_lld_connect_bus(usbp)
#define usb_lld_disconnect_bus(usbp)

SerialUSBDriver SDU1;

#define   SPI_DIV2                  0
#define   SPI_DIV4                  (SPI_CR1_BR_0)
#define   SPI_DIV8                  (SPI_CR1_BR_1)
#define   SPI_DIV16                  (SPI_CR1_BR_1 | SPI_CR1_BR_0)
#define   SPI_DIV32                  (SPI_CR1_BR_2)
#define   SPI_DIV64                  (SPI_CR1_BR_2 | SPI_CR1_BR_0)

#define VOSPI_FRAME_SIZE (164)
uint8_t lepton_frame_packet[VOSPI_FRAME_SIZE];
uint8_t tx[VOSPI_FRAME_SIZE] = {0, };
static int lepton_image[80][80];
static const SPIConfig spi1cfg = {
	NULL,
	GPIOA,
	GPIOA_PIN4,
	SPI_DIV16 | SPI_CR1_CPOL | SPI_CR1_CPHA,   
	0
};

static void print_image_ascii(void)
{
	int i;
	int j;

	chprintf((BaseSequentialStream *)&SDU1, "P2\n80\n60\n255\n");
	for(i=0;i<60;i++)
	{
		for(j=0;j<80;j++)
		{
			chprintf((BaseSequentialStream *)&SDU1, "%d ", lepton_image[i][j]);
		}
		chprintf((BaseSequentialStream *)&SDU1, "\n");
	}
	chprintf((BaseSequentialStream *)&SDU1, "\n\n");
}

static void print_image_binary(void)
{
	int i;
	int j;

	chSequentialStreamPut((BaseSequentialStream *)&SDU1,0xDE);
	chSequentialStreamPut((BaseSequentialStream *)&SDU1,0xAD);
	chSequentialStreamPut((BaseSequentialStream *)&SDU1,0xBE);
	chSequentialStreamPut((BaseSequentialStream *)&SDU1,0xEF);

	for(i=0;i<60;i++)
	{
		for(j=0;j<80;j++)
		{
			chSequentialStreamPut((BaseSequentialStream *)&SDU1,(lepton_image[i][j]>>8)&0xff);
			chSequentialStreamPut((BaseSequentialStream *)&SDU1,lepton_image[i][j]&0xff);
		}
	}
}

int lost_frame_counter = 0;
int last_frame_number;
int frame_complete = 0;
int start_image = 0;
int need_resync = 0;
int last_crc;
int new_frame = 0;
int frame_counter = 0;

static void transfer(void)
{
	int i;
	int frame_number;

	spiSelect(&SPID1);
	spiExchange(&SPID1, VOSPI_FRAME_SIZE, tx, lepton_frame_packet);
	spiUnselect(&SPID1);


	if(((lepton_frame_packet[0]&0xf) != 0x0f))
	{
		if(lepton_frame_packet[1] == 0  )
		{
			if(last_crc != (lepton_frame_packet[3]<<8 | lepton_frame_packet[4]))
			{
				new_frame = 1;
			}
			last_crc = lepton_frame_packet[3]<<8 | lepton_frame_packet[4];
		}
		frame_number = lepton_frame_packet[1];

		if(frame_number < 60 )
		{
			lost_frame_counter = 0;
			for(i=0;i<80;i++)
			{
				lepton_image[frame_number][i] = (lepton_frame_packet[2*i+4] << 8 | lepton_frame_packet[2*i+5]);
			}
		}
		if( frame_number == 59)
		{
			frame_complete = 1;
			last_frame_number = 0;
		}
	}

	lost_frame_counter++;
	if(lost_frame_counter>1000)
	{
		need_resync = 1;
		lost_frame_counter = 0;

	}

	if(need_resync)
	{
		chprintf((BaseSequentialStream *)&SDU1, "resync\n\r");
		chThdSleepMilliseconds(185);
		need_resync = 0;
	}


	if(frame_complete)
	{
		if(new_frame)
		{
			frame_counter++;

			{
				print_image_binary();
			}
			new_frame = 0;
		}
		frame_complete = 0;
	}
}

static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

	(void)arg;
	chRegSetThreadName("blinker");
	while (TRUE) {

		palSetPad(GPIOE, GPIOE_LED3_RED);
		chThdSleepMilliseconds(125);
		palClearPad(GPIOE, GPIOE_LED3_RED);
		palSetPad(GPIOE, GPIOE_LED5_ORANGE);
		chThdSleepMilliseconds(125);
		palClearPad(GPIOE, GPIOE_LED5_ORANGE);
		palSetPad(GPIOE, GPIOE_LED7_GREEN);
		chThdSleepMilliseconds(125);
		palClearPad(GPIOE, GPIOE_LED7_GREEN);
		palSetPad(GPIOE, GPIOE_LED9_BLUE);
		chThdSleepMilliseconds(125);
		palClearPad(GPIOE, GPIOE_LED9_BLUE);
		palSetPad(GPIOE, GPIOE_LED10_RED);
		chThdSleepMilliseconds(125);
		palClearPad(GPIOE, GPIOE_LED10_RED);
		palSetPad(GPIOE, GPIOE_LED8_ORANGE);
		chThdSleepMilliseconds(125);
		palClearPad(GPIOE, GPIOE_LED8_ORANGE);
		palSetPad(GPIOE, GPIOE_LED6_GREEN);
		chThdSleepMilliseconds(125);
		palClearPad(GPIOE, GPIOE_LED6_GREEN);
		palSetPad(GPIOE, GPIOE_LED4_BLUE);
		chThdSleepMilliseconds(125);
		palClearPad(GPIOE, GPIOE_LED4_BLUE);
	}
	return 0;
}

static WORKING_AREA(waThread2, 512);
static msg_t Thread2(void *arg) {

	(void)arg;
	uint8_t rxchar;
	chRegSetThreadName("print");


	while (TRUE) {

		if(chSequentialStreamRead((BaseSequentialStream *)&SDU1, &rxchar,1 ) )
		{
			if(rxchar == 'b')
			{
				print_image_binary();
			}
			else if(rxchar == 'a')
			{
				print_image_ascii();
			}
			else if(rxchar == 'r')
			{
				chprintf((BaseSequentialStream *)&SDU1, "manual resync\n\r");
				chThdSleepMilliseconds(185);
			}
			else
			{
				chprintf((BaseSequentialStream *)&SDU1, "unknown key=%d\n\r",rxchar);
			}
		}
		chThdSleepMilliseconds(1);

	}
	return 0;
}

int main(void) {

	halInit();
	chSysInit();

	sduObjectInit(&SDU1);
	sduStart(&SDU1, &serusbcfg);

	chThdSleepMilliseconds(100);
	usbDisconnectBus(serusbcfg.usbp);
	chThdSleepMilliseconds(1000);
	usbStart(serusbcfg.usbp, &usbcfg);
	usbConnectBus(serusbcfg.usbp);

	chThdSleepMilliseconds(1000);
	
	spiStart(&SPID1, &spi1cfg);
	palSetPadMode(GPIOA, 4, PAL_MODE_OUTPUT_PUSHPULL); 

	chThdCreateStatic(waThread1, sizeof(waThread1), LOWPRIO, Thread1, NULL);
	chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

	while (TRUE) 
	{
		transfer();
	}
}

