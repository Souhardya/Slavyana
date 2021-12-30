#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Dispatch_NTDLL_NtQuerySystemInformation.h"
#include "./Dispatch_NTDLL_NtQueryInformationThread.h"
#include "./Main.h"
#include "./RemoteInjection.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"

#include "./IATModifier.h"



//
//Global
//
__pfnNtResumeThread pfnNtResumeThread = NULL;



//
//Dispatch_NTDLL_NtResumeThread Functions
//
NTSTATUS
NTAPI
OnNtResumeThread(
	IN HANDLE               ThreadHandle,
	OUT PULONG              SuspendCount OPTIONAL )
{
	NTSTATUS nRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		nRet = pfnNtResumeThread(
			ThreadHandle,
			SuspendCount
			);

		return nRet;
	}

	//
	//Query target ProcessId
	//
	NTSTATUS nStatus;
	THREAD_BASIC_INFORMATION threadinfo;
	DWORD dwProcessId = 0;

	//Query information of thread
	nStatus = NtQueryInformationThread(
		ThreadHandle,
		ThreadBasicInformation,
		(PVOID)&threadinfo,
		sizeof(THREAD_BASIC_INFORMATION),
		NULL);

	if(nStatus == STATUS_SUCCESS)
	{
		DWORD dwCurrentProcessId = 0;
		dwProcessId = (DWORD)(threadinfo.ClientId.UniqueProcess);
		dwCurrentProcessId = GetCurrentProcessId();

		if( dwCurrentProcessId != dwProcessId )
		{
			if( IsProcessPatched(dwProcessId,TRUE) == FALSE )
			{
#ifdef Dbg
				WCHAR szDebugString[256] = {0};
				wsprintf(szDebugString,L"OnNtResumeThread dwProcessId=[%d]",dwProcessId);
				DebugLog(DbgInfo,szDebugString);
#endif

				//
				//Inject process
				//
				//int iInjection = 0;
				//OBJECTCONFIG OBJCfg;
				//lstrcpy(OBJCfg.szSandBoxName,g_szSandBoxName);
				//AddProcessIdToProcTable(dwProcessId);
				//iInjection = InjectProcess((HMODULE)g_hinstDLL,dwProcessId,&OBJCfg,"HookPort_Start");
				//if( iInjection == 1 )
				//{
				//	nRet = STATUS_SUCCESS;
				//	return nRet;
				//}

				//
				//Patch IAT
				//
				LogProcessInfoToRegistrySYNC(g_szSandBoxName,dwProcessId);

				char strDllPath[MAX_PATH] = {0};
				GetModuleFileNameA((HMODULE)g_hinstDLL, strDllPath, MAX_PATH);

				Process process(dwProcessId);
				IATModifier iatModifier(process);
				// retrieve image base address so IATModifier is able to find the import directory
				iatModifier.setImageBase(process.getImageBase(ThreadHandle));
				// modify import directory so our injected dll is loaded first
				iatModifier.writeIAT(strDllPath);
			}
			else
			{
#ifdef Dbg
			WCHAR szDebugString[256] = {0};
			wsprintf(szDebugString,L"Already Patched! dwProcessId=[%d]",dwProcessId);
			DebugLog(DbgInfo,szDebugString);
#endif
			}
		}//dwCurrentProcessId != dwProcessId
	}//nStatus == STATUS_SUCCESS
	else
	{
#ifdef Dbg
		DebugLog(DbgInfo,L"NtQueryInformationThread ERROR!!!!!");
#endif
	}

	nRet = pfnNtResumeThread(
		ThreadHandle,
		SuspendCount
		);

	return nRet;
}

NTSTATUS
NTAPI
NtResumeThread(
	IN HANDLE               ThreadHandle,
	OUT PULONG              SuspendCount OPTIONAL )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtResumeThread addFun = (__pfnNtResumeThread)GetProcAddress(hDll,"NtResumeThread");
	if(addFun)
	{
		ret = addFun(ThreadHandle,SuspendCount);
	}
	return ret;
}