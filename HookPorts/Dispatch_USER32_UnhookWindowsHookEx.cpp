#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_USER32_UnhookWindowsHookEx.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnUnhookWindowsHookEx pfnUnhookWindowsHookEx = NULL;



//
//Dispatch_USER32_UnhookWindowsHookEx Functions
//
BOOL
WINAPI
OnUnhookWindowsHookEx(
	IN HHOOK hhk)
{
	BOOL bRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		bRet = pfnUnhookWindowsHookEx(
			hhk
			);

		return bRet;
	}

	bRet = pfnUnhookWindowsHookEx(
		hhk
		);

	return bRet;
}