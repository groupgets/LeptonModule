/*******************************************************************************
**
**    File NAME: LEPTON_OEM.c
**
**      AUTHOR:  Benjamin Kluwe
**
**      CREATED: 03/22/2017
**
**      DESCRIPTION: Lepton SDK OEM Module Command Interface
**
**/
/******************************************************************************/
/** INCLUDE FILES                                                            **/
/******************************************************************************/
#include "LEPTON_SDK.h"
#include "LEPTON_OEM.h"
#include "LEPTON_I2C_Reg.h"

/******************************************************************************/
/** LOCAL DEFINES                                                            **/
/******************************************************************************/

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

/**************************************/

LEP_RESULT LEP_RunOemPowerOn(LEP_CAMERA_PORT_DESC_T_PTR portDescPtr)
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 1;
	LEP_UINT16 data = 0;
	
	/* this should work to turn the power back on, but it doesn't.
	** the Software IDD says to write 0 to the Camera I2C Device ID,
	** but this is the only reference to be found in the document.
	*/
	result = LEP_I2C_MasterWriteData( portDescPtr->portID,
					  portDescPtr->deviceAddress,
					  LEP_I2C_POWER_ON_REG,
					  &data,
					  attributeWordLength);

	return( result );
}

LEP_RESULT LEP_RunOemPowerDown(LEP_CAMERA_PORT_DESC_T_PTR portDescPtr)
{
	LEP_RESULT  result = LEP_OK;

	result = LEP_RunCommand( portDescPtr, ( LEP_COMMAND_ID )LEP_CID_OEM_POWER_DOWN );

	return( result );
}

LEP_RESULT LEP_GetOemFlirPartNumber(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_PART_NUMBER_T_PTR oemPartNumberPtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 16;

	if( oemPartNumberPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_FLIR_SYTEMS_PART_NUMBER,
	                           ( LEP_ATTRIBUTE_T_PTR )oemPartNumberPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemSoftwareVersion(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_SW_VERSION_T_PTR oemSoftwareVersionPtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 4;

	if( oemSoftwareVersionPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_SOFTWARE_REVISION,
	                           ( LEP_ATTRIBUTE_T_PTR )oemSoftwareVersionPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemVideoOutputEnable(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_VIDEO_OUTPUT_ENABLE_E_PTR oemVideoOutputEnablePtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( oemVideoOutputEnablePtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_VIDEO_OUTPUT_ENABLE,
	                           ( LEP_ATTRIBUTE_T_PTR )oemVideoOutputEnablePtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemVideoOutputEnable(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_VIDEO_OUTPUT_ENABLE_E oemVideoOutputEnable )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( oemVideoOutputEnable >= LEP_END_VIDEO_OUTPUT_ENABLE )
	{
	   return( LEP_RANGE_ERROR );
	}

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_VIDEO_OUTPUT_ENABLE,
	                           ( LEP_ATTRIBUTE_T_PTR ) & oemVideoOutputEnable,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemVideoOutputFormat(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_VIDEO_OUTPUT_FORMAT_E_PTR oemVideoOutputFormatPtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( oemVideoOutputFormatPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_VIDEO_OUTPUT_FORMAT_SELECT,
	                           ( LEP_ATTRIBUTE_T_PTR )oemVideoOutputFormatPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemVideoOutputFormat( LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_VIDEO_OUTPUT_FORMAT_E oemVideoOutputFormat )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( oemVideoOutputFormat >= LEP_END_VIDEO_OUTPUT_FORMAT )
	{
	   return( LEP_RANGE_ERROR );
	}

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_VIDEO_OUTPUT_FORMAT_SELECT,
	                           ( LEP_ATTRIBUTE_T_PTR ) & oemVideoOutputFormat,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemVideoOutputSource(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_VIDEO_OUTPUT_SOURCE_E_PTR oemVideoOutputSourcePtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( oemVideoOutputSourcePtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_VIDEO_OUTPUT_SOURCE_SELECT,
	                           ( LEP_ATTRIBUTE_T_PTR )oemVideoOutputSourcePtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemVideoOutputSource(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_VIDEO_OUTPUT_SOURCE_E oemVideoOutputSource )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( oemVideoOutputSource >= LEP_END_VIDEO_OUTPUT_SOURCE )
	{
	   return( LEP_RANGE_ERROR );
	}

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_VIDEO_OUTPUT_SOURCE_SELECT,
	                           ( LEP_ATTRIBUTE_T_PTR ) & oemVideoOutputSource,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemCustPartNumber(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_PART_NUMBER_T_PTR oemPartNumberPtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 16;

	if( oemPartNumberPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_CUSTOMER_PART_NUMBER,
	                           ( LEP_ATTRIBUTE_T_PTR )oemPartNumberPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemVideoOutputSourceConstant(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_UINT16 *oemVideoOutputSourceConstPtr)
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 1;

	if( oemVideoOutputSourceConstPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_VIDEO_OUTPUT_SOURCE_CONSTANT_VALUE,
	                           ( LEP_ATTRIBUTE_T_PTR )oemVideoOutputSourceConstPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemVideoOutputSourceConstant(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_UINT16 oemVideoOutputSourceConstant)
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 1;

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_VIDEO_OUTPUT_SOURCE_CONSTANT_VALUE,
	                           ( LEP_ATTRIBUTE_T_PTR ) & oemVideoOutputSourceConstant,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_RunOemReboot(LEP_CAMERA_PORT_DESC_T_PTR portDescPtr)
{
	LEP_RESULT  result = LEP_OK;

	if((result = LEP_RunOemPowerDown(portDescPtr)) != LEP_OK) {
		return result;
	}
	
	result = LEP_RunOemPowerOn(portDescPtr);

	return result;
}

LEP_RESULT LEP_GetOemFFCNormalizationTarget(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_FFC_NORMALIZATION_TARGET_T_PTR ffcTargetPtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 1;

	if( ffcTargetPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_FFC_NORMALIZATION_TARGET,
	                           ( LEP_ATTRIBUTE_T_PTR )ffcTargetPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemFFCNormalizationTarget(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_FFC_NORMALIZATION_TARGET_T ffcTarget )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 1;

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_FFC_NORMALIZATION_TARGET,
	                           ( LEP_ATTRIBUTE_T_PTR ) & ffcTarget,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_RunOemFFC( LEP_CAMERA_PORT_DESC_T_PTR portDescPtr )
{
	LEP_RESULT  result = LEP_OK;

	result = LEP_RunCommand(portDescPtr, ( LEP_COMMAND_ID )LEP_CID_OEM_FFC_NORMALIZATION_TARGET);

	return result;
}

LEP_RESULT LEP_GetOemCalStatus(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
				LEP_OEM_STATUS_E_PTR calStatusPtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( calStatusPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_CAL_STATUS,
	                           ( LEP_ATTRIBUTE_T_PTR )calStatusPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemFrameMean(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
				LEP_OEM_FRAME_AVERAGE_T_PTR frameAveragePtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 1;

	if( frameAveragePtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_FRAME_MEAN_INTENSITY,
	                           ( LEP_ATTRIBUTE_T_PTR )frameAveragePtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemGpioMode(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
				LEP_OEM_GPIO_MODE_E_PTR gpioModePtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( gpioModePtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_GPIO_MODE_SELECT,
	                           ( LEP_ATTRIBUTE_T_PTR )gpioModePtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemGpioMode(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
				LEP_OEM_GPIO_MODE_E gpioMode )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( gpioMode >= LEP_END_OEM_GPIO_MODE )
	{
	   return( LEP_RANGE_ERROR );
	}

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_GPIO_MODE_SELECT,
	                           ( LEP_ATTRIBUTE_T_PTR ) & gpioMode,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemGpioVsyncPhaseDelay(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_VSYNC_DELAY_E_PTR numHsyncLinesPtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( numHsyncLinesPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_GPIO_VSYNC_PHASE_DELAY,
	                           ( LEP_ATTRIBUTE_T_PTR )numHsyncLinesPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemGpioVsyncPhaseDelay(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_VSYNC_DELAY_E numHsyncLines )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( numHsyncLines >= LEP_END_OEM_VSYNC_DELAY )
	{
	   return( LEP_RANGE_ERROR );
	}

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_GPIO_VSYNC_PHASE_DELAY,
	                           ( LEP_ATTRIBUTE_T_PTR ) & numHsyncLines,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemUserDefaultsState(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_USER_PARAMS_STATE_E_PTR userParamsStatePtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( userParamsStatePtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_USER_DEFAULTS,
	                           ( LEP_ATTRIBUTE_T_PTR )userParamsStatePtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_RunOemUserDefaultsCopyToOtp(LEP_CAMERA_PORT_DESC_T_PTR portDescPtr)
{
	LEP_RESULT result = LEP_OK;
	
	result = LEP_RunCommand(portDescPtr, ( LEP_COMMAND_ID )LEP_CID_OEM_USER_DEFAULTS);

	return result;
}

LEP_RESULT LEP_RunOemUserDefaultsRestore(LEP_CAMERA_PORT_DESC_T_PTR portDescPtr)
{
	LEP_RESULT result = LEP_OK;
	
	result = LEP_RunCommand(portDescPtr, ( LEP_COMMAND_ID )LEP_CID_OEM_RESTORE_USER_DEFAULTS);

	return result;
}

LEP_RESULT LEP_GetOemShutterProfileObj(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_SHUTTER_PROFILE_OBJ_T_PTR ShutterProfileObjPtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( ShutterProfileObjPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_SHUTTER_PROFILE,
	                           ( LEP_ATTRIBUTE_T_PTR )ShutterProfileObjPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemShutterProfileObj(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_SHUTTER_PROFILE_OBJ_T ShutterProfileObj )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_SHUTTER_PROFILE,
	                           ( LEP_ATTRIBUTE_T_PTR ) & ShutterProfileObj,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemThermalShutdownEnable(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_THERMAL_SHUTDOWN_ENABLE_T_PTR ThermalShutdownEnableStatePtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( ThermalShutdownEnableStatePtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_THERMAL_SHUTDOWN_ENABLE,
	                           ( LEP_ATTRIBUTE_T_PTR )ThermalShutdownEnableStatePtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemThermalShutdownEnable(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_THERMAL_SHUTDOWN_ENABLE_T ThermalShutdownEnableState )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( ThermalShutdownEnableState.oemThermalShutdownEnable >= LEP_END_OEM_STATE )
	{
	   return( LEP_RANGE_ERROR );
	}

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_THERMAL_SHUTDOWN_ENABLE,
	                           ( LEP_ATTRIBUTE_T_PTR ) & ThermalShutdownEnableState,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemBadPixelReplaceControl(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_BAD_PIXEL_REPLACE_CONTROL_T_PTR BadPixelReplaceControlPtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( BadPixelReplaceControlPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_BAD_PIXEL_REPLACEMENT_CONTROL,
	                           ( LEP_ATTRIBUTE_T_PTR )BadPixelReplaceControlPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemBadPixelReplaceControl(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_BAD_PIXEL_REPLACE_CONTROL_T BadPixelReplaceControl )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( BadPixelReplaceControl.oemBadPixelReplaceEnable >= LEP_END_OEM_STATE )
	{
	   return( LEP_RANGE_ERROR );
	}

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_BAD_PIXEL_REPLACEMENT_CONTROL,
	                           ( LEP_ATTRIBUTE_T_PTR ) & BadPixelReplaceControl,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemTemporalFilterControl(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_TEMPORAL_FILTER_CONTROL_T_PTR TemporalFilterControlPtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( TemporalFilterControlPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_TEMPORAL_FILTER_CONTROL,
	                           ( LEP_ATTRIBUTE_T_PTR )TemporalFilterControlPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemTemporalFilterControl(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_TEMPORAL_FILTER_CONTROL_T TemporalFilterControl )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( TemporalFilterControl.oemTemporalFilterEnable >= LEP_END_OEM_STATE )
	{
	   return( LEP_RANGE_ERROR );
	}

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_TEMPORAL_FILTER_CONTROL,
	                           ( LEP_ATTRIBUTE_T_PTR ) & TemporalFilterControl,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemColumnNoiseEstimateControl(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_COLUMN_NOISE_ESTIMATE_CONTROL_T_PTR ColumnNoiseEstimateControlPtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( ColumnNoiseEstimateControlPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_COLUMN_NOISE_FILTER_CONTROL,
	                           ( LEP_ATTRIBUTE_T_PTR )ColumnNoiseEstimateControlPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemColumnNoiseEstimateControl(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_COLUMN_NOISE_ESTIMATE_CONTROL_T ColumnNoiseEstimateControl )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( ColumnNoiseEstimateControl.oemColumnNoiseEstimateEnable >= LEP_END_OEM_STATE )
	{
	   return( LEP_RANGE_ERROR );
	}

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_COLUMN_NOISE_FILTER_CONTROL,
	                           ( LEP_ATTRIBUTE_T_PTR ) & ColumnNoiseEstimateControl,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_GetOemPixelNoiseSettings(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_PIXEL_NOISE_SETTINGS_T_PTR pixelNoiseSettingsPtr )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( pixelNoiseSettingsPtr == NULL )
	{
	   return( LEP_BAD_ARG_POINTER_ERROR );
	}

	/* Perform Command */
	result = LEP_GetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_PIXEL_NOISE_FILTER_CONTROL,
	                           ( LEP_ATTRIBUTE_T_PTR )pixelNoiseSettingsPtr,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_SetOemPixelNoiseSettings(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_PIXEL_NOISE_SETTINGS_T pixelNoiseSettings )
{
	LEP_RESULT result = LEP_OK;
	LEP_UINT16 attributeWordLength = 2;

	if( pixelNoiseSettings.oemNoiseEstimateEnable >= LEP_END_OEM_STATE )
	{
	   return( LEP_RANGE_ERROR );
	}

	/* Perform Command */
	result = LEP_SetAttribute( portDescPtr,
	                           ( LEP_COMMAND_ID )LEP_CID_OEM_PIXEL_NOISE_FILTER_CONTROL,
	                           ( LEP_ATTRIBUTE_T_PTR ) & pixelNoiseSettings,
	                           attributeWordLength );

	return( result );
}

LEP_RESULT LEP_RunOemFFCNormalization(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
					LEP_OEM_FFC_NORMALIZATION_TARGET_T ffcTarget )
{
	LEP_RESULT result = LEP_OK;
	
	if((result = LEP_SetOemFFCNormalizationTarget(portDescPtr, ffcTarget)) != LEP_OK) {
		return result;
	}

	result = LEP_RunOemFFC(portDescPtr);

	return result;
}
