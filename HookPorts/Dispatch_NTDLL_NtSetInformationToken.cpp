#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtSetInformationToken.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtSetInformationToken pfnNtSetInformationToken = NULL;



//
//Dispatch_NTDLL_NtSetInformationToken Functions
//
NTSTATUS
NTAPI
OnNtSetInformationToken(
	IN HANDLE               TokenHandle,
	IN TOKEN_INFORMATION_CLASS TokenInformationClass,
	OUT PVOID               TokenInformation,
	IN ULONG                TokenInformationLength
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
		nRet = pfnNtSetInformationToken(
			TokenHandle,
			TokenInformationClass,
			TokenInformation,
			TokenInformationLength
			);

		return nRet;
	}

	nRet = pfnNtSetInformationToken(
		TokenHandle,
		TokenInformationClass,
		TokenInformation,
		TokenInformationLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtSetInformationToken(
	IN HANDLE               TokenHandle,
	IN TOKEN_INFORMATION_CLASS TokenInformationClass,
	OUT PVOID               TokenInformation,
	IN ULONG                TokenInformationLength
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtSetInformationToken addFun = (__pfnNtSetInformationToken)GetProcAddress(hDll,"NtSetInformationToken");
	if(addFun)
	{
		ret = addFun(TokenHandle,TokenInformationClass,TokenInformation,TokenInformationLength);
	}
	return ret;
}