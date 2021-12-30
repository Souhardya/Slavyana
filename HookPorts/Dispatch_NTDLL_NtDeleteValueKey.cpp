#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtDeleteValueKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtDeleteValueKey pfnNtDeleteValueKey = NULL;



//
//Dispatch_NTDLL_NtDeleteValueKey Functions
//
NTSTATUS
NTAPI
OnNtDeleteValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName
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
		nRet = pfnNtDeleteValueKey(
			KeyHandle,
			ValueName
			);

		return nRet;
	}

	nRet = pfnNtDeleteValueKey(
		KeyHandle,
		ValueName
		);

	return nRet;
}

NTSTATUS
NTAPI
NtDeleteValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtDeleteValueKey addFun = (__pfnNtDeleteValueKey)GetProcAddress(hDll,"NtDeleteValueKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,ValueName);
	}
	return ret;
}