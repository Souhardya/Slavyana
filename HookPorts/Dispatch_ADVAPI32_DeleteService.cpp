#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_ADVAPI32_DeleteService.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnDeleteService pfnDeleteService = NULL;



//
//Dispatch_ADVAPI32_DeleteService Functions
//
BOOL
WINAPI
OnDeleteService(
	SC_HANDLE   hService)
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
		bRet = pfnDeleteService(
			hService
			);

		return bRet;
	}

	//bRet = pfnDeleteService(
	//	hService
	//	);

	//return bRet;

	//
	//Refuse directly
	//
	return FALSE;
}