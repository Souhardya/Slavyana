#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <psapi.h>
#pragma comment (lib,"psapi.lib")

#include "./ProcessTools.h"



//
//ProcessTools Functions
//

BOOL GetProcessPath(IN DWORD dwProcessID,IN LPWSTR lpszBuffer,IN int nSize,IN BOOL bFullPath)
{
	BOOL bRet = FALSE;

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,dwProcessID);
	if(hProcess)
	{
		if(bFullPath)
		{
			if(GetModuleFileNameEx(hProcess, NULL, lpszBuffer, nSize))
			{
				bRet = TRUE;
			}
		}
		else
		{
			if(GetModuleBaseName(hProcess, NULL, lpszBuffer, nSize))
			{
				bRet = TRUE;
			}
		}
	}
	CloseHandle(hProcess);

	return bRet;
}