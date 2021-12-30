#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_ADVAPI32_StartService.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnStartServiceA pfnStartServiceA = NULL;
__pfnStartServiceW pfnStartServiceW = NULL;



//
//Dispatch_ADVAPI32_StartService Functions
//
BOOL
WINAPI
OnStartServiceA(
	SC_HANDLE            hService,
	DWORD                dwNumServiceArgs,
	LPCSTR             *lpServiceArgVectors)
{
	BOOL bRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		bRet = pfnStartServiceA(
			hService,
			dwNumServiceArgs,
			lpServiceArgVectors
			);

		return bRet;
	}

	//bRet = pfnStartServiceA(
	//	hService,
	//	dwNumServiceArgs,
	//	lpServiceArgVectors
	//	);

	//return bRet;

	//
	//Refuse directly
	//
	return FALSE;
}

BOOL
WINAPI
OnStartServiceW(
    SC_HANDLE            hService,
    DWORD                dwNumServiceArgs,
    LPCWSTR             *lpServiceArgVectors)
{
	BOOL bRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		bRet = pfnStartServiceW(
			hService,
			dwNumServiceArgs,
			lpServiceArgVectors
			);

		return bRet;
	}

	//bRet = pfnStartServiceW(
	//	hService,
	//	dwNumServiceArgs,
	//	lpServiceArgVectors
	//	);

	//return bRet;

	//
	//Refuse directly
	//
	return FALSE;
}