#include <stdio.h>
#include <unistd.h>
#include "Lepton_I2C.h"

#include "leptonSDKEmb32PUB/LEPTON_SDK.h"
#include "leptonSDKEmb32PUB/LEPTON_RAD.h"
#include "leptonSDKEmb32PUB/LEPTON_SYS.h"
#include "leptonSDKEmb32PUB/LEPTON_OEM.h"
#include "leptonSDKEmb32PUB/LEPTON_Types.h"

bool _connected;

LEP_CAMERA_PORT_DESC_T _port;

int lepton_connect() {
	LEP_OpenPort(1, LEP_CCI_TWI, 400, &_port);
	_connected = true;
	return 0;
}

void lepton_perform_ffc() {
	if(!_connected) {
		lepton_connect();
	}
	int r =LEP_RunSysFFCNormalization(&_port);
	printf("ffc: %d\n", r);
}

void lepton_linear_on() {
	if(!_connected) {
		lepton_connect();
	}
	printf("enable rad %d\n", LEP_SetRadEnableState(&_port, LEP_RAD_ENABLE));
    LEP_SYS_FPA_TEMPERATURE_KELVIN_T temp=0;
	printf("fpa temp %d\n", LEP_GetSysFpaTemperatureKelvin(&_port, &temp));
	printf("fpa temp %x\n", temp);
	//printf("enable tlin %d\n", LEP_SetRadTLinearEnableState(&_port, LEP_RAD_ENABLE));
}

void lepton_perform_reset() {
	if(!_connected) {
		lepton_connect();
	}
	printf("reboot %d\n", 
    	LEP_RunOemReboot(&_port));
}

void lepton_vsync_enable() {
	if(!_connected) {
		lepton_connect();
	}
	printf("vsync_en %d\n", 
    	LEP_SetOemGpioMode(&_port, LEP_OEM_GPIO_MODE_VSYNC));
}

void lepton_restart_vospi() {
	if(!_connected) {
		lepton_connect();
	}
	printf("video disable %d\n", 
    	LEP_SetOemVideoOutputEnable(&_port, LEP_VIDEO_OUTPUT_DISABLE));
        sleep(10);
	printf("video enable %d\n", 
    	LEP_SetOemVideoOutputEnable(&_port, LEP_VIDEO_OUTPUT_ENABLE));
}
