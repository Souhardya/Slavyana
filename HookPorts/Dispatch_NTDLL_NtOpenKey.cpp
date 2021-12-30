#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtOpenKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtOpenKey pfnNtOpenKey = NULL;



//
//Dispatch_NTDLL_NtOpenKey Functions
//
NTSTATUS
NTAPI
OnNtOpenKey(
	OUT PHANDLE  KeyHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes
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
		nRet = pfnNtOpenKey(
			KeyHandle,
			DesiredAccess,
			ObjectAttributes
			);

		return nRet;
	}

	nRet = pfnNtOpenKey(
		KeyHandle,
		DesiredAccess,
		ObjectAttributes
		);

	return nRet;
}

NTSTATUS
NTAPI
NtOpenKey(
	OUT PHANDLE  KeyHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtOpenKey addFun = (__pfnNtOpenKey)GetProcAddress(hDll,"NtOpenKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,DesiredAccess,ObjectAttributes);
	}
	return ret;
}