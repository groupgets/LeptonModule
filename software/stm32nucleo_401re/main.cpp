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


#include "mbed.h"

Serial pc(SERIAL_TX, SERIAL_RX);
SPI lepton_spi(SPI_MOSI, SPI_MISO, SPI_SCK);
DigitalOut spi_cs(SPI_CS);

#define VOSPI_FRAME_SIZE (164) 
uint8_t lepton_frame_packet[VOSPI_FRAME_SIZE]; 
int lepton_image[80][80];


int print_image_binary_state =-1;
int print_image_binary_i;
int print_image_binary_j;

static void print_image_binary_background(void)
{
	if( print_image_binary_state == -1)
	{
		return;
	}
	else if( print_image_binary_state == 0)
	{
		pc.putc(0xDE);
		print_image_binary_state++;
	} 
	else if( print_image_binary_state == 1)
	{
		pc.putc(0xAD);
		print_image_binary_state++;
	} 
	else if( print_image_binary_state == 2)
	{
		pc.putc(0xBE);
		print_image_binary_state++;
	} 
	else if( print_image_binary_state == 3)
	{
		pc.putc(0xEF);
		print_image_binary_state++;
		print_image_binary_i = 0;
		print_image_binary_j = 0;
	} 
	else if( print_image_binary_state == 4)
	{
		while(pc.writeable() == 0);
		pc.putc((lepton_image[print_image_binary_i][print_image_binary_j]>>8)&0xff);
		while(pc.writeable() == 0);
		pc.putc(lepton_image[print_image_binary_i][print_image_binary_j]&0xff);

		print_image_binary_j++;
		if(print_image_binary_j>=80)
		{
			print_image_binary_j=0;
			print_image_binary_i++;
			if(print_image_binary_i>=60)
			{
				print_image_binary_state = -1;
			}
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

void transfer(void)
{
	int i;
	int frame_number;


	spi_cs = 0;
	for(i=0;i<VOSPI_FRAME_SIZE;i++)
	{
		lepton_frame_packet[i] = lepton_spi.write(0x00);
	}
	spi_cs = 1;


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
			if(print_image_binary_state == -1)
			{
				for(i=0;i<80;i++)
				{
					lepton_image[frame_number][i] = (lepton_frame_packet[2*i+4] << 8 | lepton_frame_packet[2*i+5]);
				}
			}
		}
		else
		{
			lost_frame_counter++;
		}
		if( frame_number == 59)
		{
			frame_complete = 1;
			last_frame_number = 0;
		}
	}
	else
	{
		if(last_frame_number ==0)
		{
		}
	}

	lost_frame_counter++;
	if(lost_frame_counter>100)
	{
		need_resync = 1;
		lost_frame_counter = 0;

	}

	if(need_resync)
	{
		wait_ms(185);
		need_resync = 0;
	}


	if(frame_complete)
	{
		if(new_frame)
		{
			frame_counter++;
			{
				if(frame_counter%18 ==0)
				{
					print_image_binary_state = 0;
				}
			}
			new_frame = 0;
		}
		frame_complete = 0;
	}
}


int main() 
{
	pc.baud(115200);
	lepton_spi.format(8,3);
	lepton_spi.frequency(20000000);
	spi_cs = 1;
	spi_cs = 0;
	spi_cs = 1;

	wait_ms(185);

	while(1) 
	{ 
		transfer();
		print_image_binary_background();
	}
}

