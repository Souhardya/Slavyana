#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQueryInformationProcess.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtQueryInformationProcess pfnNtQueryInformationProcess = NULL;



//
//Dispatch_NTDLL_NtQueryInformationProcess Functions
//
NTSTATUS
NTAPI
OnNtQueryInformationProcess(
	IN HANDLE               ProcessHandle,
	IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
	OUT PVOID               ProcessInformation,
	IN ULONG                ProcessInformationLength,
	OUT PULONG              ReturnLength )
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
		nRet = pfnNtQueryInformationProcess(
			ProcessHandle,
			ProcessInformationClass,
			ProcessInformation,
			ProcessInformationLength,
			ReturnLength
			);

		return nRet;
	}

	nRet = pfnNtQueryInformationProcess(
		ProcessHandle,
		ProcessInformationClass,
		ProcessInformation,
		ProcessInformationLength,
		ReturnLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtQueryInformationProcess(
	IN HANDLE               ProcessHandle,
	IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
	OUT PVOID               ProcessInformation,
	IN ULONG                ProcessInformationLength,
	OUT PULONG              ReturnLength )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryInformationProcess addFun = (__pfnNtQueryInformationProcess)GetProcAddress(hDll,"NtQueryInformationProcess");
	if(addFun)
	{
		ret = addFun(ProcessHandle,ProcessInformationClass,ProcessInformation,ProcessInformationLength,ReturnLength);
	}
	return ret;
}