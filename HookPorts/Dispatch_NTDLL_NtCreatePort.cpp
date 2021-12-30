#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtCreatePort.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtCreatePort pfnNtCreatePort = NULL;



//
//Dispatch_NTDLL_NtCreatePort Functions
//
NTSTATUS
NTAPI
OnNtCreatePort(
	OUT PHANDLE             PortHandle,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN ULONG                MaxConnectInfoLength,
	IN ULONG                MaxDataLength,
	IN OUT PULONG           Reserved OPTIONAL
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
		nRet = pfnNtCreatePort(
			PortHandle,
			ObjectAttributes,
			MaxConnectInfoLength,
			MaxDataLength,
			Reserved
			);

		return nRet;
	}

	nRet = pfnNtCreatePort(
		PortHandle,
		ObjectAttributes,
		MaxConnectInfoLength,
		MaxDataLength,
		Reserved
		);

	return nRet;
}

NTSTATUS
NTAPI
NtCreatePort(
	OUT PHANDLE             PortHandle,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN ULONG                MaxConnectInfoLength,
	IN ULONG                MaxDataLength,
	IN OUT PULONG           Reserved OPTIONAL
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtCreatePort addFun = (__pfnNtCreatePort)GetProcAddress(hDll,"NtCreatePort");
	if(addFun)
	{
		ret = addFun(PortHandle,ObjectAttributes,MaxConnectInfoLength,MaxDataLength,Reserved);
	}
	return ret;
}