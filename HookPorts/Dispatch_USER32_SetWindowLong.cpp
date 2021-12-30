#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_USER32_SetWindowLong.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnSetWindowLongW pfnSetWindowLongW = NULL;



//
//Dispatch_USER32_SetWindowLong Functions
//
LONG
WINAPI
OnSetWindowLongW(
    IN HWND hWnd,
    IN int nIndex,
    IN LONG dwNewLong)
{
	LONG lRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		lRet = pfnSetWindowLongW(
			hWnd,
			nIndex,
			dwNewLong
			);

		return lRet;
	}

	//
	//Check if target process in SandBox
	//
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if( IsProcessPatched(dwProcessId,FALSE) == FALSE )
	{
		lRet = 0;
		return lRet;
	}

	lRet = pfnSetWindowLongW(
		hWnd,
		nIndex,
		dwNewLong
		);

	return lRet;
}