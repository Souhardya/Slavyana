#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_ADVAPI32_LockServiceDatabase.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnLockServiceDatabase pfnLockServiceDatabase = NULL;



//
//Dispatch_ADVAPI32_LockServiceDatabase Functions
//
SC_LOCK
WINAPI
OnLockServiceDatabase(
	SC_HANDLE hSCManager)
{
	SC_LOCK scRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		scRet = pfnLockServiceDatabase(
			hSCManager
			);

		return scRet;
	}

	//scRet = pfnLockServiceDatabase(
	//	hSCManager
	//	);

	//return scRet;

	//
	//Refuse directly
	//
	return NULL;
}