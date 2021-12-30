#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_KERNEL32_CreateProcessInternal.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./RemoteInjection.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnCreateProcessInternalW pfnCreateProcessInternalW = NULL;



//
//Dispatch_KERNEL32_CreateProcessInternal Functions
//
BOOL
WINAPI
OnCreateProcessInternalW(
	HANDLE hToken,
	LPCWSTR lpApplicationName,
	LPWSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles,
	DWORD dwCreationFlags,
	LPVOID lpEnvironment,
	LPCWSTR lpCurrentDirectory,
	LPSTARTUPINFOW lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation,
	PHANDLE hNewToken)
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
		bRet = pfnCreateProcessInternalW(
			hToken,
			lpApplicationName,
			lpCommandLine,
			lpProcessAttributes,
			lpThreadAttributes,
			bInheritHandles,
			dwCreationFlags,
			lpEnvironment,
			lpCurrentDirectory,
			lpStartupInfo,
			lpProcessInformation,
			hNewToken
			);

		return bRet;
	}

	////
	////Check if includes [CREATE_SUSPENDED] flags
	////
	//BOOL bSuspended = FALSE;

	//if( (dwCreationFlags & CREATE_SUSPENDED ) == 0 )
	//{
	//	dwCreationFlags |= CREATE_SUSPENDED;
	//	bSuspended = TRUE;
	//}
	//else
	//{
	//	bSuspended = FALSE;
	//}

	bRet = pfnCreateProcessInternalW(
		hToken,
		lpApplicationName,
		lpCommandLine,
		lpProcessAttributes,
		lpThreadAttributes,
		bInheritHandles,
		dwCreationFlags,
		lpEnvironment,
		lpCurrentDirectory,
		lpStartupInfo,
		lpProcessInformation,
		hNewToken
		);

	//if(bRet)
	//{
	//	//
	//	//Inject process
	//	//
	//	int iInjection = 0;
	//	OBJECTCONFIG OBJCfg;
	//	lstrcpy(OBJCfg.szSandBoxName,g_szSandBoxName);
	//	iInjection = InjectProcess((HMODULE)g_hinstDLL,lpProcessInformation->dwProcessId,&OBJCfg,"HookPort_Start");
	//	if( iInjection == 1 )
	//	{
	//		AddProcessIdToProcTable(lpProcessInformation->dwProcessId);

	//		//
	//		//Resume thread if not includes [CREATE_SUSPENDED] flags before
	//		//
	//		if(bSuspended)
	//		{
	//			ResumeThread(lpProcessInformation->hThread);
	//		}
	//	}
	//	else
	//	{
	//		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, lpProcessInformation->dwProcessId);
	//		if(hProcess == NULL)
	//		{
	//			bRet =  FALSE;
	//		}
	//		bRet = TerminateProcess(hProcess, 0);
	//		CloseHandle(hProcess);
	//	}
	//}

	return bRet;
}