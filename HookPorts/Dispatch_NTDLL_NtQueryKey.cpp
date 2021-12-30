#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQueryKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtQueryKey pfnNtQueryKey = NULL;



//
//Dispatch_NTDLL_NtQueryKey Functions
//
NTSTATUS
NTAPI
OnNtQueryKey(
	IN HANDLE  KeyHandle,
	IN KEY_INFORMATION_CLASS  KeyInformationClass,
	OUT PVOID  KeyInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
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
		nRet = pfnNtQueryKey(
			KeyHandle,
			KeyInformationClass,
			KeyInformation,
			Length,
			ResultLength
			);

		return nRet;
	}

	nRet = pfnNtQueryKey(
		KeyHandle,
		KeyInformationClass,
		KeyInformation,
		Length,
		ResultLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtQueryKey(
	IN HANDLE  KeyHandle,
	IN KEY_INFORMATION_CLASS  KeyInformationClass,
	OUT PVOID  KeyInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryKey addFun = (__pfnNtQueryKey)GetProcAddress(hDll,"NtQueryKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,KeyInformationClass,KeyInformation,Length,ResultLength);
	}
	return ret;
}