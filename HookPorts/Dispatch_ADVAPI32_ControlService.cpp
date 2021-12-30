#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_ADVAPI32_ControlService.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnControlService pfnControlService = NULL;



//
//Dispatch_ADVAPI32_ControlService Functions
//
BOOL
WINAPI
OnControlService(
	SC_HANDLE           hService,
	DWORD               dwControl,
	LPSERVICE_STATUS    lpServiceStatus)
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
		bRet = pfnControlService(
			hService,
			dwControl,
			lpServiceStatus
			);

		return bRet;
	}

	//bRet = pfnControlService(
	//	hService,
	//	dwControl,
	//	lpServiceStatus
	//	);

	//return bRet;

	//
	//Refuse directly
	//
	return FALSE;
}