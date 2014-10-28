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


#include <windows.h>
#include <commctrl.h>
#include <ntdef.h>
#include <winbase.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <setupapi.h>
#include <winioctl.h>
#include <ctype.h> 
#include <string.h>
#include <conio.h>
#include "resource.h"
#include "callbacks.h"
#include "guicon.h"

#include "serial_win32.h"

#define  TIMER1     0



HWND mainhWnd;
OVERLAPPED ol;
HANDLE MyThreadHandle;
;

int timeout_counter = 0;
static int lepton_image[80][80];
static int draw_lepton_image[80][80];
#define MAX_IMAGE_SIZE (80*60*2)
unsigned char image_buffer[MAX_IMAGE_SIZE];
int image_buffer_index = 0;
int parser_state = 0;

int request_next_image = 1;

typedef struct { 
	unsigned char r; 
	unsigned char g; 
	unsigned char b; 
} color_triplet;

color_triplet table[1280] = {{0,0,0}};
unsigned long colors[1280] ;

void generate_pallette(void)
{
	int i;
	int NCOLORS=1280;

	for(int i = 0; i <= 255; i++) 
	{
	
		// Red (255,0,0) to yellow (255,255,0)
		colors[NCOLORS-i] = RGB(255,i,0);
		// Yellow to green (0,255,0)
		colors[NCOLORS-(i + 256)] = RGB(255-i,255,0);
		// Green to cyan (0,255,255)
		colors[NCOLORS-(i + 512)] = RGB(0,255,i);
		// cyan to blue (0,0,255)
		colors[NCOLORS-(i + 768)] = RGB(0,255-i,255);

		//blue to black
		colors[NCOLORS-(i + 1024)] = RGB(0,0,255-i); // Fades down into black
	}
	colors[0] = RGB(0,0,0); //black
}

void get_color(int rawcolor, color_triplet * color)
{
	color->r = rawcolor;
	color->g = 0;
	color->b = 0;

}

static void init_image(void)
{
	int i;
	int j;

	for(i=0;i<60;i++)
	{
		for(j=0;j<80;j++)
		{
			lepton_image[i][j] = rand();
		}
	}
}

void copy_image(void)
{
	int i;
	int j;

	for(i=0;i<60;i++)
	{
		for(j=0;j<80;j++)
		{
			draw_lepton_image[i][j] =   lepton_image[i][j] ;
		}
	}
}

void norm_image(void)
{
	int i;
	int j;
	int max = 0;
	int min = 0xffff;

	for(i=0;i<60;i++)
	{
		for(j=0;j<80;j++)
		{
			if(draw_lepton_image[i][j] > max )
			{
				max = draw_lepton_image[i][j];
			}

			if(draw_lepton_image[i][j] < min )
			{
				min = draw_lepton_image[i][j];
			}
		}
	}

	for(i=0;i<60;i++)
	{
		for(j=0;j<80;j++)
		{
			draw_lepton_image[i][j] -= min;

			draw_lepton_image[i][j] = (( draw_lepton_image[i][j] * 255) / (max-min))+1;

		}
	}


}

void scale_image(void)
{
	int i;
	int j;
	int max = 0;
	int min = 0xffff;

	for(i=0;i<60;i++)
	{
		for(j=0;j<80;j++)
		{
			if(draw_lepton_image[i][j] > max )
			{
				max = draw_lepton_image[i][j];
			}

			if(draw_lepton_image[i][j] < min )
			{
				min = draw_lepton_image[i][j];
			}
		}
	}

	for(i=0;i<60;i++)
	{
		for(j=0;j<80;j++)
		{
			draw_lepton_image[i][j] -= min;

			draw_lepton_image[i][j] = (( draw_lepton_image[i][j] * 1280) / (max-min))+1;

		}
	}


}



#define SCALE (4)
void DrawPixels(HWND hwnd)
{
	PAINTSTRUCT ps;
	RECT r;
	HBITMAP bitmap;
	color_triplet drawcolor;


	HDC hDC = BeginPaint(hwnd, &ps);

	RECT rect;
	GetClientRect(hwnd, &rect); // To get size of window

	HDC hDCMem = CreateCompatibleDC(hDC); // Create a device context in memory
	HBITMAP memBM = CreateCompatibleBitmap ( hDC, 80, 60 );
	SelectObject ( hDCMem, memBM );


	int i;
	int j;
	for(j=0;j<80;j++)
	{
		for(i=0;i<60;i++)
		{
			//SetPixelV(hDCMem,  j, i, RGB(draw_lepton_image[i][j], 0, 0));
			//SetPixelV(hDCMem,  j, i, RGB(draw_lepton_image[i][j], draw_lepton_image[i][j], draw_lepton_image[i][j]));
			//SetPixelV(hDCMem,  j, i, RGB(table[draw_lepton_image[i][j]].r, table[draw_lepton_image[i][j]].g, table[draw_lepton_image[i][j]].b));

			SetPixelV(hDCMem,  j, i, colors[ (draw_lepton_image[i][j])%1280]);
			//get_color(draw_lepton_image[i][j], &drawcolor);
			//SetPixelV(hDCMem,  j, i, RGB(drawcolor.r, drawcolor.g, drawcolor.b));
		}
	}

	StretchBlt(hDC, 0, 0,  rect.right, rect.bottom, hDCMem,  0,  0, 80, 60, SRCCOPY);  
	
	//RestoreDC(hDCMem, savedDC);
	//DeleteDC(hDCMem);

	EndPaint(hwnd, &ps);
}

void convert_bytes_to_image(void)
{
	int i;
	int j;
	for(j=0;j<80;j++)
	{
		for(i=0;i<60;i++)
		{
			lepton_image[i][j] = image_buffer[(j+1)*((i)*2)]<<8 |   image_buffer[(j+1)*((i)*2)+1] ;
			printf("%d %d, ",(j+1)*((i)*2) ,(j+1)*((i)*2)+1 );
		}
	}

}

static void save_pgm_file(void)
{
	int i;
	int j;

	FILE *f = fopen("image.pgm", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	fprintf(f,"P2\n80 60\n255\n");
	for(i=0;i<60;i++)
	{
		for(j=0;j<80;j++)
		{
			fprintf(f,"%d ", lepton_image[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");

	fclose(f);
}

static void print_image(void)
{
	int i;
	int j;

	printf("P2\n80\n60\n255\n");
	for(i=0;i<60;i++)
	{
		for(j=0;j<80;j++)
		{
			printf("%d ", lepton_image[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}


int parser_i;
int parser_j;
void parse_binary(unsigned char input)
{
	switch(parser_state)
	{
		case 0:
			if(input == 0xde)
			{
				printf("got header\n");
				parser_state = 1;
			}
			else
			{
				printf(".");
			}

			break;
		case 1:
			if(input == 0xad)
			{
				parser_state = 2;
			}
			else if(input == 0xde)
			{
				parser_state = 1;
			}
			else
			{
				printf("error1 %x \n",input);
				parser_state = 0;
			}
			break;
		case 2:
			if(input == 0xbe)
			{
				parser_state = 3;
			}
			else
			{
				printf("error2 %x \n",input);
				parser_state = 0;
			}

			break;
		case 3:
			if(input == 0xef)
			{
				printf("got header sync\n");
				parser_state = 4;
				image_buffer_index = 0;
				parser_i = 0;
				parser_j = 0;
			}
			else
			{
				parser_state = 0;
			}
			break;
		case 4:

			if(image_buffer_index <= MAX_IMAGE_SIZE)
			{
				if(image_buffer_index%2==0)
				{
					lepton_image[parser_i][parser_j] = input<<8;
				}
				else
				{
					lepton_image[parser_i][parser_j] |= input;
					parser_j++;
					if(parser_j>=80)
					{
						parser_j = 0;
						parser_i++;
					}
				}

				image_buffer[image_buffer_index++] = input;

				if(image_buffer_index == MAX_IMAGE_SIZE)
				{
					printf("got all data\n");
					//convert_bytes_to_image();
					//print_image();
					//save_pgm_file();

					copy_image();
					//norm_image();
					scale_image();

					parser_state = 0;
					request_next_image = 1;

					InvalidateRect(mainhWnd, NULL, TRUE); 
				}
			}
			else
			{

				printf("error\n");
				parser_state = 0;
			}


			break;
	}

	if(parser_state != 0)
	{
		timeout_counter = 0;
	}


}


DWORD WINAPI MyThreadFunction( LPVOID lpParam ) 
{ 
	HANDLE hComPort=INVALID_HANDLE_VALUE;
	HANDLE hStdout;
	int i;
	signed char rxchar;
	char port[10];
	unsigned char buf[10240];
	DCB dcb;
	COMMTIMEOUTS commtimeouts;
	DWORD read;
	DWORD written;

	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = 115200;
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = FALSE;
	dcb.fTXContinueOnXoff = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.ErrorChar = FALSE;
	dcb.fNull = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fAbortOnError = FALSE;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	serial_w32SetTimeOut(hComPort,1);


	//hComPort = CreateFile(TEXT("COM8:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	hComPort = CreateFile(TEXT("COM2:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	//hComPort = CreateFile(TEXT("COM4:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (!SetupComm(hComPort, 10240, 10240))
	{
		CloseHandle(hComPort);
		fprintf(stderr, "ser_open(): can't set buffers\n\r");
		//exit(1);
	}

	if (SetCommState(hComPort, &dcb) == 0) {
		fprintf(stderr, "ser_open(): can't set com-state0\n\r");
		CloseHandle(hComPort);
	}

	if (GetCommTimeouts(hComPort, &commtimeouts) == 0) {
		fprintf(stderr, "GetCommTimeouts error\n\r");
		CloseHandle(hComPort);
		return 0;
	}

	commtimeouts.ReadIntervalTimeout = MAXDWORD;
	commtimeouts.ReadTotalTimeoutMultiplier = 0;
	commtimeouts.ReadTotalTimeoutConstant = 0;
	commtimeouts.WriteTotalTimeoutMultiplier = 10;
	commtimeouts.WriteTotalTimeoutConstant = 1000;

	if (SetCommTimeouts(hComPort, &commtimeouts) == 0) {
		fprintf(stderr, "GetCommTimeouts error\n\r");
		CloseHandle(hComPort);
		return 0;
	}

	if (!serial_w32SetTimeOut(hComPort,0))
	{
		CloseHandle(hComPort);
		fprintf(stderr, "ser_open(): can't set initial timeout\n\r");
		//exit(1);
	}

	if (hComPort == INVALID_HANDLE_VALUE) {
		printf("error!\n");
	}

	printf("port open\n");

	init_image();
	generate_pallette();
	request_next_image = 1;
	//Sleep(1000);

	//xSerialPutChar(&fd,'b',0);
	//
	



	while(1)
	{
		//if(request_next_image)
		if(0)
		{
			//printf("request image\n");
			//buf[0] = 'b';
			//WriteFile (hComPort, buf, 1, &written, NULL);
			//request_next_image = 0;
		}

		if (!ReadFile(hComPort, buf, 4, &read, NULL)) {
			LPVOID lpMsgBuf;
			FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 	);
			fprintf(stderr, "ser_recv(): read error: %s\n",(char*)lpMsgBuf);
			LocalFree( lpMsgBuf );
			//exit(1);
		}
		else
		{
			if(timeout_counter++ > 10000)
			{
				//printf("timeout");
				//buf[0] = 'r';
				//WriteFile (hComPort, buf, 1, &written, NULL);
				//request_next_image = 0;
			}
		}

		if(read)
		{
			for(i=0;i<read;i++)
			{
				parse_binary(buf[i]);
			}
		}




		//if(xSerialGetChar(&fd,&rxchar, 1))
		{
			//printf("%x",rxchar);
		}	
		//Sleep(1000);
	}


	printf("MyThreadFunction Exiting\n");




	return 0; 
} 


HACCEL hAccelerators;
HMENU hSysMenu;
MSG msg;

HWND hWndSlider;
BOOL CALLBACK AppDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInstance;
	CREATESTRUCT            *cs;
	static   DWORD          dwPos;
	LPNMUPDOWN lpnmud;
	int i;

	switch(uMsg)
	{
		case WM_PAINT:
			{
				mainhWnd = hWnd;


				DrawPixels(hWnd);
				break;
			}
		case WM_ERASEBKGND:
			return 1;
		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
	}

	return 0;
}





// Our application entry point.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	INITCOMMONCONTROLSEX icc;
	WNDCLASSEX wc;
	LPCTSTR MainWndClass = TEXT("ThermalView");
	HACCEL hAccelerators;
	HMENU hSysMenu;
	MSG msg;


	// Initialise common controls.
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	// Class for our main window.
	wc.cbSize        = sizeof(wc);
	wc.style         = 0;
	wc.lpfnWndProc   = &AppDlgProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 0, 0, LR_SHARED);
	wc.hCursor       = (HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
	wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAINMENU);
	wc.lpszClassName = MainWndClass;
	wc.hIconSm       = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, LR_SHARED);

	RedirectIOToConsole();


	//xSerialPortInitMinimal("COM8",115200, &fd);




	MyThreadHandle = CreateThread( 
			NULL ,                   // default security attributes
			0,                      // use default stack size  
			MyThreadFunction,       // thread function name
			0,          // argument to thread function 
			CREATE_SUSPENDED,                      // use default creation flags 
			NULL);   // returns the thread identifier 

	SetThreadPriority(MyThreadHandle,THREAD_PRIORITY_HIGHEST);   
	ResumeThread(MyThreadHandle);   




	Sleep(100);






	// Register our window classes, or error.
	if (! RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Error registering window class."), TEXT("Error"), MB_ICONERROR | MB_OK);
		return 0;
	}

	// Show window and force a paint.
	InitCommonControls();
	hAccelerators = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));
	DialogBoxParam(hInstance, (LPCTSTR)MAIN_DIALOG, 0, AppDlgProc, 0);

	return (int) msg.wParam;
}



// Dialog procedure for our "about" dialog.
INT_PTR CALLBACK AboutDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COMMAND:
			{
				switch (LOWORD(wParam))
				{
					case IDOK:
					case IDCANCEL:
						{
							EndDialog(hWnd, (INT_PTR) LOWORD(wParam));
							return (INT_PTR) TRUE;
						}
				}
				break;
			}

		case WM_INITDIALOG:
			return (INT_PTR) TRUE;
	}

	return (INT_PTR) FALSE;
}

