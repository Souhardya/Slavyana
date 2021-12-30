#include <windows.h>
#include <stdio.h>
#include <shellapi.h>

#include "./BeginLauncher.h"
#include "../Config/Config.h"
#include "../Common/DebugLog.h"



//
//BeginLauncher Functions
//
int BeginLauncherByName(IN const WCHAR * lpSandBoxName)
{
	//Return Value:
	//-1 = error
	//0 = succeed

	//
	//Run Program
	//
	BeginLauncherRunProgram(lpSandBoxName);

	return 0;
}

int BeginLauncherRunProgram(IN const WCHAR * lpSandBoxName)
{
	//Return Value:
	//-1 = error
	//0 = succeed

	//
	//Get value [StartupPath]
	//
	WCHAR szApplicationPath[MAX_PATH] = {NULL};

	HKEY hkStartupPath = NULL;
	WCHAR szSubKeyName[256] = {NULL};
	lstrcpy(szSubKeyName,CONF_SoftwareReg_SandBox);
	lstrcat(szSubKeyName,L"_");
	lstrcat(szSubKeyName,lpSandBoxName);
	lstrcat(szSubKeyName,L"\\");
	lstrcat(szSubKeyName,CONF_SoftwareReg_SandBox_SYNC);

	DWORD dwType = REG_EXPAND_SZ;
	DWORD dwBufLen = MAX_PATH;

	if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkStartupPath) != ERROR_SUCCESS )
	{
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif

		return -1;
	}

	RegQueryValueEx(
					hkStartupPath,
					CONF_Launcher_StartupPath_ValueName,
					NULL,
					&dwType,
					(LPBYTE)&szApplicationPath,
					&dwBufLen
					);

	RegCloseKey(hkStartupPath);

	//
	//Create Process
	//
	DWORD dwCreateProcessRet = 0;

	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);
	StartupInfo.lpDesktop = NULL;
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_SHOWNORMAL;

	if( CreateProcess(
		NULL,
		szApplicationPath,
		NULL,
		NULL,
		FALSE,
		NULL,
		NULL,
		NULL,
		&StartupInfo,
		&ProcessInfo
		) )
	{
		CloseHandle(ProcessInfo.hThread);
		CloseHandle(ProcessInfo.hProcess);

		dwCreateProcessRet = 1;
	}
	else
	{
		HINSTANCE hRet = 0;
		hRet = ShellExecuteW(NULL, L"open", szApplicationPath, NULL, NULL, SW_SHOWNORMAL);

		if( (int)hRet > 32 )
		{
			dwCreateProcessRet = 1;
		}
		else
		{
#ifdef Dbg
			DebugLog(DbgInfo,L"ERROR");
#endif
		}
	}

	//
	//Active RegNotifyChangeKeyValue Callback
	//
	RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkStartupPath);

	RegSetValueEx(
		hkStartupPath,
		CONF_Launcher_StartupInfo_ValueName,
		NULL,
		REG_DWORD,
		(const BYTE *)&dwCreateProcessRet,
		sizeof(DWORD)
		);

	RegCloseKey(hkStartupPath);

	return 0;
}