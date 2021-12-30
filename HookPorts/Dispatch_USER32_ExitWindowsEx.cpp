#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_USER32_ExitWindowsEx.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnExitWindowsEx pfnExitWindowsEx = NULL;



//
//Dispatch_USER32_ExitWindowsEx Functions
//
BOOL
WINAPI
OnExitWindowsEx(
    IN UINT uFlags,
    IN DWORD dwReserved)
{
	DWORD dwRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		dwRet = pfnExitWindowsEx(
			uFlags,
			dwReserved
			);

		return dwRet;
	}

	//
	//Refuse directly
	//
	return FALSE;
}