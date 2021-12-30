#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_ADVAPI32_CreateService.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnCreateServiceA pfnCreateServiceA = NULL;
__pfnCreateServiceW pfnCreateServiceW = NULL;



//
//Dispatch_ADVAPI32_CreateService Functions
//
SC_HANDLE
WINAPI
OnCreateServiceA(
	SC_HANDLE    hSCManager,
	LPCSTR     lpServiceName,
	LPCSTR     lpDisplayName,
	DWORD        dwDesiredAccess,
	DWORD        dwServiceType,
	DWORD        dwStartType,
	DWORD        dwErrorControl,
	LPCSTR     lpBinaryPathName,
	LPCSTR     lpLoadOrderGroup,
	LPDWORD      lpdwTagId,
	LPCSTR     lpDependencies,
	LPCSTR     lpServiceStartName,
	LPCSTR     lpPassword)
{
	SC_HANDLE scRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		scRet = pfnCreateServiceA(
			hSCManager,
			lpServiceName,
			lpDisplayName,
			dwDesiredAccess,
			dwServiceType,
			dwStartType,
			dwErrorControl,
			lpBinaryPathName,
			lpLoadOrderGroup,
			lpdwTagId,
			lpDependencies,
			lpServiceStartName,
			lpPassword
			);

		return scRet;
	}

	//scRet = pfnCreateServiceA(
	//	hSCManager,
	//	lpServiceName,
	//	lpDisplayName,
	//	dwDesiredAccess,
	//	dwServiceType,
	//	dwStartType,
	//	dwErrorControl,
	//	lpBinaryPathName,
	//	lpLoadOrderGroup,
	//	lpdwTagId,
	//	lpDependencies,
	//	lpServiceStartName,
	//	lpPassword
	//	);

	//return scRet;

	//
	//Refuse directly
	//
	return NULL;
}

SC_HANDLE
WINAPI
OnCreateServiceW(
	SC_HANDLE    hSCManager,
	LPCWSTR     lpServiceName,
	LPCWSTR     lpDisplayName,
	DWORD        dwDesiredAccess,
	DWORD        dwServiceType,
	DWORD        dwStartType,
	DWORD        dwErrorControl,
	LPCWSTR     lpBinaryPathName,
	LPCWSTR     lpLoadOrderGroup,
	LPDWORD      lpdwTagId,
	LPCWSTR     lpDependencies,
	LPCWSTR     lpServiceStartName,
	LPCWSTR     lpPassword)
{
	SC_HANDLE scRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		scRet = pfnCreateServiceW(
			hSCManager,
			lpServiceName,
			lpDisplayName,
			dwDesiredAccess,
			dwServiceType,
			dwStartType,
			dwErrorControl,
			lpBinaryPathName,
			lpLoadOrderGroup,
			lpdwTagId,
			lpDependencies,
			lpServiceStartName,
			lpPassword
			);

		return scRet;
	}

	//scRet = pfnCreateServiceW(
	//	hSCManager,
	//	lpServiceName,
	//	lpDisplayName,
	//	dwDesiredAccess,
	//	dwServiceType,
	//	dwStartType,
	//	dwErrorControl,
	//	lpBinaryPathName,
	//	lpLoadOrderGroup,
	//	lpdwTagId,
	//	lpDependencies,
	//	lpServiceStartName,
	//	lpPassword
	//	);

	//return scRet;

	//
	//Refuse directly
	//
	return NULL;
}