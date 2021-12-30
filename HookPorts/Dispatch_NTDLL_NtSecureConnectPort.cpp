#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtSecureConnectPort.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtSecureConnectPort pfnNtSecureConnectPort = NULL;



//
//Dispatch_NTDLL_NtSecureConnectPort Functions
//
NTSTATUS
NTAPI
OnNtSecureConnectPort(
	PHANDLE PortHandle,
	PUNICODE_STRING PortName,
	PSECURITY_QUALITY_OF_SERVICE SecurityQos,
	PLPC_SECTION_WRITE WriteSection,
	PSID pSid,
	PLPC_SECTION_READ ReadSection,
	PULONG MaximumMessageLength,
	PVOID ConnectInfo,
	PULONG pConnectInfoLength
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
		nRet = pfnNtSecureConnectPort(
			PortHandle,
			PortName,
			SecurityQos,
			WriteSection,
			pSid,
			ReadSection,
			MaximumMessageLength,
			ConnectInfo,
			pConnectInfoLength
			);

		return nRet;
	}

	nRet = pfnNtSecureConnectPort(
		PortHandle,
		PortName,
		SecurityQos,
		WriteSection,
		pSid,
		ReadSection,
		MaximumMessageLength,
		ConnectInfo,
		pConnectInfoLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtSecureConnectPort(
	PHANDLE PortHandle,
	PUNICODE_STRING PortName,
	PSECURITY_QUALITY_OF_SERVICE SecurityQos,
	PLPC_SECTION_WRITE WriteSection,
	PSID pSid,
	PLPC_SECTION_READ ReadSection,
	PULONG MaximumMessageLength,
	PVOID ConnectInfo,
	PULONG pConnectInfoLength
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtSecureConnectPort addFun = (__pfnNtSecureConnectPort)GetProcAddress(hDll,"NtSecureConnectPort");
	if(addFun)
	{
		ret = addFun(PortHandle,PortName,SecurityQos,WriteSection,pSid,ReadSection,MaximumMessageLength,ConnectInfo,pConnectInfoLength);
	}
	return ret;
}