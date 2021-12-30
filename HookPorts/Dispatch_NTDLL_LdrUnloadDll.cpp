#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_LdrUnloadDll.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"



//
//Global
//
__pfnLdrUnloadDll pfnLdrUnloadDll = NULL;



//
//Dispatch_NTDLL_LdrUnloadDll Functions
//
NTSTATUS
NTAPI
OnLdrUnloadDll(
	IN HANDLE               ModuleHandle )
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
		nRet = pfnLdrUnloadDll(
			ModuleHandle
			);

		return nRet;
	}

	nRet = pfnLdrUnloadDll(
		ModuleHandle
		);

//#ifdef Dbg
//	DebugLog(DbgInfo,L"Called");
//#endif

	return nRet;
}