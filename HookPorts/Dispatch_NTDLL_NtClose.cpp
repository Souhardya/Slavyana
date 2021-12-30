#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtClose.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtClose pfnNtClose = NULL;



//
//Dispatch_NTDLL_NtClose Functions
//
NTSTATUS
NTAPI
OnNtClose(
    IN HANDLE  Handle
    )
{
	NTSTATUS nRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		nRet = pfnNtClose(
			Handle
			);

		return nRet;
	}

	nRet = pfnNtClose(
		Handle
		);

	return nRet;
}

NTSTATUS
NTAPI
NtClose(
    IN HANDLE  Handle
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtClose addFun = (__pfnNtClose)GetProcAddress(hDll,"NtClose");
	if(addFun)
	{
		ret = addFun(Handle);
	}
	return ret;
}