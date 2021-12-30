#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtLoadDriver.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"



//
//Global
//
__pfnNtLoadDriver pfnNtLoadDriver = NULL;



//
//Dispatch_NTDLL_NtCreateThread Functions
//
NTSTATUS
NTAPI
OnNtLoadDriver(
	IN PUNICODE_STRING DriverServiceName )
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
		nRet = pfnNtLoadDriver(
			DriverServiceName
			);

		return nRet;
	}

	//
	//Refuse directly
	//
	return STATUS_ACCESS_DENIED;
}

NTSTATUS
NTAPI
NtLoadDriver(
	IN PUNICODE_STRING DriverServiceName )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtLoadDriver addFun = (__pfnNtLoadDriver)GetProcAddress(hDll,"NtLoadDriver");
	if(addFun)
	{
		ret = addFun(DriverServiceName);
	}
	return ret;
}