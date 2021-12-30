#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtCreateProcess.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtCreateProcess pfnNtCreateProcess = NULL;



//
//Dispatch_NTDLL_NtCreateProcess Functions
//
NTSTATUS
NTAPI
OnNtCreateProcess(
	OUT PHANDLE ProcessHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE ParentProcess,
	IN BOOLEAN InheritObjectTable,
	IN HANDLE SectionHandle,
	IN HANDLE DebugPort,
	IN HANDLE ExceptionPort)
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
		nRet = pfnNtCreateProcess(
			ProcessHandle,
			DesiredAccess,
			ObjectAttributes,
			ParentProcess,
			InheritObjectTable,
			SectionHandle,
			DebugPort,
			ExceptionPort
			);

		return nRet;
	}

	nRet = pfnNtCreateProcess(
		ProcessHandle,
		DesiredAccess,
		ObjectAttributes,
		ParentProcess,
		InheritObjectTable,
		SectionHandle,
		DebugPort,
		ExceptionPort
		);

	return nRet;
}

NTSTATUS
NTAPI
NtCreateProcess(
	OUT PHANDLE ProcessHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE ParentProcess,
	IN BOOLEAN InheritObjectTable,
	IN HANDLE SectionHandle,
	IN HANDLE DebugPort,
	IN HANDLE ExceptionPort)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtCreateProcess addFun = (__pfnNtCreateProcess)GetProcAddress(hDll,"NtCreateProcess");
	if(addFun)
	{
		ret = addFun(ProcessHandle,DesiredAccess,ObjectAttributes,ParentProcess,InheritObjectTable,SectionHandle,DebugPort,ExceptionPort);
	}
	return ret;
}