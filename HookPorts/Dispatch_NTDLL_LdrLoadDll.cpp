#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_LdrLoadDll.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"



//
//Global
//
__pfnLdrLoadDll pfnLdrLoadDll = NULL;



//
//Dispatch_NTDLL_LdrLoadDll Functions
//
NTSTATUS
NTAPI
OnLdrLoadDll(
  IN PWCHAR               PathToFile OPTIONAL,
  IN ULONG                Flags OPTIONAL,
  IN PUNICODE_STRING      ModuleFileName,
  OUT PHANDLE             ModuleHandle )
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
		nRet = pfnLdrLoadDll(
			PathToFile,
			Flags,
			ModuleFileName,
			ModuleHandle
			);
	}

	nRet = pfnLdrLoadDll(
		PathToFile,
		Flags,
		ModuleFileName,
		ModuleHandle
		);

//	__try
//	{
//#ifdef Dbg
//		WCHAR szDebugString[512] = {0};
//		wsprintf(
//			szDebugString,
//			L"PathToFile=[%s]\r\nBuffer of ModuleFileName=[%s]",
//			PathToFile,
//			ModuleFileName->Buffer);
//		DebugLog(DbgInfo,szDebugString);
//#endif
//
//#ifdef Dbg
//		DebugLog(DbgInfo,L"Called");
//#endif
//	}
//	__except(EXCEPTION_EXECUTE_HANDLER)
//	{
//	}

	return nRet;
}