#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtDeleteKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtDeleteKey pfnNtDeleteKey = NULL;



//
//Dispatch_NTDLL_NtDeleteKey Functions
//
NTSTATUS
NTAPI
OnNtDeleteKey(
	IN HANDLE  KeyHandle
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
		nRet = pfnNtDeleteKey(
			KeyHandle
			);

		return nRet;
	}

	nRet = pfnNtDeleteKey(
		KeyHandle
		);

	return nRet;
}

NTSTATUS
NTAPI
NtDeleteKey(
	IN HANDLE  KeyHandle
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtDeleteKey addFun = (__pfnNtDeleteKey)GetProcAddress(hDll,"NtDeleteKey");
	if(addFun)
	{
		ret = addFun(KeyHandle);
	}
	return ret;
}