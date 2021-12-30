#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtDeleteFile.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"



//
//Global
//
__pfnNtDeleteFile pfnNtDeleteFile = NULL;



//
//Dispatch_NTDLL_NtDeleteFile Functions
//
NTSTATUS
NTAPI
OnNtDeleteFile(
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
		nRet = pfnNtDeleteFile(
			ObjectAttributes
			);

		return nRet;
	}

	nRet = pfnNtDeleteFile(
		ObjectAttributes
		);

	return nRet;
}

NTSTATUS
NTAPI
NtDeleteFile(
    IN POBJECT_ATTRIBUTES   ObjectAttributes)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtDeleteFile addFun = (__pfnNtDeleteFile)GetProcAddress(hDll,"NtDeleteFile");
	if(addFun)
	{
		ret = addFun(ObjectAttributes);
	}
	return ret;
}