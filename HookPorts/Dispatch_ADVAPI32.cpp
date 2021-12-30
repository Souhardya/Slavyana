#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_ADVAPI32.h"
#include "./InlineHook.h"
#include "../Common/DebugLog.h"

#include "./Dispatch_ADVAPI32_ChangeServiceConfig.h"
#include "./Dispatch_ADVAPI32_ChangeServiceConfig2.h"
#include "./Dispatch_ADVAPI32_ControlService.h"
#include "./Dispatch_ADVAPI32_CreateService.h"
#include "./Dispatch_ADVAPI32_DeleteService.h"
#include "./Dispatch_ADVAPI32_LockServiceDatabase.h"
#include "./Dispatch_ADVAPI32_SetServiceStatus.h"
#include "./Dispatch_ADVAPI32_StartService.h"
#include "./Dispatch_ADVAPI32_UnlockServiceDatabase.h"



//
//Global
//
BOOL bADVAPI32Patched = FALSE;



//
//Dispatch_ADVAPI32 Functions
//
int Dispatch_ADVAPI32_Start(void)
{
	//Return Value:
	//-1 = error
	//0 = patched
	//1 = succeed

	if( bADVAPI32Patched == TRUE )
	{
		return 0;
	}

	HINSTANCE hADVAPI32 = NULL;
	hADVAPI32 = LoadLibrary(L"advapi32.dll");

	//
	//Patch API
	//

	//ChangeServiceConfigA
	InlineHook(
		(__pfnChangeServiceConfigA)GetProcAddress(hADVAPI32,"ChangeServiceConfigA"),
		OnChangeServiceConfigA,
		(void **)&pfnChangeServiceConfigA
		);
	//ChangeServiceConfigW
	InlineHook(
		(__pfnChangeServiceConfigW)GetProcAddress(hADVAPI32,"ChangeServiceConfigW"),
		OnChangeServiceConfigW,
		(void **)&pfnChangeServiceConfigW
		);

	//ChangeServiceConfig2A
	InlineHook(
		(__pfnChangeServiceConfig2A)GetProcAddress(hADVAPI32,"ChangeServiceConfig2A"),
		OnChangeServiceConfig2A,
		(void **)&pfnChangeServiceConfig2A
		);
	//ChangeServiceConfig2W
	InlineHook(
		(__pfnChangeServiceConfig2W)GetProcAddress(hADVAPI32,"ChangeServiceConfig2W"),
		OnChangeServiceConfig2W,
		(void **)&pfnChangeServiceConfig2W
		);

	//ControlService
	InlineHook(
		(__pfnControlService)GetProcAddress(hADVAPI32,"ControlService"),
		OnControlService,
		(void **)&pfnControlService
		);

	//CreateServiceA
	InlineHook(
		(__pfnCreateServiceA)GetProcAddress(hADVAPI32,"CreateServiceA"),
		OnCreateServiceA,
		(void **)&pfnCreateServiceA
		);
	//CreateServiceW
	InlineHook(
		(__pfnCreateServiceW)GetProcAddress(hADVAPI32,"CreateServiceW"),
		OnCreateServiceW,
		(void **)&pfnCreateServiceW
		);

	//DeleteService
	InlineHook(
		(__pfnDeleteService)GetProcAddress(hADVAPI32,"DeleteService"),
		OnDeleteService,
		(void **)&pfnDeleteService
		);

	//LockServiceDatabase
	InlineHook(
		(__pfnLockServiceDatabase)GetProcAddress(hADVAPI32,"LockServiceDatabase"),
		OnLockServiceDatabase,
		(void **)&pfnLockServiceDatabase
		);

	//SetServiceStatus
	InlineHook(
		(__pfnSetServiceStatus)GetProcAddress(hADVAPI32,"SetServiceStatus"),
		OnSetServiceStatus,
		(void **)&pfnSetServiceStatus
		);

	//StartServiceA
	InlineHook(
		(__pfnStartServiceA)GetProcAddress(hADVAPI32,"StartServiceA"),
		OnStartServiceA,
		(void **)&pfnStartServiceA
		);
	//StartServiceW
	InlineHook(
		(__pfnStartServiceW)GetProcAddress(hADVAPI32,"StartServiceW"),
		OnStartServiceW,
		(void **)&pfnStartServiceW
		);

	//UnlockServiceDatabase
	InlineHook(
		(__pfnUnlockServiceDatabase)GetProcAddress(hADVAPI32,"UnlockServiceDatabase"),
		OnUnlockServiceDatabase,
		(void **)&pfnUnlockServiceDatabase
		);

	return 1;
}