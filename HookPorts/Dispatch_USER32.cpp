#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_USER32.h"
#include "./InlineHook.h"
#include "../Common/DebugLog.h"

#include "./Dispatch_USER32_CreateWindowEx.h"
#include "./Dispatch_USER32_ExitWindowsEx.h"
#include "./Dispatch_USER32_GetWindowThreadProcessId.h"
#include "./Dispatch_USER32_PostMessage.h"
#include "./Dispatch_USER32_SendMessage.h"
#include "./Dispatch_USER32_SetWindowLong.h"
#include "./Dispatch_USER32_SetWindowsHookEx.h"
#include "./Dispatch_USER32_SetWindowText.h"
#include "./Dispatch_USER32_UnhookWindowsHookEx.h"



//
//Global
//
BOOL bUSER32Patched = FALSE;



//
//Dispatch_USER32 Functions
//
int Dispatch_USER32_Start(void)
{
	//Return Value:
	//-1 = error
	//0 = patched
	//1 = succeed

	if( bUSER32Patched == TRUE )
	{
		return 0;
	}

	HINSTANCE hUSER32 = NULL;
	hUSER32 = LoadLibrary(L"user32.dll");

	//
	//Patch API
	//

	//CreateWindowExA
	InlineHook(
		(__pfnCreateWindowExA)GetProcAddress(hUSER32,"CreateWindowExA"),
		OnCreateWindowExA,
		(void **)&pfnCreateWindowExA
		);
	//CreateWindowExW
	InlineHook(
		(__pfnCreateWindowExW)GetProcAddress(hUSER32,"CreateWindowExW"),
		OnCreateWindowExW,
		(void **)&pfnCreateWindowExW
		);

	//ExitWindowsEx
	InlineHook(
		(__pfnExitWindowsEx)GetProcAddress(hUSER32,"ExitWindowsEx"),
		OnExitWindowsEx,
		(void **)&pfnExitWindowsEx
		);

	//GetWindowThreadProcessId
	InlineHook(
		(__pfnGetWindowThreadProcessId)GetProcAddress(hUSER32,"GetWindowThreadProcessId"),
		OnGetWindowThreadProcessId,
		(void **)&pfnGetWindowThreadProcessId
		);

	//PostMessageA
	InlineHook(
		(__pfnPostMessageA)GetProcAddress(hUSER32,"PostMessageA"),
		OnPostMessageA,
		(void **)&pfnPostMessageA
		);
	//PostMessageW
	InlineHook(
		(__pfnPostMessageW)GetProcAddress(hUSER32,"PostMessageW"),
		OnPostMessageW,
		(void **)&pfnPostMessageW
		);

	//SendMessageA
	InlineHook(
		(__pfnSendMessageA)GetProcAddress(hUSER32,"SendMessageA"),
		OnSendMessageA,
		(void **)&pfnSendMessageA
		);
	//SendMessageW
	InlineHook(
		(__pfnSendMessageW)GetProcAddress(hUSER32,"SendMessageW"),
		OnSendMessageW,
		(void **)&pfnSendMessageW
		);

	//SetWindowLongW
	InlineHook(
		(__pfnSetWindowLongW)GetProcAddress(hUSER32,"SetWindowLongW"),
		OnSetWindowLongW,
		(void **)&pfnSetWindowLongW
		);

	//SetWindowsHookExW
	InlineHook(
		(__pfnSetWindowsHookExW)GetProcAddress(hUSER32,"SetWindowsHookExW"),
		OnSetWindowsHookExW,
		(void **)&pfnSetWindowsHookExW
		);

	//SetWindowTextA
	InlineHook(
		(__pfnSetWindowTextA)GetProcAddress(hUSER32,"SetWindowTextA"),
		OnSetWindowTextA,
		(void **)&pfnSetWindowTextA
		);
	//SetWindowTextW
	InlineHook(
		(__pfnSetWindowTextW)GetProcAddress(hUSER32,"SetWindowTextW"),
		OnSetWindowTextW,
		(void **)&pfnSetWindowTextW
		);

	//UnhookWindowsHookEx
	InlineHook(
		(__pfnUnhookWindowsHookEx)GetProcAddress(hUSER32,"UnhookWindowsHookEx"),
		OnUnhookWindowsHookEx,
		(void **)&pfnUnhookWindowsHookEx
		);

	return 1;
}