#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtOpenEvent.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "../Config/Config.h"



//
//Global
//
__pfnNtOpenEvent pfnNtOpenEvent = NULL;



//
//Dispatch_NTDLL_NtOpenEvent Functions
//
NTSTATUS
NTAPI
OnNtOpenEvent(
	OUT PHANDLE             EventHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes)
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
		nRet = pfnNtOpenEvent(
			EventHandle,
			DesiredAccess,
			ObjectAttributes
			);

		return nRet;
	}

	nRet = pfnNtOpenEvent(
		EventHandle,
		DesiredAccess,
		ObjectAttributes
		);

	return nRet;
}

NTSTATUS
NTAPI
NtOpenEvent(
	OUT PHANDLE             EventHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtOpenEvent addFun = (__pfnNtOpenEvent)GetProcAddress(hDll,"NtOpenEvent");
	if(addFun)
	{
		ret = addFun(EventHandle,DesiredAccess,ObjectAttributes);
	}
	return ret;
}