#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtAllocateVirtualMemory.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtAllocateVirtualMemory pfnNtAllocateVirtualMemory = NULL;



//
//Dispatch_NTDLL_NtAllocateVirtualMemory Functions
//
NTSTATUS
NTAPI
OnNtAllocateVirtualMemory(
	IN HANDLE               ProcessHandle,
	IN OUT PVOID            *BaseAddress,
	IN ULONG                ZeroBits,
	IN OUT PULONG           RegionSize,
	IN ULONG                AllocationType,
	IN ULONG                Protect )
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
		nRet = pfnNtAllocateVirtualMemory(
			ProcessHandle,
			BaseAddress,
			ZeroBits,
			RegionSize,
			AllocationType,
			Protect
			);

		return nRet;
	}

	nRet = pfnNtAllocateVirtualMemory(
		ProcessHandle,
		BaseAddress,
		ZeroBits,
		RegionSize,
		AllocationType,
		Protect
		);

#ifdef Dbg
	DebugLog(DbgInfo,L"Called");
#endif

	return nRet;
}

NTSTATUS
NTAPI
NtAllocateVirtualMemory(
	IN HANDLE               ProcessHandle,
	IN OUT PVOID            *BaseAddress,
	IN ULONG                ZeroBits,
	IN OUT PULONG           RegionSize,
	IN ULONG                AllocationType,
	IN ULONG                Protect )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtAllocateVirtualMemory addFun = (__pfnNtAllocateVirtualMemory)GetProcAddress(hDll,"NtAllocateVirtualMemory");
	if(addFun)
	{
		ret = addFun(ProcessHandle,BaseAddress,ZeroBits,RegionSize,AllocationType,Protect);
	}
	return ret;
}