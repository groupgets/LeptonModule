#include "LeptonThread.h"

#include "Palettes.h"
#include "SPI.h"
#include "Lepton_I2C.h"
#include <stdio.h>
#include <stdlib.h>

#define FPS 27;

#define SPI_PORT_INDEX 1
#define SPI_PORT_HANDLE spi_cs1_fd
#define SPI_READ_VOSPI_AMOUNT 59

LeptonThread::LeptonThread():QThread()
{
 //ôîðìèðóåì ïàëèòðó ïî-óìîë÷àíèþ
 for(long n=0;n<256;n++)
 {
  ColorMap_R[n]=n;
  ColorMap_G[n]=n;
  ColorMap_B[n]=n;
 } 
}

LeptonThread::~LeptonThread() 
{
}

//----------------------------------------------------------------------------------------------------
//çàãðóçèòü ïàëèòðó
//----------------------------------------------------------------------------------------------------
bool LeptonThread::LoadPalette(const char *file_name)
{
 FILE *file=fopen(file_name,"rb");
 if (file==NULL) return(false);
 for(long n=0;n<256;n++)
 {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  if (fread(&r,sizeof(unsigned char),1,file)<=0) break;
  if (fread(&g,sizeof(unsigned char),1,file)<=0) break;
  if (fread(&b,sizeof(unsigned char),1,file)<=0) break;
  ColorMap_R[n]=r;
  ColorMap_G[n]=g;
  ColorMap_B[n]=b;
 }
 fclose(file);
 return(true);
}

void LeptonThread::CreateImage(void)
{
 unsigned short *raw14_ptr=LEPTONCONTROL_GetRAW14Ptr();
 unsigned long min=0x10000;
 unsigned long max=0;
 unsigned long y;
 unsigned long x;
 unsigned short *raw14_local_ptr;
 raw14_local_ptr=raw14_ptr;
 unsigned long width=LEPTON_ORIGINAL_IMAGE_WIDTH;
 unsigned long height=LEPTON_ORIGINAL_IMAGE_HEIGHT;
 for(y=0;y<height;y++)
 {
  for(x=0;x<width;x++,raw14_local_ptr++)
  {
   unsigned short raw14=*raw14_local_ptr;
   if (raw14>max) max=raw14;
   if (raw14<min) min=raw14;
  }
 }
 long delta=max-min;
 if (delta==0) delta=1;
 raw14_local_ptr=raw14_ptr;
 QRgb color;
 for(y=0;y<height;y++)
 {
  for(x=0;x<width;x++,raw14_local_ptr++)
  {
   long value=*raw14_local_ptr;
   value-=min;
   value=(value*255)/delta;
   if (value>255) value=255;
   if (value<0) value=0;
   color=qRgb(ColorMap_R[value],ColorMap_G[value],ColorMap_B[value]);
   myImage.setPixel(x,y,color);
  }
 }
}

void LeptonThread::run()
{
 LEPTONCONTROL_Init();
 //çàãðóæàåì ïàëèòðó
 LoadPalette("./Iron.pal");
 //create the initial image
 myImage=QImage(LEPTON_ORIGINAL_IMAGE_WIDTH,LEPTON_ORIGINAL_IMAGE_HEIGHT,QImage::Format_RGB888);
 unsigned char buffer[VOSPI_PACKAGE_SIZE*SPI_READ_VOSPI_AMOUNT];
 //open spi port
 SpiOpenPort(SPI_PORT_INDEX);
 while(1) 
 {
  while(1)
  {
   read(SPI_PORT_HANDLE,buffer,VOSPI_PACKAGE_SIZE); 
   bool first_line=false;
   unsigned char *buffer_ptr=buffer;
   LEPTONCONTROL_PushVoSPI(buffer_ptr,first_line);
   if (first_line==true) break;
  }
  //read data packets from lepton over SPI
  read(SPI_PORT_HANDLE,buffer,VOSPI_PACKAGE_SIZE*SPI_READ_VOSPI_AMOUNT);
  unsigned char *buffer_ptr=buffer;
  for(long n=0;n<SPI_READ_VOSPI_AMOUNT;n++,buffer_ptr+=VOSPI_PACKAGE_SIZE) 
  {
   bool first_line=false;
   bool res=LEPTONCONTROL_PushVoSPI(buffer_ptr,first_line);
   if (res==true) 
   {
    CreateImage();
    //lets emit the signal for update
    emit updateImage(myImage);
   }
  }
 }
 //finally, close SPI port just bcuz
 SpiClosePort(SPI_PORT_INDEX);
}

void LeptonThread::performFFC() 
{
 //perform FFC
 lepton_perform_ffc();
}
