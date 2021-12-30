#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_ADVAPI32_SetServiceStatus.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnSetServiceStatus pfnSetServiceStatus = NULL;



//
//Dispatch_ADVAPI32_SetServiceStatus Functions
//
BOOL
WINAPI
OnSetServiceStatus(
	SERVICE_STATUS_HANDLE hServiceStatus,
	LPSERVICE_STATUS lpServiceStatus)
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
		bRet = pfnSetServiceStatus(
			hServiceStatus,
			lpServiceStatus
			);

		return bRet;
	}

	//bRet = pfnSetServiceStatus(
	//	hServiceStatus,
	//	lpServiceStatus
	//	);

	//return bRet;

	//
	//Refuse directly
	//
	return FALSE;
}