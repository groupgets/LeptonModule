#include <windows.h>
#include <stdio.h>



void RedirectIOToConsole()
{
	AllocConsole();
	freopen("CONIN$","rb",stdin);   // reopen stdin handle as console window input
	freopen("CONOUT$","wb",stdout);  // reopen stout handle as console window output
	freopen("CONOUT$","wb",stderr); // reopen stderr handle as console window output
}


