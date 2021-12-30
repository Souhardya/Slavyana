#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_USER32_GetWindowThreadProcessId.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnGetWindowThreadProcessId pfnGetWindowThreadProcessId = NULL;



//
//Dispatch_USER32_GetWindowThreadProcessId Functions
//
DWORD
WINAPI
OnGetWindowThreadProcessId(
    IN HWND hWnd,
    OUT LPDWORD lpdwProcessId)
{
	DWORD dwRet;

	//DWORD dwRetAddr = 0;
	//__asm
	//{
	//	mov eax, [ebp+4];
	//	sub eax, 5;
	//	mov dwRetAddr, eax;
	//}
	//if( IsBypassCaller(dwRetAddr) )
	//{
	//	dwRet = pfnGetWindowThreadProcessId(
	//		hWnd,
	//		lpdwProcessId
	//		);

	//	return dwRet;
	//}

	dwRet = pfnGetWindowThreadProcessId(
		hWnd,
		lpdwProcessId
		);

	return dwRet;
}