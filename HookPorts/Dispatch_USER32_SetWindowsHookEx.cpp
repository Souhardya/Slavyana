#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_USER32_SetWindowsHookEx.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnSetWindowsHookExW pfnSetWindowsHookExW = NULL;



//
//Dispatch_USER32_SetWindowsHookEx Functions
//
HHOOK
WINAPI
OnSetWindowsHookExW(
	IN int idHook,
	IN HOOKPROC lpfn,
	IN HINSTANCE hmod,
	IN DWORD dwThreadId)
{
	HHOOK hRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		hRet = pfnSetWindowsHookExW(
			idHook,
			lpfn,
			hmod,
			dwThreadId
			);

		return hRet;
	}

	//
	//Parameters
	//
	//dwThreadId
	//If this parameter is zero, the hook procedure is associated with all existing threads running in the same desktop as the calling thread.

	//
	//Check if is global hook
	//
	if( dwThreadId == 0 )
	{
		hRet = NULL;
		return hRet;
	}

	hRet = pfnSetWindowsHookExW(
		idHook,
		lpfn,
		hmod,
		dwThreadId
		);

	return hRet;
}