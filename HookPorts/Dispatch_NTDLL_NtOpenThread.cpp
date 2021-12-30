#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtOpenThread.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"
#include "./Dispatch_NTDLL_NtQueryInformationThread.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtOpenProcess.h"
#include "./Dispatch_NTDLL_NtClose.h"



//
//Global
//
__pfnNtOpenThread pfnNtOpenThread = NULL;



//
//Dispatch_NTDLL_NtOpenThread Functions
//
NTSTATUS
NTAPI
OnNtOpenThread(
	OUT PHANDLE             ThreadHandle,
	IN ACCESS_MASK          AccessMask,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN PCLIENT_ID           ClientId )
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
		nRet = pfnNtOpenThread(
			ThreadHandle,
			AccessMask,
			ObjectAttributes,
			ClientId
			);

		return nRet;
	}

	//
	//Check AccessMask
	//
	if (AccessMask & (DELETE|
						WRITE_DAC |
						WRITE_OWNER |
						THREAD_TERMINATE|
						THREAD_SUSPEND_RESUME|
						THREAD_ALERT|
						THREAD_SET_CONTEXT|
						THREAD_SET_INFORMATION|
						THREAD_SET_THREAD_TOKEN))
	{
		//
		//OpenThread to query information
		//
		NTSTATUS nNtOpenThreadStatus;
		HANDLE hThread = NULL;
		ACCESS_MASK ThreadAccessMask = THREAD_QUERY_INFORMATION;
		nNtOpenThreadStatus = NtOpenThread(
											&hThread,
											ThreadAccessMask,
											ObjectAttributes,
											ClientId
											);

		if(nNtOpenThreadStatus == STATUS_SUCCESS)
		{
			//
			//Query ProcessId of Caller
			//
			NTSTATUS nStatus;
			THREAD_BASIC_INFORMATION threadinfo;
			DWORD dwProcessId = 0;

			//Query information of thread
			nStatus = NtQueryInformationThread(
				hThread,
				ThreadBasicInformation,
				(PVOID)&threadinfo,
				sizeof(THREAD_BASIC_INFORMATION),
				NULL);

			if(nStatus == STATUS_SUCCESS)
			{
#ifdef Dbg
				dwProcessId = (DWORD)(threadinfo.ClientId.UniqueProcess);

				WCHAR szDebugString[256] = {0};
				wsprintf(
					szDebugString,
					L"OnNtOpenThread dwProcessId=[%d]",
					dwProcessId
					);
				DebugLog(DbgInfo,szDebugString);
#endif
				if( IsProcessPatched(dwProcessId,FALSE) == FALSE )//Outside SandBox
				{
					NtClose(hThread);
					return STATUS_ACCESS_DENIED;
				}
				else//Inside SandBox
				{
					NtClose(hThread);
					nRet = pfnNtOpenThread(
						ThreadHandle,
						AccessMask,
						ObjectAttributes,
						ClientId
						);

					return nRet;
				}
			}//nStatus
		}//nNtOpenThreadStatus
		NtClose(hThread);
	}

	nRet = pfnNtOpenThread(
		ThreadHandle,
		AccessMask,
		ObjectAttributes,
		ClientId
		);

	return nRet;
}

NTSTATUS
NTAPI
NtOpenThread(
	OUT PHANDLE             ThreadHandle,
	IN ACCESS_MASK          AccessMask,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN PCLIENT_ID           ClientId )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtOpenThread addFun = (__pfnNtOpenThread)GetProcAddress(hDll,"NtOpenThread");
	if(addFun)
	{
		ret = addFun(ThreadHandle,AccessMask,ObjectAttributes,ClientId);
	}
	return ret;
}