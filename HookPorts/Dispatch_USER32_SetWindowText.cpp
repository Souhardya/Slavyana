#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_USER32_SetWindowText.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnSetWindowTextA pfnSetWindowTextA = NULL;
__pfnSetWindowTextW pfnSetWindowTextW = NULL;



//
//Dispatch_USER32_SetWindowText Functions
//
BOOL
WINAPI
OnSetWindowTextA(
    IN HWND hWnd,
    IN LPCSTR lpString)
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
		bRet = pfnSetWindowTextA(
			hWnd,
			lpString
			);

		return bRet;
	}

	//
	//Check if target process in SandBox
	//
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if( IsProcessPatched(dwProcessId,FALSE) == FALSE )
	{
		bRet = FALSE;
		return bRet;
	}

	//
	//Check if includes [WS_CAPTION] Window Styles
	//
	LONG lStylesValue = GetWindowLong(hWnd,GWL_STYLE);
	if( lStylesValue == 0 )
	{
		bRet = pfnSetWindowTextA(
			hWnd,
			lpString
			);

		return bRet;
	}
	if( (lStylesValue & WS_CAPTION) == 0 )
	{
		bRet = pfnSetWindowTextA(
			hWnd,
			lpString
			);

		return bRet;
	}

	//
	//Set new window title
	//
	char szNewlpString[1024] = {0};
	wsprintfA(szNewlpString,"[#] [%s] %s [#]",g_strSandBoxName,lpString);

	bRet = pfnSetWindowTextA(
		hWnd,
		szNewlpString
		);

#ifdef Dbg
	OutputDebugStringA("SetWindowTextA szNewlpString=");
	OutputDebugStringA(szNewlpString);
#endif

	return bRet;
}

BOOL
WINAPI
OnSetWindowTextW(
    IN HWND hWnd,
    IN LPCWSTR lpString)
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
		bRet = pfnSetWindowTextW(
			hWnd,
			lpString
			);

		return bRet;
	}

	//
	//Check if target process in SandBox
	//
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if( IsProcessPatched(dwProcessId,FALSE) == FALSE )
	{
		bRet = FALSE;
		return bRet;
	}

	//
	//Check if includes [WS_CAPTION] Window Styles
	//
	LONG lStylesValue = GetWindowLong(hWnd,GWL_STYLE);
	if( lStylesValue == 0 )
	{
		bRet = pfnSetWindowTextW(
			hWnd,
			lpString
			);

		return bRet;
	}
	if( (lStylesValue & WS_CAPTION) == 0 )
	{
		bRet = pfnSetWindowTextW(
			hWnd,
			lpString
			);

		return bRet;
	}

	//
	//Set new window title
	//
	WCHAR szNewlpString[1024] = {0};
	wsprintfW(szNewlpString,L"[#] [%s] %s [#]",g_szSandBoxName,lpString);

	bRet = pfnSetWindowTextW(
		hWnd,
		szNewlpString
		);

#ifdef Dbg
	OutputDebugStringW(L"SetWindowTextW szNewlpString=");
	OutputDebugStringW(szNewlpString);
#endif

	return bRet;
}