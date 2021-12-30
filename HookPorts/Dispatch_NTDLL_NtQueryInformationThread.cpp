#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQueryInformationThread.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtQueryInformationThread pfnNtQueryInformationThread = NULL;



//
//Dispatch_NTDLL_NtQueryInformationThread Functions
//
NTSTATUS
NTAPI
OnNtQueryInformationThread(
	IN HANDLE               ThreadHandle,
	IN THREAD_INFORMATION_CLASS ThreadInformationClass,
	OUT PVOID               ThreadInformation,
	IN ULONG                ThreadInformationLength,
	OUT PULONG              ReturnLength OPTIONAL )
{
	NTSTATUS nRet;

	//DWORD dwRetAddr = 0;
	//__asm
	//{
	//	mov eax, [ebp+4];
	//	sub eax, 5;
	//	mov dwRetAddr, eax;
	//}
	//if( IsBypassCaller(dwRetAddr) )
	//{
	//	nRet = pfnNtQueryInformationThread(
	//		ThreadHandle,
	//		ThreadInformationClass,
	//		ThreadInformation,
	//		ThreadInformationLength,
	//		ReturnLength
	//		);

	//	return nRet;
	//}

	nRet = pfnNtQueryInformationThread(
		ThreadHandle,
		ThreadInformationClass,
		ThreadInformation,
		ThreadInformationLength,
		ReturnLength
		);

//#ifdef Dbg
//	DebugLog(DbgInfo,L"Called");
//#endif

	return nRet;
}

NTSTATUS
NTAPI
NtQueryInformationThread(
	IN HANDLE               ThreadHandle,
	IN THREAD_INFORMATION_CLASS ThreadInformationClass,
	OUT PVOID               ThreadInformation,
	IN ULONG                ThreadInformationLength,
	OUT PULONG              ReturnLength OPTIONAL )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryInformationThread addFun = (__pfnNtQueryInformationThread)GetProcAddressEx(hDll,"NtQueryInformationThread");
	if(addFun)
	{
		ret = addFun(ThreadHandle,ThreadInformationClass,ThreadInformation,ThreadInformationLength,ReturnLength);
	}
	return ret;
}