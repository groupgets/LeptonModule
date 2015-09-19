/*
Copyright (c) 2014, C. Gyger, S. Raible
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

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<mraa.h>

void sig_handler(int signum);
void save_pgm_file();

static sig_atomic_t volatile isrunning = 1;
static unsigned int image[80*60];

main(int argc, char **argv)
{
	signal(SIGINT, &sig_handler);
	
	// init
	mraa_init();
	
	mraa_gpio_context cs = mraa_gpio_init(10);	
	mraa_gpio_dir(cs, MRAA_GPIO_OUT);

	mraa_spi_context spi = mraa_spi_init(5);	// bus 5 for edison
	mraa_spi_mode(spi, MRAA_SPI_MODE3);	
	mraa_spi_frequency(spi, 6250000);
	mraa_spi_lsbmode(spi, 0);
	mraa_spi_bit_per_word(spi, 8);
		
	uint8_t payload[164];
	uint8_t *recv = NULL;
	int packetNb;
	int i;
	uint8_t checkByte = 0x0f;
	
	printf("\nStarting Lepton Test\n");
	
	//sync	
	mraa_gpio_write(cs, 1);	// high to de-select chip
	usleep(200000);
	mraa_gpio_write(cs, 0);	// low to select chip
	
	// loop while discard packets		
	while((checkByte & 0x0f) == 0x0f && isrunning){
		if(recv)
			free(recv);
		recv = mraa_spi_write_buf(spi, payload, 164);
		checkByte = recv[0];
		packetNb = recv[1];
	}	
		
	// sync done, first packet is ready, store packets
	while(packetNb < 60 && isrunning)
	{	
		// ignore discard packets
		if((recv[0] & 0x0f) != 0x0f){			
			for(i=0;i<80;i++)
			{
				image[packetNb * 80 + i] = (recv[2*i+4] << 8 | recv[2*i+5]);
			}			
		}
		
		// read next packet
		if(recv)
			free(recv);
		recv = mraa_spi_write_buf(spi, payload, 164);
		packetNb = recv[1];
	}
	
	printf("\nFrame received, storing PGM file\n");
	save_pgm_file();
	
	// de-init	
	fprintf(stdout, "\nEnding, set CS = 0\n");
	mraa_gpio_write(cs, 0);	
	mraa_gpio_close(cs);
	
	fprintf(stdout, "\nDone\n");
	
	return MRAA_SUCCESS;	
}

void save_pgm_file()
{
	int i;
	int j;
	unsigned int maxval = 0;
	unsigned int minval = 100000;
	FILE *f = fopen("image.pgm", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	printf("\nCalculating min/max values for proper scaling...\n");
	for(i=0;i<60;i++)
	{
		for(j=0;j<80;j++)
		{
			if (image[i * 80 + j] > maxval) {
				maxval = image[i * 80 + j];
			}
			if (image[i * 80 + j] < minval) {
				minval = image[i * 80 + j];
			}
		}
	}
	printf("maxval = %u\n",maxval);
	printf("minval = %u\n",minval);
	fprintf(f,"P2\n80 60\n%u\n",maxval-minval);
	for(i=0;i<60;i++)
	{
		for(j=0;j<80;j++)
		{
			fprintf(f,"%d ", image[i * 80 + j] - minval);
		}
		fprintf(f,"\n");
	}
	fprintf(f,"\n\n");
	fclose(f);
}

void sig_handler(int signum)
{
	if(signum == SIGINT)
		isrunning = 0;
}
