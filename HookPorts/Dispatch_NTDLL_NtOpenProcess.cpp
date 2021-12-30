#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtOpenProcess.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"



//
//Global
//
__pfnNtOpenProcess pfnNtOpenProcess = NULL;



//
//Dispatch_NTDLL_NtOpenProcess Functions
//
NTSTATUS
NTAPI
OnNtOpenProcess(
    OUT PHANDLE ProcessHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN PCLIENT_ID ClientId OPTIONAL
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
		nRet = pfnNtOpenProcess(
			ProcessHandle,
			DesiredAccess,
			ObjectAttributes,
			ClientId
			);

		return nRet;
	}

	//
	//Check if target process in SandBox
	//
	if( ClientId != NULL )
	{
		if( IsProcessPatched((DWORD)ClientId->UniqueProcess,FALSE) == FALSE )
		{
			if(DesiredAccess & (PROCESS_TERMINATE |
								PROCESS_CREATE_PROCESS|
								PROCESS_CREATE_THREAD |
								PROCESS_SET_SESSIONID |
								PROCESS_VM_OPERATION |
								PROCESS_VM_READ |
								PROCESS_VM_WRITE |
								/*PROCESS_DUP_HANDLE |*/
								PROCESS_SET_INFORMATION |
								DELETE |
								WRITE_DAC |
								WRITE_OWNER |
								PROCESS_SUSPEND_RESUME |
								PROCESS_SET_QUOTA)
								)
			{
				return STATUS_ACCESS_DENIED;
			}
		}
	}

	nRet = pfnNtOpenProcess(
		ProcessHandle,
		DesiredAccess,
		ObjectAttributes,
		ClientId
		);

//#ifdef Dbg
//	DebugLog(DbgInfo,L"Called");
//#endif

	return nRet;
}

NTSTATUS
NTAPI
NtOpenProcess(
    OUT PHANDLE ProcessHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN PCLIENT_ID ClientId OPTIONAL
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtOpenProcess addFun = (__pfnNtOpenProcess)GetProcAddress(hDll,"NtOpenProcess");
	if(addFun)
	{
		ret = addFun(ProcessHandle,DesiredAccess,ObjectAttributes,ClientId);
	}
	return ret;
}