#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtProtectVirtualMemory.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtProtectVirtualMemory pfnNtProtectVirtualMemory = NULL;



//
//Dispatch_NTDLL_NtProtectVirtualMemory Functions
//
NTSTATUS
NTAPI
OnNtProtectVirtualMemory(
	IN HANDLE               ProcessHandle,
	IN OUT PVOID            *BaseAddress,
	IN OUT PULONG           NumberOfBytesToProtect,
	IN ULONG                NewAccessProtection,
	OUT PULONG              OldAccessProtection )
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
		nRet = pfnNtProtectVirtualMemory(
			ProcessHandle,
			BaseAddress,
			NumberOfBytesToProtect,
			NewAccessProtection,
			OldAccessProtection
			);

		return nRet;
	}

	nRet = pfnNtProtectVirtualMemory(
		ProcessHandle,
		BaseAddress,
		NumberOfBytesToProtect,
		NewAccessProtection,
		OldAccessProtection
		);

	return nRet;
}

NTSTATUS
NTAPI
NtProtectVirtualMemory(
	IN HANDLE               ProcessHandle,
	IN OUT PVOID            *BaseAddress,
	IN OUT PULONG           NumberOfBytesToProtect,
	IN ULONG                NewAccessProtection,
	OUT PULONG              OldAccessProtection )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtProtectVirtualMemory addFun = (__pfnNtProtectVirtualMemory)GetProcAddress(hDll,"NtProtectVirtualMemory");
	if(addFun)
	{
		ret = addFun(ProcessHandle,BaseAddress,NumberOfBytesToProtect,NewAccessProtection,OldAccessProtection);
	}
	return ret;
}