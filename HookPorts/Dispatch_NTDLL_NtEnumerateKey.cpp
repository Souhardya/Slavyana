#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtEnumerateKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtEnumerateKey pfnNtEnumerateKey = NULL;



//
//Dispatch_NTDLL_NtEnumerateKey Functions
//
NTSTATUS
NTAPI
OnNtEnumerateKey(
	IN HANDLE  KeyHandle,
	IN ULONG  Index,
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
		nRet = pfnNtEnumerateKey(
			KeyHandle,
			Index,
			KeyInformationClass,
			KeyInformation,
			Length,
			ResultLength
			);

		return nRet;
	}

	nRet = pfnNtEnumerateKey(
		KeyHandle,
		Index,
		KeyInformationClass,
		KeyInformation,
		Length,
		ResultLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtEnumerateKey(
	IN HANDLE  KeyHandle,
	IN ULONG  Index,
	IN KEY_INFORMATION_CLASS  KeyInformationClass,
	OUT PVOID  KeyInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtEnumerateKey addFun = (__pfnNtEnumerateKey)GetProcAddress(hDll,"NtEnumerateKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,Index,KeyInformationClass,KeyInformation,Length,ResultLength);
	}
	return ret;
}