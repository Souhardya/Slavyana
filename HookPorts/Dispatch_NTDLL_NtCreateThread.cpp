#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtCreateThread.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtQueryInformationProcess.h"



//
//Global
//
__pfnNtCreateThread pfnNtCreateThread = NULL;



//
//Dispatch_NTDLL_NtCreateThread Functions
//
NTSTATUS
NTAPI
OnNtCreateThread(
	OUT PHANDLE             ThreadHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN HANDLE               ProcessHandle,
	OUT PCLIENT_ID          ClientId,
	IN PCONTEXT             ThreadContext,
	IN PINITIAL_TEB         InitialTeb,
	IN BOOLEAN              CreateSuspended )
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
		nRet = pfnNtCreateThread(
			ThreadHandle,
			DesiredAccess,
			ObjectAttributes,
			ProcessHandle,
			ClientId,
			ThreadContext,
			InitialTeb,
			CreateSuspended
			);

		return nRet;
	}

	nRet = pfnNtCreateThread(
		ThreadHandle,
		DesiredAccess,
		ObjectAttributes,
		ProcessHandle,
		ClientId,
		ThreadContext,
		InitialTeb,
		CreateSuspended
		);

	if(nRet == STATUS_SUCCESS)
	{
#ifdef Dbg
	DWORD dwProcessId = (DWORD)(ClientId->UniqueProcess);

	WCHAR szDebugString[256] = {0};
	wsprintf(
		szDebugString,
		L"OnNtCreateThread dwProcessId=[%d]",
		dwProcessId
		);
	DebugLog(DbgInfo,szDebugString);
#endif
	}

#ifdef Dbg
	DebugLog(DbgInfo,L"Called");
#endif

	return nRet;
}

NTSTATUS
NTAPI
NtCreateThread(
	OUT PHANDLE             ThreadHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN HANDLE               ProcessHandle,
	OUT PCLIENT_ID          ClientId,
	IN PCONTEXT             ThreadContext,
	IN PINITIAL_TEB         InitialTeb,
	IN BOOLEAN              CreateSuspended )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtCreateThread addFun = (__pfnNtCreateThread)GetProcAddress(hDll,"NtCreateThread");
	if(addFun)
	{
		ret = addFun(ThreadHandle,DesiredAccess,ObjectAttributes,ProcessHandle,ClientId,ThreadContext,InitialTeb,CreateSuspended);
	}
	return ret;
}