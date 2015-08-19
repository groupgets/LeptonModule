/*******************************************************************************
**
**    File NAME: raspi_I2C.h
**
**      AUTHOR:  Joseph Colicchio
**
**      CREATED: 9/25/2012
**  
**      DESCRIPTION: 
**
**      HISTORY:  9/25/2012 HT - Initial Draft 
**
**      Copyright 2011,2012,2013,2014,2015 FLIR Systems - Commercial Vision Systems
**      All rights reserved.
**
**      Proprietary - Company Only.
**  
**      This document is controlled to FLIR Technology Level 2.
**      The information contained in this document pertains to a dual use product
**      Controlled for export by the Export Administration Regulations (EAR). 
**      FLIR trade secrets contained herein are subject to disclosure restrictions 
**      as a matter of law. Diversion contrary to US law is prohibited.  
**      US Department of Commerce authorization is required prior to export or 
**      transfer to foreign persons or parties and for uses otherwise prohibited. 
**
*******************************************************************************/
#ifndef _RASPI_I2C_H_ 
    #define _RASPI_I2C_H_

    #ifdef __cplusplus
extern "C"
{
    #endif
/******************************************************************************/
/** INCLUDE FILES                                                            **/
/******************************************************************************/
    #include "LEPTON_Types.h"
    #include "LEPTON_ErrorCodes.h"

/******************************************************************************/
/** EXPORTED DEFINES                                                         **/
/******************************************************************************/

/******************************************************************************/
/** EXPORTED TYPE DEFINITIONS                                                **/
/******************************************************************************/

/******************************************************************************/
/** EXPORTED PUBLIC DATA                                                     **/
/******************************************************************************/

/******************************************************************************/
/** EXPORTED PUBLIC FUNCTIONS                                                **/
/******************************************************************************/

    extern LEP_RESULT DEV_I2C_MasterInit(LEP_UINT16 portID,
                                         LEP_UINT16 *BaudRate);

    extern LEP_RESULT DEV_I2C_MasterClose();

    extern LEP_RESULT DEV_I2C_MasterReset(void );

    extern LEP_RESULT DEV_I2C_MasterReadData(LEP_UINT16 portID,
                                             LEP_UINT8   deviceAddress,
                                             LEP_UINT16  regAddress,            // Lepton Register Address
                                             LEP_UINT16 *readDataPtr,
                                             LEP_UINT16  wordsToRead,          // Number of 16-bit words to Read
                                             LEP_UINT16 *numWordsRead,         // Number of 16-bit words actually Read
                                             LEP_UINT16 *status
                                            );

    extern LEP_RESULT DEV_I2C_MasterWriteData(LEP_UINT16 portID,
                                              LEP_UINT8   deviceAddress,
                                              LEP_UINT16  regAddress,            // Lepton Register Address
                                              LEP_UINT16 *writeDataPtr,
                                              LEP_UINT16  wordsToWrite,        // Number of 16-bit words to Write
                                              LEP_UINT16 *numWordsWritten,     // Number of 16-bit words actually written
                                              LEP_UINT16 *status
                                             );

    extern LEP_RESULT DEV_I2C_MasterReadRegister( LEP_UINT16 portID,
                                                  LEP_UINT8  deviceAddress, 
                                                  LEP_UINT16 regAddress,
                                                  LEP_UINT16 *regValue,     // Number of 16-bit words actually written
                                                  LEP_UINT16 *status
                                                );

    extern LEP_RESULT DEV_I2C_MasterWriteRegister( LEP_UINT16 portID,
                                                   LEP_UINT8  deviceAddress, 
                                                   LEP_UINT16 regAddress,
                                                   LEP_UINT16 regValue,     // Number of 16-bit words actually written
                                                   LEP_UINT16 *status
                                                 );

    extern LEP_RESULT DEV_I2C_MasterStatus(void );

/******************************************************************************/
    #ifdef __cplusplus
}
    #endif

#endif  /* _RASPI_I2C_H_ */

