#include "leptoncontrol.h"
#include <unistd.h>

static unsigned short RAW14Image[LEPTON_ORIGINAL_IMAGE_HEIGHT*LEPTON_ORIGINAL_IMAGE_WIDTH];//собираемое изображение

#define RESYNC_TIMEOUT 19890
#define NO_SEGMENT -1
#define ERROR_PACKAGE -2

//----------------------------------------------------------------------------------------------------
//инициализация
//----------------------------------------------------------------------------------------------------
void LEPTONCONTROL_Init(void)
{ 
}

//----------------------------------------------------------------------------------------------------
//вычислить crc
//----------------------------------------------------------------------------------------------------
void LEPTONCONTROL_CalculateCRC(unsigned short *crc,unsigned char byte)
{
 (*crc)^=(byte<<8);
 for(unsigned char n=0;n<8;n++) 
 { 
  if ((*crc)&0x8000) *crc=((*crc)<<1)^0x1021;
                else (*crc)<<=1;
 }
}


//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
long LEPTONCONTROL_ReadSegment(unsigned short *raw14_ptr,unsigned char data[VOSPI_PACKAGE_SIZE],bool &first_line)
{
 static long current_package=-1;
 static long segment=-1;
 long n;
 first_line=false;
 if ((data[0]&0x0F)==0x0F) return(NO_SEGMENT);//отбрасываемый пакет
 unsigned short crc=data[2];
 crc<<=8;
 crc|=data[3];
 //считаем CRC
 unsigned short crc16=0;
 LEPTONCONTROL_CalculateCRC(&crc16,data[0]&0x0F);
 LEPTONCONTROL_CalculateCRC(&crc16,data[1]);
 LEPTONCONTROL_CalculateCRC(&crc16,0);
 LEPTONCONTROL_CalculateCRC(&crc16,0);
 for(n=4;n<VOSPI_PACKAGE_SIZE;n++) LEPTONCONTROL_CalculateCRC(&crc16,data[n]);
 if (crc16!=crc) return(ERROR_PACKAGE);//ошибка CRC
 //определяем номер пакета
 unsigned short package=data[0]&0x0F;
 package<<=8;
 package|=data[1];
 if (package==0)
 {
  first_line=true;
  current_package=0;
 }
 if (package==20)
 {
  unsigned char ttt=(data[0]&0x70)>>4;//номер кадра бывает только в 20 пакете
  segment=ttt;
 }
 if (current_package<0) return(NO_SEGMENT);
 if (current_package!=package)
 { 
  current_package=-1;
  return(ERROR_PACKAGE);
 }
 unsigned short *raw_ptr=raw14_ptr+current_package*VOSPI_PACKAGE_LINE_SIZE/2;
 for(n=0;n<VOSPI_PACKAGE_LINE_SIZE/2;n++,raw_ptr++)
 {
  //байты заданы в порядке big-endian: старший, младший
  unsigned short value=data[n*sizeof(short)+4];
  value<<=8;
  value|=data[n*sizeof(short)+5];
  *raw_ptr=value;
 }
 current_package++;
 if (current_package!=VOSPI_FRAME_HEIGHT) return(NO_SEGMENT);
 current_package=-1;
 return(segment);
}

//----------------------------------------------------------------------------------------------------
//подать данные одного пакета VoSPI на вход модуля
//----------------------------------------------------------------------------------------------------
bool LEPTONCONTROL_PushVoSPI(unsigned char data[VOSPI_PACKAGE_SIZE],bool &first_line)
{
 first_line=false;
 static long waitable_segment=1;
 long segment=LEPTONCONTROL_ReadSegment(RAW14Image+(waitable_segment-1)*VOSPI_FRAME_WIDTH*VOSPI_SEGMENT_LINE_AMOUNT,data,first_line);
 if (segment==ERROR_PACKAGE) usleep(RESYNC_TIMEOUT);
 if (segment==ERROR_PACKAGE || segment==0) waitable_segment=1;
 if (segment==ERROR_PACKAGE || segment==NO_SEGMENT || segment==0) return(false);
 
 if (segment!=waitable_segment)
 {
  waitable_segment=1;
  if (segment!=1) return(false);
 }
 waitable_segment++;
 if (waitable_segment!=5) return(false);
 waitable_segment=1; 
 return(true);
}
//----------------------------------------------------------------------------------------------------
//получить указатель на данные собранного изображения
//----------------------------------------------------------------------------------------------------
unsigned short *LEPTONCONTROL_GetRAW14Ptr(void)
{
 return(RAW14Image);
}

/*
 long n;
 if ((data[0]&0x0F)==0x0F) return(false);//отбрасываемый пакет
 unsigned short crc=data[2];
 crc<<=8;
 crc|=data[3];
 //считаем CRC
 unsigned short crc16=0;
 LEPTONCONTROL_CalculateCRC(&crc16,data[0]&0x0F);
 LEPTONCONTROL_CalculateCRC(&crc16,data[1]);
 LEPTONCONTROL_CalculateCRC(&crc16,0);
 LEPTONCONTROL_CalculateCRC(&crc16,0);
 for(n=4;n<VOSPI_PACKAGE_SIZE;n++) LEPTONCONTROL_CalculateCRC(&crc16,data[n]);
 if (crc16!=crc) return(false);//ошибка CRC

 //определяем номер пакета
 unsigned short package=data[0]&0x0F;
 package<<=8;
 package|=data[1];
 if (package==0) first_line=true;
 if (package==20)
 {
  unsigned char ttt=(data[0]&0x70)>>4;//номер кадра бывает только в 20 пакете
  if (ttt==0)
  {
   CurrentPackage=0;
   CurrentSegment=0;
   return(false);//любой сегмент с нулевым ttt должен быть проигнорирован (в документации ошибка?)
  }
  //собираем изображение
  ttt--;
  if (CurrentSegment!=ttt)//номер кадра неверный
  {
   CurrentPackage=0;
   CurrentSegment=0;
   return(false);
  }
 }
 if (CurrentPackage!=package)//номер строки неверный
 {
  CurrentPackage=0;
  CurrentSegment=0;
  return(false);
 }
 unsigned long sub_package=(CurrentSegment*VOSPI_FRAME_HEIGHT)+CurrentPackage;
 unsigned short *raw_ptr=&(RAW14Image[(sub_package>>1)*LEPTON_ORIGINAL_IMAGE_WIDTH+(sub_package&0x01)*VOSPI_FRAME_WIDTH]);
 for(n=0;n<VOSPI_PACKAGE_LINE_SIZE/2;n++,raw_ptr++)
 {
  //байты заданы в порядке big-endian: старший, младший
  unsigned short value=data[n*sizeof(short)+4];
  value<<=8;
  value|=data[n*sizeof(short)+5];
  *raw_ptr=value;
 }
 CurrentPackage++;
 if (package==VOSPI_FRAME_HEIGHT-1)//сегмент закончился
 {
  CurrentSegment++;//переходим к следующему сегменту
  CurrentPackage=0;
 }
 if (CurrentSegment==4)//всё изображение собрано
 {
  CurrentPackage=0;
  CurrentSegment=0;
  return(true);
 }
 return(false);

*/