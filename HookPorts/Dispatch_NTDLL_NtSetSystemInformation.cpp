#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtSetSystemInformation.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtSetSystemInformation pfnNtSetSystemInformation = NULL;



//
//Dispatch_NTDLL_NtCreateThread Functions
//
NTSTATUS
NTAPI
OnNtSetSystemInformation(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	IN PVOID                SystemInformation,
	IN ULONG                SystemInformationLength )
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
		nRet = pfnNtSetSystemInformation(
			SystemInformationClass,
			SystemInformation,
			SystemInformationLength
			);

		return nRet;
	}

	if( SystemInformation == NULL )
	{
		return STATUS_INVALID_INFO_CLASS;
	}

	if( SystemInformationClass == SystemLoadAndCallImage )
	{
		return STATUS_INVALID_INFO_CLASS;
	}

	nRet = pfnNtSetSystemInformation(
		SystemInformationClass,
		SystemInformation,
		SystemInformationLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtSetSystemInformation(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	IN PVOID                SystemInformation,
	IN ULONG                SystemInformationLength )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtSetSystemInformation addFun = (__pfnNtSetSystemInformation)GetProcAddress(hDll,"NtSetSystemInformation");
	if(addFun)
	{
		ret = addFun(SystemInformationClass,SystemInformation,SystemInformationLength);
	}
	return ret;
}