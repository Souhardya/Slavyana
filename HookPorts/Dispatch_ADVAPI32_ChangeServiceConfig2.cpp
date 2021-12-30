#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_ADVAPI32_ChangeServiceConfig2.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnChangeServiceConfig2A pfnChangeServiceConfig2A = NULL;
__pfnChangeServiceConfig2W pfnChangeServiceConfig2W = NULL;



//
//Dispatch_ADVAPI32_ChangeServiceConfig2 Functions
//
BOOL
WINAPI
OnChangeServiceConfig2A(
	SC_HANDLE    hService,
	DWORD        dwInfoLevel,
	LPVOID       lpInfo)
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
		bRet = pfnChangeServiceConfig2A(
			hService,
			dwInfoLevel,
			lpInfo
			);

		return bRet;
	}

	//bRet = pfnChangeServiceConfig2A(
	//	hService,
	//	dwInfoLevel,
	//	lpInfo
	//	);

	//return bRet;

	//
	//Refuse directly
	//
	return FALSE;
}

BOOL
WINAPI
OnChangeServiceConfig2W(
	SC_HANDLE    hService,
	DWORD        dwInfoLevel,
	LPVOID       lpInfo)
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
		bRet = pfnChangeServiceConfig2W(
			hService,
			dwInfoLevel,
			lpInfo
			);

		return bRet;
	}

	//bRet = pfnChangeServiceConfig2W(
	//	hService,
	//	dwInfoLevel,
	//	lpInfo
	//	);

	//return bRet;

	//
	//Refuse directly
	//
	return FALSE;
}