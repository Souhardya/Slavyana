#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_KERNEL32.h"
#include "./InlineHook.h"
#include "../Common/DebugLog.h"

#include "./Dispatch_KERNEL32_CreateProcessInternal.h"



//
//Global
//
BOOL bKERNEL32Patched = FALSE;



//
//Dispatch_KERNEL32 Functions
//
int Dispatch_KERNEL32_Start(void)
{
	//Return Value:
	//-1 = error
	//0 = patched
	//1 = succeed

	if( bKERNEL32Patched == TRUE )
	{
		return 0;
	}

	HINSTANCE hKERNEL32 = NULL;
	hKERNEL32 = LoadLibrary(L"kernel32.dll");

	//
	//Patch API
	//

	//CreateProcessInternalW
	InlineHook(
		(__pfnCreateProcessInternalW)GetProcAddress(hKERNEL32,"CreateProcessInternalW"),
		OnCreateProcessInternalW,
		(void **)&pfnCreateProcessInternalW
		);

	return 1;
}