#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtCreateEvent.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "../Config/Config.h"



//
//Global
//
__pfnNtCreateEvent pfnNtCreateEvent = NULL;



//
//Dispatch_NTDLL_NtCreateEvent Functions
//
NTSTATUS
NTAPI
OnNtCreateEvent(
	OUT PHANDLE             EventHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN EVENT_TYPE           EventType,
	IN BOOLEAN              InitialState)
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
		nRet = pfnNtCreateEvent(
			EventHandle,
			DesiredAccess,
			ObjectAttributes,
			EventType,
			InitialState
			);

		return nRet;
	}

	nRet = pfnNtCreateEvent(
		EventHandle,
		DesiredAccess,
		ObjectAttributes,
		EventType,
		InitialState
		);

	return nRet;
}

NTSTATUS
NTAPI
NtCreateEvent(
	OUT PHANDLE             EventHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN EVENT_TYPE           EventType,
	IN BOOLEAN              InitialState)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtCreateEvent addFun = (__pfnNtCreateEvent)GetProcAddress(hDll,"NtCreateEvent");
	if(addFun)
	{
		ret = addFun(EventHandle,DesiredAccess,ObjectAttributes,EventType,InitialState);
	}
	return ret;
}