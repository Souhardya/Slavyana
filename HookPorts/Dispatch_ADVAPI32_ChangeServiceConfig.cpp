#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_ADVAPI32_ChangeServiceConfig.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnChangeServiceConfigA pfnChangeServiceConfigA = NULL;
__pfnChangeServiceConfigW pfnChangeServiceConfigW = NULL;



//
//Dispatch_ADVAPI32_ChangeServiceConfig Functions
//
BOOL
WINAPI
OnChangeServiceConfigA(
	SC_HANDLE    hService,
	DWORD        dwServiceType,
	DWORD        dwStartType,
	DWORD        dwErrorControl,
	LPCSTR     lpBinaryPathName,
	LPCSTR     lpLoadOrderGroup,
	LPDWORD      lpdwTagId,
	LPCSTR     lpDependencies,
	LPCSTR     lpServiceStartName,
	LPCSTR     lpPassword,
	LPCSTR     lpDisplayName)
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
		bRet = pfnChangeServiceConfigA(
			hService,
			dwServiceType,
			dwStartType,
			dwErrorControl,
			lpBinaryPathName,
			lpLoadOrderGroup,
			lpdwTagId,
			lpDependencies,
			lpServiceStartName,
			lpPassword,
			lpDisplayName
			);

		return bRet;
	}

	//bRet = pfnChangeServiceConfigA(
	//	hService,
	//	dwServiceType,
	//	dwStartType,
	//	dwErrorControl,
	//	lpBinaryPathName,
	//	lpLoadOrderGroup,
	//	lpdwTagId,
	//	lpDependencies,
	//	lpServiceStartName,
	//	lpPassword,
	//	lpDisplayName
	//	);

	//return bRet;

	//
	//Refuse directly
	//
	return FALSE;
}

BOOL
WINAPI
OnChangeServiceConfigW(
	SC_HANDLE    hService,
	DWORD        dwServiceType,
	DWORD        dwStartType,
	DWORD        dwErrorControl,
	LPCWSTR     lpBinaryPathName,
	LPCWSTR     lpLoadOrderGroup,
	LPDWORD      lpdwTagId,
	LPCWSTR     lpDependencies,
	LPCWSTR     lpServiceStartName,
	LPCWSTR     lpPassword,
	LPCWSTR     lpDisplayName)
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
		bRet = pfnChangeServiceConfigW(
			hService,
			dwServiceType,
			dwStartType,
			dwErrorControl,
			lpBinaryPathName,
			lpLoadOrderGroup,
			lpdwTagId,
			lpDependencies,
			lpServiceStartName,
			lpPassword,
			lpDisplayName
			);

		return bRet;
	}

	//bRet = pfnChangeServiceConfigW(
	//	hService,
	//	dwServiceType,
	//	dwStartType,
	//	dwErrorControl,
	//	lpBinaryPathName,
	//	lpLoadOrderGroup,
	//	lpdwTagId,
	//	lpDependencies,
	//	lpServiceStartName,
	//	lpPassword,
	//	lpDisplayName
	//	);

	//return bRet;

	//
	//Refuse directly
	//
	return FALSE;
}