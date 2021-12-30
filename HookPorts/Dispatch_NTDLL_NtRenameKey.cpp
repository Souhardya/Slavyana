#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtRenameKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtRenameKey pfnNtRenameKey = NULL;



//
//Dispatch_NTDLL_NtRenameKey Functions
//
NTSTATUS
NTAPI
OnNtRenameKey(
	IN HANDLE			KeyHandle,
	IN PUNICODE_STRING	NewName
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
		nRet = pfnNtRenameKey(
			KeyHandle,
			NewName
			);

		return nRet;
	}

	nRet = pfnNtRenameKey(
		KeyHandle,
		NewName
		);

	return nRet;
}

NTSTATUS
NTAPI
NtRenameKey(
	IN HANDLE			KeyHandle,
	IN PUNICODE_STRING	NewName
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtRenameKey addFun = (__pfnNtRenameKey)GetProcAddress(hDll,"NtRenameKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,NewName);
	}
	return ret;
}