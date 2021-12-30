#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtConnectPort.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtConnectPort pfnNtConnectPort = NULL;



//
//Dispatch_NTDLL_NtConnectPort Functions
//
NTSTATUS
NTAPI
OnNtConnectPort(
	OUT PHANDLE             ClientPortHandle,
	IN PUNICODE_STRING      ServerPortName,
	IN PSECURITY_QUALITY_OF_SERVICE SecurityQos,
	IN OUT PLPC_SECTION_OWNER_MEMORY ClientSharedMemory OPTIONAL,
	OUT PLPC_SECTION_MEMORY ServerSharedMemory OPTIONAL,
	OUT PULONG              MaximumMessageLength OPTIONAL,
	IN PVOID                ConnectionInfo OPTIONAL,
	IN PULONG               ConnectionInfoLength OPTIONAL
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
		nRet = pfnNtConnectPort(
			ClientPortHandle,
			ServerPortName,
			SecurityQos,
			ClientSharedMemory,
			ServerSharedMemory,
			MaximumMessageLength,
			ConnectionInfo,
			ConnectionInfoLength
			);

		return nRet;
	}

	nRet = pfnNtConnectPort(
		ClientPortHandle,
		ServerPortName,
		SecurityQos,
		ClientSharedMemory,
		ServerSharedMemory,
		MaximumMessageLength,
		ConnectionInfo,
		ConnectionInfoLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtConnectPort(
	OUT PHANDLE             ClientPortHandle,
	IN PUNICODE_STRING      ServerPortName,
	IN PSECURITY_QUALITY_OF_SERVICE SecurityQos,
	IN OUT PLPC_SECTION_OWNER_MEMORY ClientSharedMemory OPTIONAL,
	OUT PLPC_SECTION_MEMORY ServerSharedMemory OPTIONAL,
	OUT PULONG              MaximumMessageLength OPTIONAL,
	IN PVOID                ConnectionInfo OPTIONAL,
	IN PULONG               ConnectionInfoLength OPTIONAL
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtConnectPort addFun = (__pfnNtConnectPort)GetProcAddress(hDll,"NtConnectPort");
	if(addFun)
	{
		ret = addFun(ClientPortHandle,ServerPortName,SecurityQos,ClientSharedMemory,ServerSharedMemory,MaximumMessageLength,ConnectionInfo,ConnectionInfoLength);
	}
	return ret;
}