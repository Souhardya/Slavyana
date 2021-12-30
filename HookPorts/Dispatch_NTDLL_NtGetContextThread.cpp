#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtGetContextThread.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtGetContextThread pfnNtGetContextThread = NULL;



//
//Dispatch_NTDLL_NtGetContextThread Functions
//
NTSTATUS
NTAPI
OnNtGetContextThread(
	IN HANDLE               ThreadHandle,
	OUT PCONTEXT            pContext )
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
		nRet = pfnNtGetContextThread(
			ThreadHandle,
			pContext
			);

		return nRet;
	}

	nRet = pfnNtGetContextThread(
		ThreadHandle,
		pContext
		);

#ifdef Dbg
	DebugLog(DbgInfo,L"Called");
#endif

	return nRet;
}

NTSTATUS
NTAPI
NtGetContextThread(
	IN HANDLE               ThreadHandle,
	OUT PCONTEXT            pContext )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtGetContextThread addFun = (__pfnNtGetContextThread)GetProcAddress(hDll,"NtGetContextThread");
	if(addFun)
	{
		ret = addFun(ThreadHandle,pContext);
	}
	return ret;
}