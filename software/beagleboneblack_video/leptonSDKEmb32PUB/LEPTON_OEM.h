/*******************************************************************************
**
**	 File NAME: LEPTON_OEM.h
**
**		AUTHOR:  Benjamin Kluwe
**
**		CREATED: 03/22/2017
**
**		DESCRIPTION: Lepton SDK OEM Module Command Interface
**
**/

#ifndef _LEPTON_OEM_H_
#define _LEPTON_OEM_H_

#ifdef __cplusplus
extern "C"
{
#endif
	
	/******************************************************************************/
	/** INCLUDE FILES
	/******************************************************************************/
	#include "LEPTON_Types.h"
	
	/******************************************************************************/
	/** EXPORTED DEFINES
	/******************************************************************************/
	
	/* OEM Module Command IDs
	** protection bit 0x4000 enabled by default */
	
	#define LEP_OEM_MODULE_BASE									0x4800
	
	#define LEP_CID_OEM_POWER_ON				(LEP_OEM_MODULE_BASE + 0x0000 )
	#define LEP_CID_OEM_POWER_DOWN				(LEP_OEM_MODULE_BASE + 0x0000 )
	#define LEP_CID_OEM_FLIR_SYTEMS_PART_NUMBER	 	(LEP_OEM_MODULE_BASE + 0x001C )
	#define LEP_CID_OEM_SOFTWARE_REVISION			(LEP_OEM_MODULE_BASE + 0x0020 )
	#define LEP_CID_OEM_VIDEO_OUTPUT_ENABLE			(LEP_OEM_MODULE_BASE + 0x0024 )
	#define LEP_CID_OEM_VIDEO_OUTPUT_FORMAT_SELECT 		(LEP_OEM_MODULE_BASE + 0x0028 )
	#define LEP_CID_OEM_VIDEO_OUTPUT_SOURCE_SELECT 		(LEP_OEM_MODULE_BASE + 0x002C )
	#define LEP_CID_OEM_CUSTOMER_PART_NUMBER		(LEP_OEM_MODULE_BASE + 0x0038 )
	#define LEP_CID_OEM_VIDEO_OUTPUT_SOURCE_CONSTANT_VALUE	(LEP_OEM_MODULE_BASE + 0x003C )
	#define LEP_CID_OEM_CAMERA_REBOOT	 		(LEP_OEM_MODULE_BASE + 0x0040 )
	#define LEP_CID_OEM_FFC_NORMALIZATION_TARGET		(LEP_OEM_MODULE_BASE + 0x0044 )
	#define LEP_CID_OEM_CAL_STATUS				(LEP_OEM_MODULE_BASE + 0x0048 )
	#define LEP_CID_OEM_FRAME_MEAN_INTENSITY		(LEP_OEM_MODULE_BASE + 0x004C )
	#define LEP_CID_OEM_GPIO_MODE_SELECT			(LEP_OEM_MODULE_BASE + 0x0054 )
	#define LEP_CID_OEM_GPIO_VSYNC_PHASE_DELAY		(LEP_OEM_MODULE_BASE + 0x0058 )
	#define LEP_CID_OEM_USER_DEFAULTS			(LEP_OEM_MODULE_BASE + 0x005C )
	#define LEP_CID_OEM_RESTORE_USER_DEFAULTS		(LEP_OEM_MODULE_BASE + 0x0060 )
	#define LEP_CID_OEM_SHUTTER_PROFILE	 		(LEP_OEM_MODULE_BASE + 0x0064 )
	#define LEP_CID_OEM_THERMAL_SHUTDOWN_ENABLE		(LEP_OEM_MODULE_BASE + 0x0068 )
	#define LEP_CID_OEM_BAD_PIXEL_REPLACEMENT_CONTROL	(LEP_OEM_MODULE_BASE + 0x006C )
	#define LEP_CID_OEM_TEMPORAL_FILTER_CONTROL		(LEP_OEM_MODULE_BASE + 0x0070 )
	#define LEP_CID_OEM_COLUMN_NOISE_FILTER_CONTROL		(LEP_OEM_MODULE_BASE + 0x0074 )
	#define LEP_CID_OEM_PIXEL_NOISE_FILTER_CONTROL		(LEP_OEM_MODULE_BASE + 0x0078 )
	
	/******************************************************************************/
	/** EXPORTED PUBLIC TYPES
	/******************************************************************************/
	
	/* Part Number: A (32 byte string) identifier unique to a
	**	specific configuration of module; essentially a module
	**	Configuration ID.
	*/
	typedef LEP_CHAR8 *LEP_OEM_PART_NUMBER_T, *LEP_OEM_PART_NUMBER_T_PTR;
	typedef LEP_UINT16 LEP_OEM_FFC_NORMALIZATION_TARGET_T, *LEP_OEM_FFC_NORMALIZATION_TARGET_T_PTR;
	typedef LEP_UINT16 LEP_OEM_FRAME_AVERAGE_T, *LEP_OEM_FRAME_AVERAGE_T_PTR;
		
	/* Video Output Enable Enum */
	typedef enum LEP_OEM_VIDEO_OUTPUT_ENABLE_TAG
	{
		LEP_VIDEO_OUTPUT_DISABLE = 0,
		LEP_VIDEO_OUTPUT_ENABLE,
	
		LEP_END_VIDEO_OUTPUT_ENABLE
	} LEP_OEM_VIDEO_OUTPUT_ENABLE_E, *LEP_OEM_VIDEO_OUTPUT_ENABLE_E_PTR;
	
	/* Video Output Format Selection */
	typedef enum LEP_OEM_VIDEO_OUTPUT_FORMAT_TAG
	{
		LEP_VIDEO_OUTPUT_FORMAT_RAW8 = 0,	// To be supported in later release
		LEP_VIDEO_OUTPUT_FORMAT_RAW10,		// To be supported in later release
		LEP_VIDEO_OUTPUT_FORMAT_RAW12,		// To be supported in later release
		LEP_VIDEO_OUTPUT_FORMAT_RGB888,		// SUPPORTED in this release
		LEP_VIDEO_OUTPUT_FORMAT_RGB666,		// To be supported in later release
		LEP_VIDEO_OUTPUT_FORMAT_RGB565,		// To be supported in later release
		LEP_VIDEO_OUTPUT_FORMAT_YUV422_8BIT,	// To be supported in later release
		LEP_VIDEO_OUTPUT_FORMAT_RAW14,		// SUPPORTED in this release
		LEP_VIDEO_OUTPUT_FORMAT_YUV422_10BIT,	// To be supported in later release
		LEP_VIDEO_OUTPUT_FORMAT_USER_DEFINED,	// To be supported in later release
		LEP_VIDEO_OUTPUT_FORMAT_RAW8_2,		// To be supported in later release
		LEP_VIDEO_OUTPUT_FORMAT_RAW8_3,		// To be supported in later release
		LEP_VIDEO_OUTPUT_FORMAT_RAW8_4,		// To be supported in later release
		LEP_VIDEO_OUTPUT_FORMAT_RAW8_5,		// To be supported in later release
		LEP_VIDEO_OUTPUT_FORMAT_RAW8_6,		// To be supported in later release
	
		LEP_END_VIDEO_OUTPUT_FORMAT
	} LEP_OEM_VIDEO_OUTPUT_FORMAT_E, *LEP_OEM_VIDEO_OUTPUT_FORMAT_E_PTR;
	
	/* Video Output Source Selection */
	typedef enum LEP_OEM_VIDEO_OUTPUT_SOURCE_TAG
	{
		LEP_VIDEO_OUTPUT_SOURCE_RAW = 0,	// To be supported in later release
		LEP_VIDEO_OUTPUT_SOURCE_COOKED,		// To be supported in later release
		LEP_VIDEO_OUTPUT_SOURCE_RAMP,		// To be supported in later release
		LEP_VIDEO_OUTPUT_SOURCE_CONSTANT,	// SUPPORTED in this release
		LEP_VIDEO_OUTPUT_SOURCE_RAMP_H,		// To be supported in later release
		LEP_VIDEO_OUTPUT_SOURCE_RAMP_V,		// To be supported in later release
		LEP_VIDEO_OUTPUT_SOURCE_RAMP_CUSTOM,	// To be supported in later release
		LEP_VIDEO_OUTPUT_SOURCE_FRAME_CAPTURE,	// SUPPORTED in this release
		LEP_VIDEO_OUTPUT_SOURCE_FRAME_FREEZE,	// To be supported in later release
		LEP_VIDEO_OUTPUT_SOURCE_FRAME_0,	// To be supported in later release
		LEP_VIDEO_OUTPUT_SOURCE_FRAME_1,	// To be supported in later release
		LEP_VIDEO_OUTPUT_SOURCE_FRAME_2,	// To be supported in later release
		LEP_VIDEO_OUTPUT_SOURCE_FRAME_3,	// To be supported in later release
		LEP_VIDEO_OUTPUT_SOURCE_FRAME_4,	// To be supported in later release
		
		LEP_END_VIDEO_OUTPUT_SOURCE
	} LEP_OEM_VIDEO_OUTPUT_SOURCE_E, *LEP_OEM_VIDEO_OUTPUT_SOURCE_E_PTR;
	
	/* OEM Status */
	typedef enum LEP_OEM_STATUS_E_TAG
	{
		LEP_OEM_STATUS_OTP_WRITE_ERROR = -2,
		LEP_OEM_STATUS_ERROR,
		LEP_OEM_STATUS_READY,
		LEP_OEM_STATUS_BUSY,
		LEP_OEM_STATUS_FRAME_AVERAGE_COLLECTING_FRAMES,
	
		LEP_END_OEM_STATUS
	} LEP_OEM_STATUS_E, *LEP_OEM_STATUS_E_PTR;
	
	/* GPIO Mode Selection */
	typedef enum LEP_OEM_GPIO_MODE_E_TAG
	{
		LEP_OEM_GPIO_MODE_GPIO = 0,
		LEP_OEM_GPIO_MODE_I2C_MASTER,
		LEP_OEM_GPIO_MODE_SPI_MASTER_VLB_DATA,
		LEP_OEM_GPIO_MODE_SPI_MASTER_REG_DATA,
		LEP_OEM_GPIO_MODE_SPI_SLAVE_REG_DATA,
		LEP_OEM_GPIO_MODE_VSYNC,
		
		LEP_END_OEM_GPIO_MODE
	} LEP_OEM_GPIO_MODE_E, *LEP_OEM_GPIO_MODE_E_PTR;
	
	/* VSync Delay Selection */
	typedef enum LEP_OEM_VSYNC_DELAY_E_TAG
	{
		LEP_OEM_VSYNC_DELAY_MINUS_3 = -3,
		LEP_OEM_VSYNC_DELAY_MINUS_2,
		LEP_OEM_VSYNC_DELAY_MINUS_1,
		LEP_OEM_VSYNC_DELAY_NONE,
		LEP_OEM_VSYNC_DELAY_PLUS_1,
		LEP_OEM_VSYNC_DELAY_PLUS_2,
		LEP_OEM_VSYNC_DELAY_PLUS_3,
	
		LEP_END_OEM_VSYNC_DELAY
	} LEP_OEM_VSYNC_DELAY_E, *LEP_OEM_VSYNC_DELAY_E_PTR;
	
	/* OEM User Defaults State */
	typedef enum LEP_OEM_USER_PARAMS_STATE_E_TAG
	{
		LEP_OEM_USER_PARAMS_STATE_NOT_WRITTEN = 0,
		LEP_OEM_USER_PARAMS_STATE_WRITTEN,
	
		LEP_END_OEM_USER_PARAMS_STATE
	} LEP_OEM_USER_PARAMS_STATE_E, *LEP_OEM_USER_PARAMS_STATE_E_PTR;
	
	/* OEM State */
	typedef enum LEP_OEM_STATE_E_TAG
	{
		LEP_OEM_DISABLE = 0,
		LEP_OEM_ENABLE,
	
		LEP_END_OEM_STATE
	} LEP_OEM_STATE_E, *LEP_OEM_STATE_E_PTR;
	
	/******************************************************************************/
	/** EXPORTED PUBLIC STRUCTURES
	/******************************************************************************/
	
	/* Software Version ID: A (24 bit depth) identifier for
	** the software version stored in OTP. */
	typedef struct LEP_OEM_SW_VERSION_TAG
	{
		LEP_UINT8	gpp_major;
		LEP_UINT8	gpp_minor;
		LEP_UINT8	gpp_build;
		LEP_UINT8	dsp_major;
		LEP_UINT8	dsp_minor;
		LEP_UINT8	dsp_build;
		LEP_UINT16	reserved;
	} LEP_OEM_SW_VERSION_T, *LEP_OEM_SW_VERSION_T_PTR;

	/* Shutter Profile Object */
	typedef struct LEP_OEM_SHUTTER_PROFILE_OBJ_T_TAG
	{
		LEP_UINT16 closePreiodInFrames;
		LEP_UINT16 openPeriodInFrames;
	} LEP_OEM_SHUTTER_PROFILE_OBJ_T, *LEP_OEM_SHUTTER_PROFILE_OBJ_T_PTR;
	
	/* Thermal Shutdown structure */
	typedef struct LEP_OEM_THERMAL_SHUTDOWN_ENABLE_T_TAG
	{
		LEP_OEM_STATE_E oemThermalShutdownEnable;
	} LEP_OEM_THERMAL_SHUTDOWN_ENABLE_T, *LEP_OEM_THERMAL_SHUTDOWN_ENABLE_T_PTR;
	
	/* Bad Pixel Replacement Control structure */
	typedef struct LEP_OEM_BAD_PIXEL_REPLACE_CONTROL_T_TAG
	{
		LEP_OEM_STATE_E oemBadPixelReplaceEnable;
	} LEP_OEM_BAD_PIXEL_REPLACE_CONTROL_T, *LEP_OEM_BAD_PIXEL_REPLACE_CONTROL_T_PTR;
	
	/* Temporal Filter Control structure */
	typedef struct LEP_OEM_TEMPORAL_FILTER_CONTROL_T_TAG
	{
		LEP_OEM_STATE_E oemTemporalFilterEnable;
	} LEP_OEM_TEMPORAL_FILTER_CONTROL_T, *LEP_OEM_TEMPORAL_FILTER_CONTROL_T_PTR;
	
	/* Column Noise Filter Control structure */
	typedef struct LEP_OEM_COLUMN_NOISE_ESTIMATE_CONTROL_T_TAG
	{
		LEP_OEM_STATE_E oemColumnNoiseEstimateEnable;
	} LEP_OEM_COLUMN_NOISE_ESTIMATE_CONTROL_T, *LEP_OEM_COLUMN_NOISE_ESTIMATE_CONTROL_T_PTR;
	
	/* Pixel Noise Filter Control structure */
	typedef struct LEP_OEM_PIXEL_NOISE_SETTINGS_T_TAG
	{
		LEP_OEM_STATE_E oemNoiseEstimateEnable;
	} LEP_OEM_PIXEL_NOISE_SETTINGS_T, *LEP_OEM_PIXEL_NOISE_SETTINGS_T_PTR;
	
	/******************************************************************************/
	/** EXPORTED PUBLIC DATA
	/******************************************************************************/
	
	/******************************************************************************/
	/** EXPORTED PUBLIC FUNCTIONS
	/******************************************************************************/
	
	extern LEP_RESULT LEP_RunOemPowerOn(LEP_CAMERA_PORT_DESC_T_PTR portDescPtr);
	
	extern LEP_RESULT LEP_RunOemPowerDown(LEP_CAMERA_PORT_DESC_T_PTR portDescPtr);
	
	extern LEP_RESULT LEP_GetOemFlirPartNumber(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_PART_NUMBER_T_PTR oemPartNumberPtr );
	
	extern LEP_RESULT LEP_GetOemSoftwareVersion(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_SW_VERSION_T *oemSoftwareVersionPtr );
	
	extern LEP_RESULT LEP_GetOemVideoOutputEnable(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_VIDEO_OUTPUT_ENABLE_E_PTR oemVideoOutputEnablePtr );
	
	extern LEP_RESULT LEP_SetOemVideoOutputEnable(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_VIDEO_OUTPUT_ENABLE_E oemVideoOutputEnable );
	
	extern LEP_RESULT LEP_GetOemVideoOutputFormat(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_VIDEO_OUTPUT_FORMAT_E_PTR oemVideoOutputFormatPtr );
	
	extern LEP_RESULT LEP_SetOemVideoOutputFormat( LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_VIDEO_OUTPUT_FORMAT_E oemVideoOutputFormat );
	
	extern LEP_RESULT LEP_GetOemVideoOutputSource(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_VIDEO_OUTPUT_SOURCE_E_PTR oemVideoOutputSourcePtr );
	
	extern LEP_RESULT LEP_SetOemVideoOutputSource(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_VIDEO_OUTPUT_SOURCE_E oemVideoOutputSource );
	
	extern LEP_RESULT LEP_GetOemCustPartNumber(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_PART_NUMBER_T_PTR oemPartNumberPtr );
	
	extern LEP_RESULT LEP_GetOemVideoOutputSourceConstant(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_UINT16 *oemVideoOutputSourceConstPtr);
	
	extern LEP_RESULT LEP_SetOemVideoOutputSourceConstant(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_UINT16 oemVideoOutputSourceConstant);
	
	extern LEP_RESULT LEP_RunOemReboot(LEP_CAMERA_PORT_DESC_T_PTR portDescPtr);
	
	extern LEP_RESULT LEP_GetOemFFCNormalizationTarget(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_OEM_FFC_NORMALIZATION_TARGET_T_PTR ffcTargetPtr );
	
	extern LEP_RESULT LEP_SetOemFFCNormalizationTarget(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_OEM_FFC_NORMALIZATION_TARGET_T ffcTarget );
	
	extern LEP_RESULT LEP_RunOemFFC( LEP_CAMERA_PORT_DESC_T_PTR portDescPtr );
	
	extern LEP_RESULT LEP_GetOemCalStatus(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_STATUS_E_PTR calStatusPtr );
	
	extern LEP_RESULT LEP_GetOemFrameMean(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_FRAME_AVERAGE_T_PTR frameAveragePtr );
	
	extern LEP_RESULT LEP_GetOemGpioMode(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_GPIO_MODE_E_PTR gpioModePtr );
	
	extern LEP_RESULT LEP_SetOemGpioMode(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
						LEP_OEM_GPIO_MODE_E gpioMode );
	
	extern LEP_RESULT LEP_GetOemGpioVsyncPhaseDelay(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_OEM_VSYNC_DELAY_E_PTR numHsyncLinesPtr );
	
	extern LEP_RESULT LEP_SetOemGpioVsyncPhaseDelay(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_OEM_VSYNC_DELAY_E numHsyncLines );
	
	extern LEP_RESULT LEP_GetOemUserDefaultsState(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_USER_PARAMS_STATE_E_PTR userParamsStatePtr );
	
	extern LEP_RESULT LEP_RunOemUserDefaultsRestore(LEP_CAMERA_PORT_DESC_T_PTR portDescPtr);
	
	extern LEP_RESULT LEP_GetOemShutterProfileObj(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_SHUTTER_PROFILE_OBJ_T_PTR ShutterProfileObjPtr );
	
	extern LEP_RESULT LEP_SetOemShutterProfileObj(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_SHUTTER_PROFILE_OBJ_T ShutterProfileObj );
	
	extern LEP_RESULT LEP_GetOemThermalShutdownEnable(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_OEM_THERMAL_SHUTDOWN_ENABLE_T_PTR ThermalShutdownEnableStatePtr );
	
	extern LEP_RESULT LEP_SetOemThermalShutdownEnable(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_OEM_THERMAL_SHUTDOWN_ENABLE_T ThermalShutdownEnableState );
	
	extern LEP_RESULT LEP_GetOemBadPixelReplaceControl(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_OEM_BAD_PIXEL_REPLACE_CONTROL_T_PTR BadPixelReplaceControlPtr );
	
	extern LEP_RESULT LEP_SetOemBadPixelReplaceControl(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_OEM_BAD_PIXEL_REPLACE_CONTROL_T BadPixelReplaceControl );
	
	extern LEP_RESULT LEP_GetOemTemporalFilterControl(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_OEM_TEMPORAL_FILTER_CONTROL_T_PTR TemporalFilterControlPtr );
	
	extern LEP_RESULT LEP_SetOemTemporalFilterControl(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_OEM_TEMPORAL_FILTER_CONTROL_T TemporalFilterControl );
	
	extern LEP_RESULT LEP_GetOemColumnNoiseEstimateControl(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_OEM_COLUMN_NOISE_ESTIMATE_CONTROL_T_PTR ColumnNoiseEstimateControlPtr );
	
	extern LEP_RESULT LEP_SetOemColumnNoiseEstimateControl(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
								LEP_OEM_COLUMN_NOISE_ESTIMATE_CONTROL_T ColumnNoiseEstimateControl );
	
	extern LEP_RESULT LEP_GetOemPixelNoiseSettings(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_PIXEL_NOISE_SETTINGS_T_PTR pixelNoiseSettingsPtr );
	
	extern LEP_RESULT LEP_SetOemPixelNoiseSettings(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_PIXEL_NOISE_SETTINGS_T pixelNoiseSettings );
	
	extern LEP_RESULT LEP_RunOemFFCNormalization(	LEP_CAMERA_PORT_DESC_T_PTR portDescPtr,
							LEP_OEM_FFC_NORMALIZATION_TARGET_T ffcTarget );
	
	#ifdef __cplusplus

}
#endif

#endif /* _LEPTON_OEM_H_ */
