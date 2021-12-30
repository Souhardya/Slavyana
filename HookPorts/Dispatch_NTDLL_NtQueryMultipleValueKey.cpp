#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQueryMultipleValueKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtQueryMultipleValueKey pfnNtQueryMultipleValueKey = NULL;



//
//Dispatch_NTDLL_NtQueryMultipleValueKey Functions
//
NTSTATUS
NTAPI
OnNtQueryMultipleValueKey(
	IN HANDLE               KeyHandle,
	IN OUT PKEY_MULTIPLE_VALUE_INFORMATION ValuesList,
	IN ULONG                NumberOfValues,
	OUT PVOID               DataBuffer,
	IN OUT ULONG            BufferLength,
	OUT PULONG              RequiredLength OPTIONAL
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
		nRet = pfnNtQueryMultipleValueKey(
			KeyHandle,
			ValuesList,
			NumberOfValues,
			DataBuffer,
			BufferLength,
			RequiredLength
			);

		return nRet;
	}

	nRet = pfnNtQueryMultipleValueKey(
		KeyHandle,
		ValuesList,
		NumberOfValues,
		DataBuffer,
		BufferLength,
		RequiredLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtQueryMultipleValueKey(
	IN HANDLE               KeyHandle,
	IN OUT PKEY_MULTIPLE_VALUE_INFORMATION ValuesList,
	IN ULONG                NumberOfValues,
	OUT PVOID               DataBuffer,
	IN OUT ULONG            BufferLength,
	OUT PULONG              RequiredLength OPTIONAL
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryMultipleValueKey addFun = (__pfnNtQueryMultipleValueKey)GetProcAddress(hDll,"NtQueryMultipleValueKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,ValuesList,NumberOfValues,DataBuffer,BufferLength,RequiredLength);
	}
	return ret;
}