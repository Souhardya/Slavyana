#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_ADVAPI32_UnlockServiceDatabase.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnUnlockServiceDatabase pfnUnlockServiceDatabase = NULL;



//
//Dispatch_ADVAPI32_UnlockServiceDatabase Functions
//
BOOL
WINAPI
OnUnlockServiceDatabase(
	SC_LOCK ScLock)
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
		bRet = pfnUnlockServiceDatabase(
			ScLock
			);

		return bRet;
	}

	//bRet = pfnUnlockServiceDatabase(
	//	ScLock
	//	);

	//return bRet;

	//
	//Refuse directly
	//
	return FALSE;
}