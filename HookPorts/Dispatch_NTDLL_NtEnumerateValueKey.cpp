#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtEnumerateValueKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtEnumerateValueKey pfnNtEnumerateValueKey = NULL;



//
//Dispatch_NTDLL_NtEnumerateValueKey Functions
//
NTSTATUS
NTAPI
OnNtEnumerateValueKey(
	IN HANDLE  KeyHandle,
	IN ULONG  Index,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInformationClass,
	OUT PVOID  KeyValueInformation,
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
		nRet = pfnNtEnumerateValueKey(
			KeyHandle,
			Index,
			KeyValueInformationClass,
			KeyValueInformation,
			Length,
			ResultLength
			);

		return nRet;
	}

	nRet = pfnNtEnumerateValueKey(
		KeyHandle,
		Index,
		KeyValueInformationClass,
		KeyValueInformation,
		Length,
		ResultLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtEnumerateValueKey(
	IN HANDLE  KeyHandle,
	IN ULONG  Index,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInformationClass,
	OUT PVOID  KeyValueInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtEnumerateValueKey addFun = (__pfnNtEnumerateValueKey)GetProcAddress(hDll,"NtEnumerateValueKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,Index,KeyValueInformationClass,KeyValueInformation,Length,ResultLength);
	}
	return ret;
}