/*******************************************************************************
**
**    File NAME: jova_I2C.c
**
**      AUTHOR:  Hart Thomson
**
**      CREATED: 9/25/2012
**  
**      DESCRIPTION: Lepton Device-Specific Driver for the JOVA
**                   Master I2C
**
**      HISTORY:  9/25/2012 HT - Initial Draft 
**
** Copyright 2010, 2011, 2012, 2013 FLIR Systems - Commercial Vision Systems
**
**  All rights reserved.
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions are met:
**
**  Redistributions of source code must retain the above copyright notice, this
**  list of conditions and the following disclaimer.
**
**  Redistributions in binary form must reproduce the above copyright notice,
**  this list of conditions and the following disclaimer in the documentation
**  and/or other materials provided with the distribution.
**
**  Neither the name of the Indigo Systems Corporation nor the names of its
**  contributors may be used to endorse or promote products derived from this
**  software without specific prior written permission.
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
**  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
**  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
**  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
**  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
**  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
**  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
**  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
**  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
**  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**  THE POSSIBILITY OF SUCH DAMAGE.
**
*******************************************************************************/
/******************************************************************************/
/** INCLUDE FILES                                                            **/
/******************************************************************************/

#include "LEPTON_Types.h"
#include "LEPTON_ErrorCodes.h"
#include "LEPTON_Macros.h"
#include "bbb_I2C.h"
#include "LEPTON_I2C_Reg.h"
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>

//#define _AVR_STK600_ATXMEGA128A1_BOARD

//#ifdef _AVR_STK600_ATXMEGA128A1_BOARD
//    #include "twiDriver.h"
//#endif


/******************************************************************************/
/** LOCAL DEFINES                                                            **/
/******************************************************************************/
//Raspi handle;
int leptonDevice;
const LEP_INT32 ADDRESS_SIZE_BYTES = 2;
const LEP_INT32 VALUE_SIZE_BYTES = 2;
float clk_rate;
const LEP_INT32 comm_timeout_ms = 500;

/******************************************************************************/
/** LOCAL TYPE DEFINITIONS                                                   **/
/******************************************************************************/

/******************************************************************************/
/** PRIVATE DATA DECLARATIONS                                                **/
/******************************************************************************/

/******************************************************************************/
/** PRIVATE FUNCTION DECLARATIONS                                            **/
/******************************************************************************/

/******************************************************************************/
/** EXPORTED PUBLIC DATA                                                     **/
/******************************************************************************/

/******************************************************************************/
/** EXPORTED PUBLIC FUNCTIONS                                                **/
/******************************************************************************/


/******************************************************************************/
/**
 * Performs I2C Master Initialization
 * 
 * @param portID     LEP_UINT16  User specified port ID tag.  Can be used to
 *                   select between multiple cameras
 * 
 * @param BaudRate   Clock speed in kHz. Typically this is 400.
 *                   The Device Specific Driver will try to match the desired
 *                   speed.  This parameter is updated to the actual speed the
 *                   driver can use.
 * 
 * @return LEP_RESULT  0 if all goes well, errno otherwise
 */
LEP_RESULT DEV_I2C_MasterInit(LEP_UINT16 portID, 
                              LEP_UINT16 *BaudRate)
{
    LEP_RESULT result = LEP_OK;
   int bbb_result;
   LEP_UINT16 numFreeDevices[5];
   LEP_UINT8 testwrite[12];

    /* Place Device-Specific Interface here
    */
  
   //NONE: here we do a bbb_close(0) through 4
   //aa_close(0); aa_close(1); aa_close(2); aa_close(3); aa_close(4);


   //NONE: here we do a bbb_find_devices(5, numFreeDevices);
   //bbb_result = aa_find_devices(5, numFreeDevices);
   //handle = aa_open(0);
   //aa_i2c_bitrate(handle, 400);
   //aa_target_power(handle, AA_TARGET_POWER_BOTH);

    //do it live!
    leptonDevice = open("/dev/i2c-1", O_RDWR);
    if(leptonDevice < 0) {
	//we have problem connecting
	result = LEP_ERROR;
    } else if(ioctl(leptonDevice, I2C_SLAVE, LEP_I2C_DEVICE_ADDRESS) < 0) {
	result = LEP_ERROR;
    }

    return(result);
}

/**
 * Closes the I2C driver connection.
 * 
 * @return LEP_RESULT  0 if all goes well, errno otherwise.
 */
LEP_RESULT DEV_I2C_MasterClose()
{
    LEP_RESULT result = LEP_OK;

    /* Place Device-Specific Interface here
    */ 
   LEP_UINT32 handle = 1;
   //int return_value = ISLDLL_close(handle);

    return(result);
}

/**
 * Resets the I2C driver back to the READY state.
 * 
 * @return LEP_RESULT  0 if all goes well, errno otherwise.
 */
LEP_RESULT DEV_I2C_MasterReset(void )
{
    LEP_RESULT result = LEP_OK;

    /* Place Device-Specific Interface here
    */ 
   

    return(result);
}

LEP_RESULT DEV_I2C_MasterReadData(LEP_UINT16  portID,               // User-defined port ID
                                  LEP_UINT8   deviceAddress,        // Lepton Camera I2C Device Address
                                  LEP_UINT16  regAddress,           // Lepton Register Address
                                  LEP_UINT16 *readDataPtr,          // Read DATA buffer pointer
                                  LEP_UINT16  wordsToRead,          // Number of 16-bit words to Read
                                  LEP_UINT16 *numWordsRead,         // Number of 16-bit words actually Read
                                  LEP_UINT16 *status                // Transaction Status
                                 )
{
    LEP_RESULT result = LEP_OK;

    /* Place Device-Specific Interface here
    */ 
   int bbb_result;
   LEP_UINT16 bytesToWrite = ADDRESS_SIZE_BYTES;
   LEP_UINT16 bytesToRead = wordsToRead << 1;
   LEP_UINT16 bytesActuallyWritten = 0;
   LEP_UINT16 bytesActuallyRead = 0;
   LEP_UINT16 wordsActuallyRead = 0;
   LEP_UINT8* txdata = (LEP_UINT8*)malloc(sizeof(LEP_UINT8)*ADDRESS_SIZE_BYTES);
   LEP_UINT8* rxdata = (LEP_UINT8*)malloc(sizeof(LEP_UINT8)*bytesToRead);
   LEP_UINT16 *dataPtr;
   LEP_UINT16 *writePtr;

   *(LEP_UINT16*)txdata = REVERSE_ENDIENESS_UINT16(regAddress);

   //bbb_result = aa_i2c_write_read(handle, deviceAddress, AA_I2C_NO_FLAGS, 
   //            bytesToWrite, txdata, &bytesActuallyWritten, //write params
   //            bytesToRead, rxdata, &bytesActuallyRead); //read params

   //DONE: here we do a bbb bbb_i2c_write_read
   //this means writing the register, then reading bytes!


   int writeValue = write(leptonDevice, txdata, bytesToWrite);
   if(writeValue < 0) {
	//we have error!
	bytesActuallyRead = 0;
	bbb_result = -1;
   } else {

   	int readValue = read(leptonDevice, rxdata, bytesToRead);

	if(readValue < 0) {
		bytesActuallyRead = 0;
		bbb_result = -1;
	} else {
		bytesActuallyRead = readValue;
		bbb_result = LEP_OK;
	}
   }

   wordsActuallyRead = (LEP_UINT16)(bytesActuallyRead >> 1);
   *numWordsRead = wordsActuallyRead;

   dataPtr = (LEP_UINT16*)&rxdata[0];
   writePtr = readDataPtr;
   while(wordsActuallyRead--){
      *writePtr++ = REVERSE_ENDIENESS_UINT16(*dataPtr);
      dataPtr++;
   }
   free(txdata);
   free(rxdata);

   LEP_UINT8* byteData = (LEP_UINT8*)readDataPtr;


   if(bbb_result != 0 || bytesActuallyRead != bytesToRead)
   {
      result = LEP_ERROR_I2C_FAIL;
   }
   else
   {
      result = (LEP_RESULT)bbb_result;
   }
   return(result);
}

LEP_RESULT DEV_I2C_MasterWriteData(LEP_UINT16  portID,              // User-defined port ID
                                   LEP_UINT8   deviceAddress,       // Lepton Camera I2C Device Address
                                   LEP_UINT16  regAddress,          // Lepton Register Address
                                   LEP_UINT16 *writeDataPtr,        // Write DATA buffer pointer
                                   LEP_UINT16  wordsToWrite,        // Number of 16-bit words to Write
                                   LEP_UINT16 *numWordsWritten,     // Number of 16-bit words actually written
                                   LEP_UINT16 *status)              // Transaction Status
{
    LEP_RESULT result = LEP_OK;
   
   int bbb_result;
   
   LEP_INT16 bytesOfDataToWrite = (wordsToWrite << 1);
   LEP_INT16 bytesToWrite = bytesOfDataToWrite + ADDRESS_SIZE_BYTES;
   LEP_INT16 bytesActuallyWritten = 0;
   LEP_UINT8* txdata = (LEP_UINT8*)malloc(sizeof(LEP_UINT8)*(int)bytesToWrite);
   LEP_UINT16 *dataPtr;
   LEP_UINT16 *txPtr;

   *(LEP_UINT16*)txdata = REVERSE_ENDIENESS_UINT16(regAddress);
   dataPtr = (LEP_UINT16*)&writeDataPtr[0];
   txPtr = (LEP_UINT16*)&txdata[ADDRESS_SIZE_BYTES]; //Don't overwrite the address bytes
   while(wordsToWrite--){
      *txPtr++ = (LEP_UINT16)REVERSE_ENDIENESS_UINT16(*dataPtr);
      dataPtr++;
   }

   //bbb_result = aa_i2c_write(handle, device_addr, AA_I2C_NO_FLAGS, bytesToWrite, (LEP_UINT8*)writeDataPtr);
   
    //DONE: here we do bbb_i2c_write_ext
    //bbb_result = aa_i2c_write_ext(handle, deviceAddress, AA_I2C_NO_FLAGS, bytesToWrite, (LEP_UINT8*)txdata, (u16*)&bytesActuallyWritten);


    bytesActuallyWritten = write(leptonDevice, (LEP_UINT8*)txdata, bytesToWrite);

    if(bytesActuallyWritten < 0) {
	//if it's -1, we had error, no bytes written or something. just lie and say no bytes written
	bytesActuallyWritten = 0;
	bbb_result = LEP_ERROR;
    } else {
	bbb_result = LEP_OK;
    }

   *numWordsWritten = (bytesActuallyWritten >> 1);

   result = (LEP_RESULT)bbb_result;
   free(txdata);

   if(bbb_result != 0 || bytesActuallyWritten != bytesToWrite)
   {
      result = LEP_ERROR;
   }
   return(result);
}

LEP_RESULT DEV_I2C_MasterReadRegister( LEP_UINT16 portID,
                                       LEP_UINT8  deviceAddress, 
                                       LEP_UINT16 regAddress,
                                       LEP_UINT16 *regValue,     // Number of 16-bit words actually written
                                       LEP_UINT16 *status
                                     )
{
    LEP_RESULT result = LEP_OK;

   LEP_UINT16 wordsActuallyRead;
    /* Place Device-Specific Interface here
    */ 
   result = DEV_I2C_MasterReadData(portID, deviceAddress, regAddress, regValue, 1 /*1 word*/, &wordsActuallyRead, status);

   return(result);
}

LEP_RESULT DEV_I2C_MasterWriteRegister( LEP_UINT16 portID,
                                        LEP_UINT8  deviceAddress, 
                                        LEP_UINT16 regAddress,
                                        LEP_UINT16 regValue,     // Number of 16-bit words actually written
                                        LEP_UINT16 *status
                                      )
{
   LEP_RESULT result = LEP_OK;
   LEP_UINT16 wordsActuallyWritten;
    /* Place Device-Specific Interface here
    */ 
   result = DEV_I2C_MasterWriteData(portID, deviceAddress, regAddress, &regValue, 1, &wordsActuallyWritten, status);

   return(result);
}
LEP_RESULT DEV_I2C_MasterStatus(void )
{
    LEP_RESULT result = LEP_OK;

    /* Place Device-Specific Interface here
    */ 


    return(result);
}


/******************************************************************************/
/** PRIVATE MODULE FUNCTIONS                                                 **/
/******************************************************************************/


