#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_USER32_CreateWindowEx.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnCreateWindowExA pfnCreateWindowExA = NULL;
__pfnCreateWindowExW pfnCreateWindowExW = NULL;



//
//Dispatch_USER32_CreateWindowEx Functions
//
HWND
WINAPI
OnCreateWindowExA(
    IN DWORD dwExStyle,
    IN LPCSTR lpClassName,
    IN LPCSTR lpWindowName,
    IN DWORD dwStyle,
    IN int X,
    IN int Y,
    IN int nWidth,
    IN int nHeight,
    IN HWND hWndParent,
    IN HMENU hMenu,
    IN HINSTANCE hInstance,
    IN LPVOID lpParam)
{
	HWND hRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		hRet = pfnCreateWindowExA(
			dwExStyle,
			lpClassName,
			lpWindowName,
			dwStyle,
			X,
			Y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
			);

		return hRet;
	}

	//
	//Check if includes [WS_EX_TOOLWINDOW] Extended Window Styles
	//
	if( (dwExStyle & (WS_EX_TOOLWINDOW)) )
	{
		hRet = pfnCreateWindowExA(
			dwExStyle,
			lpClassName,
			lpWindowName,
			dwStyle,
			X,
			Y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
			);

		return hRet;
	}

	//
	//Check if includes [WS_CHILD] Window Styles
	//
	if( (dwStyle & WS_CHILD) ||
		(dwStyle & WS_CAPTION) == 0
		)
	{
		hRet = pfnCreateWindowExA(
			dwExStyle,
			lpClassName,
			lpWindowName,
			dwStyle,
			X,
			Y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
			);

		return hRet;
	}

	//
	//Check if is init Window
	//
	char strWindowName[1024] = {NULL};
	lstrcpyA(strWindowName,lpWindowName);
	if( lstrlenA(strWindowName) <= 0 )
	{
		hRet = pfnCreateWindowExA(
			dwExStyle,
			lpClassName,
			lpWindowName,
			dwStyle,
			X,
			Y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
			);

		return hRet;
	}

	//
	//Set new window title
	//
	char strNewWindowTitle[1024] = {NULL};
	wsprintfA(strNewWindowTitle,"[#] [%s] %s [#]",g_strSandBoxName,lpWindowName);

#ifdef Dbg
	OutputDebugStringA("\r\nOnCreateWindowExA strNewWindowTitle=");
	OutputDebugStringA(strNewWindowTitle);
#endif

	hRet = pfnCreateWindowExA(
		dwExStyle,
		lpClassName,
		strNewWindowTitle,
		dwStyle,
		X,
		Y,
		nWidth,
		nHeight,
		hWndParent,
		hMenu,
		hInstance,
		lpParam
		);

	return hRet;
}

HWND
WINAPI
OnCreateWindowExW(
    IN DWORD dwExStyle,
    IN LPCWSTR lpClassName,
    IN LPCWSTR lpWindowName,
    IN DWORD dwStyle,
    IN int X,
    IN int Y,
    IN int nWidth,
    IN int nHeight,
    IN HWND hWndParent,
    IN HMENU hMenu,
    IN HINSTANCE hInstance,
    IN LPVOID lpParam)
{
	HWND hRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		hRet = pfnCreateWindowExW(
			dwExStyle,
			lpClassName,
			lpWindowName,
			dwStyle,
			X,
			Y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
			);

		return hRet;
	}

	//
	//Check if includes [WS_EX_TOOLWINDOW] Extended Window Styles
	//
	if( (dwExStyle & WS_EX_TOOLWINDOW) )
	{
		hRet = pfnCreateWindowExW(
			dwExStyle,
			lpClassName,
			lpWindowName,
			dwStyle,
			X,
			Y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
			);

		return hRet;
	}

	//
	//Check if includes [WS_CHILD] Window Styles
	//
	if( (dwStyle & WS_CHILD) )
	{
		hRet = pfnCreateWindowExW(
			dwExStyle,
			lpClassName,
			lpWindowName,
			dwStyle,
			X,
			Y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
			);

		return hRet;
	}

	//
	//Check if is init Window
	//
	WCHAR szWindowName[1024] = {NULL};
	lstrcpyW(szWindowName,lpWindowName);
	if( lstrlenW(szWindowName) <= 0 ||
		wcsicmp(szWindowName,L"MSCTFIME UI") == 0 ||
		wcsicmp(szWindowName,L"OleMainThreadWndName") == 0
		)
	{
		hRet = pfnCreateWindowExW(
			dwExStyle,
			lpClassName,
			lpWindowName,
			dwStyle,
			X,
			Y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
			);

		return hRet;
	}

	//
	//Set new window title
	//
	WCHAR szNewWindowTitle[1024] = {NULL};
	wsprintfW(szNewWindowTitle,L"[#] [%s] %s [#]",g_szSandBoxName,lpWindowName);

#ifdef Dbg
	OutputDebugStringW(L"\r\nOnCreateWindowExW szNewWindowTitle=");
	OutputDebugStringW(szNewWindowTitle);
#endif

	hRet = pfnCreateWindowExW(
		dwExStyle,
		lpClassName,
		szNewWindowTitle,
		dwStyle,
		X,
		Y,
		nWidth,
		nHeight,
		hWndParent,
		hMenu,
		hInstance,
		lpParam
		);

	return hRet;
}