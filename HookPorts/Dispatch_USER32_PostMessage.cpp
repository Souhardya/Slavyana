#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_USER32_PostMessage.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnPostMessageA pfnPostMessageA = NULL;
__pfnPostMessageW pfnPostMessageW = NULL;



//
//Dispatch_USER32_PostMessage Functions
//
BOOL
WINAPI
OnPostMessageA(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam)
{
	BOOL bRet;

	//DWORD dwRetAddr = 0;
	//__asm
	//{
	//	mov eax, [ebp+4];
	//	sub eax, 5;
	//	mov dwRetAddr, eax;
	//}
	//if( IsBypassCaller(dwRetAddr) )
	//{
	//	bRet = pfnPostMessageA(
	//		hWnd,
	//		Msg,
	//		wParam,
	//		lParam
	//		);

	//	return bRet;
	//}

	////
	////Check if target process in SandBox
	////
	//DWORD dwProcessId = 0;
	//GetWindowThreadProcessId(hWnd, &dwProcessId);
	//if( IsProcessPatched(dwProcessId,FALSE) == FALSE )
	//{
	//	bRet = FALSE;
	//	return bRet;
	//}

	bRet = pfnPostMessageA(
		hWnd,
		Msg,
		wParam,
		lParam
		);

	return bRet;
}

BOOL
WINAPI
OnPostMessageW(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam)
{
	BOOL bRet;

	//DWORD dwRetAddr = 0;
	//__asm
	//{
	//	mov eax, [ebp+4];
	//	sub eax, 5;
	//	mov dwRetAddr, eax;
	//}
	//if( IsBypassCaller(dwRetAddr) )
	//{
	//	bRet = pfnPostMessageW(
	//		hWnd,
	//		Msg,
	//		wParam,
	//		lParam
	//		);

	//	return bRet;
	//}

	////
	////Check if target process in SandBox
	////
	//DWORD dwProcessId = 0;
	//GetWindowThreadProcessId(hWnd, &dwProcessId);
	//if( IsProcessPatched(dwProcessId,FALSE) == FALSE )
	//{
	//	bRet = FALSE;
	//	return bRet;
	//}

	bRet = pfnPostMessageW(
		hWnd,
		Msg,
		wParam,
		lParam
		);

	return bRet;
}