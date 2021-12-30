#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtCreateProcessEx.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "./Dispatch_NTDLL_NtQueryInformationProcess.h"
#include "./Main.h"
#include "./RemoteInjection.h"
#include "./Initalization.h"



//
//Global
//
__pfnNtCreateProcessEx pfnNtCreateProcessEx = NULL;



//
//Dispatch_NTDLL_NtCreateProcessEx Functions
//
NTSTATUS
NTAPI
OnNtCreateProcessEx(
	OUT PHANDLE ProcessHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE ParentProcess,
	IN BOOLEAN InheritObjectTable,
	IN HANDLE SectionHandle OPTIONAL,
	IN HANDLE DebugPort OPTIONAL,
	IN HANDLE ExceptionPort OPTIONAL,
	IN HANDLE Unknown)
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
		nRet = pfnNtCreateProcessEx(
			ProcessHandle,
			DesiredAccess,
			ObjectAttributes,
			ParentProcess,
			InheritObjectTable,
			SectionHandle,
			DebugPort,
			ExceptionPort,
			Unknown
			);

		return nRet;
	}

	nRet = pfnNtCreateProcessEx(
		ProcessHandle,
		DesiredAccess,
		ObjectAttributes,
		ParentProcess,
		InheritObjectTable,
		SectionHandle,
		DebugPort,
		ExceptionPort,
		Unknown
		);

	return nRet;
}

NTSTATUS
NTAPI
NtCreateProcessEx(
	OUT PHANDLE ProcessHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE ParentProcess,
	IN BOOLEAN InheritObjectTable,
	IN HANDLE SectionHandle OPTIONAL,
	IN HANDLE DebugPort OPTIONAL,
	IN HANDLE ExceptionPort OPTIONAL,
	IN HANDLE Unknown)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtCreateProcessEx addFun = (__pfnNtCreateProcessEx)GetProcAddress(hDll,"NtCreateProcessEx");
	if(addFun)
	{
		ret = addFun(ProcessHandle,DesiredAccess,ObjectAttributes,ParentProcess,InheritObjectTable,SectionHandle,DebugPort,ExceptionPort,Unknown);
	}
	return ret;
}