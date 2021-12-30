#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtWriteVirtualMemory.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtWriteVirtualMemory pfnNtWriteVirtualMemory = NULL;



//
//Dispatch_NTDLL_NtWriteVirtualMemory Functions
//
NTSTATUS
NTAPI
OnNtWriteVirtualMemory(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	IN PVOID                Buffer,
	IN ULONG                NumberOfBytesToWrite,
	OUT PULONG              NumberOfBytesWritten OPTIONAL )
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
		nRet = pfnNtWriteVirtualMemory(
			ProcessHandle,
			BaseAddress,
			Buffer,
			NumberOfBytesToWrite,
			NumberOfBytesWritten
			);

		return nRet;
	}

	nRet = pfnNtWriteVirtualMemory(
		ProcessHandle,
		BaseAddress,
		Buffer,
		NumberOfBytesToWrite,
		NumberOfBytesWritten
		);

	return nRet;
}

NTSTATUS
NTAPI
NtWriteVirtualMemory(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	IN PVOID                Buffer,
	IN ULONG                NumberOfBytesToWrite,
	OUT PULONG              NumberOfBytesWritten OPTIONAL )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtWriteVirtualMemory addFun = (__pfnNtWriteVirtualMemory)GetProcAddress(hDll,"NtWriteVirtualMemory");
	if(addFun)
	{
		ret = addFun(ProcessHandle,BaseAddress,Buffer,NumberOfBytesToWrite,NumberOfBytesWritten);
	}
	return ret;
}