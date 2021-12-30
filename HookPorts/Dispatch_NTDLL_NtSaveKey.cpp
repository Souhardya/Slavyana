#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtSaveKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtSaveKey pfnNtSaveKey = NULL;



//
//Dispatch_NTDLL_NtSaveKey Functions
//
NTSTATUS
NTAPI
OnNtSaveKey(
	IN HANDLE               KeyHandle,
	IN HANDLE               FileHandle
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
		nRet = pfnNtSaveKey(
			KeyHandle,
			FileHandle
			);

		return nRet;
	}

	nRet = pfnNtSaveKey(
		KeyHandle,
		FileHandle
		);

	return nRet;
}

NTSTATUS
NTAPI
NtSaveKey(
	IN HANDLE               KeyHandle,
	IN HANDLE               FileHandle
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtSaveKey addFun = (__pfnNtSaveKey)GetProcAddress(hDll,"NtSaveKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,FileHandle);
	}
	return ret;
}