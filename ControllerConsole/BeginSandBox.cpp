#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#pragma comment (lib,"shlwapi.lib")

#include "../Config/Config.h"
#include "./DisplayTools.h"
#include "./BeginSandBox.h"
#include "./Initalization.h"



//
//BeginSandBox Functions
//
int BeginSandBoxRoutine(void)
{
	//
	//Adjust Privileges
	//
	AdjustPrivileges();

	//
	//List SandBox
	//
	int iRet = 0;

	iRet = ListSandBoxToBegin();
	if( iRet == -1 )
	{
		return -1;
	}

	//
	//Waiting order
	//
	wprintfex(
		L"\r\n[NOTIFY] Please input the INDEX(eg. 1) or NAME(eg. \"DefaultBox\") of SandBox:\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	WCHAR szUserInput[256] = {NULL};

	_getws_s(szUserInput);

	if( lstrlen(szUserInput) >= 1 &&
		lstrlen(szUserInput) <= CONF_SandBoxNameLength+2
		)
	{
		if( BeginSandBoxDispatch(szUserInput) == -1 )
		{
			wprintfex(
				L"\r\n[NOTIFY] Begin SandBox failed!\r\n",
				FOREGROUND_INTENSITY | FOREGROUND_RED);

			return -1;
		}
	}
	else
	{
		wprintfex(
			L"\r\n[NOTIFY] Invaild INDEX or NAME of SandBox!\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED);

		return -1;
	}

	wprintfex(
		L"\r\n[NOTIFY] SandBox begin sucessfully!\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	return 0;
}

int ListSandBoxToBegin(void)
{
	//Return Value:
	//-1 = error
	//0 = succeed

	wprintfex(
		L"\r\n[NOTIFY] Choose a SandBox from list to begin.\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	wprintfex(
		L"\r\n[INDEX]    [NAME]                [STATE]\r\n\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN|FOREGROUND_BLUE);

	HKEY hkSandBoxMain = NULL;
	WCHAR szRegSandBoxPath[256] = {NULL};
	int nId = 0;
	WCHAR szSandBoxName[CONF_SandBoxNameLength+1] = {NULL};
	WCHAR szSandBoxLongName[CONF_SandBoxNameLength+1] = {NULL};
	WCHAR szSandBoxInfo[256] = {NULL};

	lstrcpy(szRegSandBoxPath,L"SOFTWARE\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareRegName);
	lstrcat(szRegSandBoxPath,L"\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareReg_SandBox);

	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegSandBoxPath,NULL,KEY_ALL_ACCESS,&hkSandBoxMain) == ERROR_SUCCESS )
	{
		while( RegEnumKey(hkSandBoxMain,nId,szSandBoxName,CONF_SandBoxNameLength+1) == ERROR_SUCCESS )
		{
			nId++;

			lstrcpy(szSandBoxLongName,L"                ");
			memcpy(szSandBoxLongName,szSandBoxName,lstrlen(szSandBoxName)*sizeof(WCHAR));

			if( QuerySandBoxState(szSandBoxName) == 1 )
			{
				wsprintf(szSandBoxInfo,L"   %d       %s      [ON]\r\n",nId,szSandBoxLongName);
			}
			else
			{
				wsprintf(szSandBoxInfo,L"   %d       %s      [OFF]\r\n",nId,szSandBoxLongName);
			}

			wprintfex(szSandBoxInfo,FOREGROUND_INTENSITY | FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
		}

		RegCloseKey(hkSandBoxMain);
	}
	else
	{
		return -1;
	}

	return 0;
}

int QuerySandBoxState(IN const WCHAR * lpSandBoxName)
{
	//Return Value:
	//0 = OFF
	//1 = ON
	int iRet = 0;

	HKEY hkSandBox = NULL;
	WCHAR szSubKeyName[256] = {NULL};
	lstrcpy(szSubKeyName,CONF_SoftwareReg_SandBox);
	lstrcat(szSubKeyName,L"_");
	lstrcat(szSubKeyName,lpSandBoxName);
	lstrcat(szSubKeyName,L"\\");
	lstrcat(szSubKeyName,CONF_SoftwareReg_SandBox_SYNC);

	if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkSandBox) == ERROR_SUCCESS )
	{
		iRet = 1;
	}
	RegCloseKey(hkSandBox);

	return iRet;
}

int BeginSandBoxDispatch(IN const WCHAR * lpString)
{
	//Return Value:
	//-1 = error
	//0 = succeed

	int iRet = 0;

	//
	//Check if INDEX or NAME
	//
	WCHAR szSandBoxName[CONF_SandBoxNameLength+2+1] = {NULL};
	WCHAR szSandBoxTrueName[CONF_SandBoxNameLength+2+1] = {NULL};
	lstrcpy(szSandBoxName,lpString);
	lstrcpy(szSandBoxTrueName,lpString);

	HKEY hkSandBoxMain = NULL;
	WCHAR szRegSandBoxPath[256] = {NULL};
	int nId = 0;

	lstrcpy(szRegSandBoxPath,L"SOFTWARE\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareRegName);
	lstrcat(szRegSandBoxPath,L"\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareReg_SandBox);

	//NAME
	if( wcsncmp(szSandBoxTrueName,L"\"",lstrlen(L"\"")) == 0 &&
		wcsicmp(&szSandBoxTrueName[lstrlen(szSandBoxTrueName)-1],L"\"") == 0
		)
	{
		szSandBoxTrueName[lstrlen(szSandBoxTrueName)-1] = '\0';
		lstrcpy(szSandBoxTrueName,&szSandBoxTrueName[1]);

		iRet = -1;

		if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegSandBoxPath,NULL,KEY_ALL_ACCESS,&hkSandBoxMain) == ERROR_SUCCESS )
		{
			while( RegEnumKey(hkSandBoxMain,nId,szSandBoxName,CONF_SandBoxNameLength+1) == ERROR_SUCCESS )
			{
				nId++;

				if( wcsicmp(szSandBoxName,szSandBoxTrueName) == 0 )
				{
					if( BeginSandBoxByName(szSandBoxName) == -1 )
					{
						iRet = -1;
					}
					else
					{
						iRet = 0;
					}
				}
			}
			RegCloseKey(hkSandBoxMain);
		}
		else
		{
			iRet = -1;
		}
	}
	else//INDEX
	{
		iRet = -1;

		if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegSandBoxPath,NULL,KEY_ALL_ACCESS,&hkSandBoxMain) == ERROR_SUCCESS )
		{
			while( RegEnumKey(hkSandBoxMain,nId,szSandBoxName,CONF_SandBoxNameLength+1) == ERROR_SUCCESS )
			{
				nId++;

				if( nId == _wtoi(lpString) )
				{
					if( BeginSandBoxByName(szSandBoxName) == -1 )
					{
						iRet = -1;
					}
					else
					{
						iRet = 0;
					}
				}
			}
			RegCloseKey(hkSandBoxMain);
		}
		else
		{
			iRet = -1;
		}
	}

	return iRet;
}

int BeginSandBoxByName(IN const WCHAR * lpSandBoxName)
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = 0;

	HKEY hkSandBoxMain = NULL;
	WCHAR szRegSandBoxPath[256] = {NULL};

	lstrcpy(szRegSandBoxPath,L"SOFTWARE\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareRegName);
	lstrcat(szRegSandBoxPath,L"\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareReg_SandBox);
	lstrcat(szRegSandBoxPath,L"\\");
	lstrcat(szRegSandBoxPath,lpSandBoxName);

	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegSandBoxPath,NULL,KEY_ALL_ACCESS,&hkSandBoxMain) != ERROR_SUCCESS )
	{
		return -1;
	}
	RegCloseKey(hkSandBoxMain);

	//
	//Load RegHive
	//
	BeginSandBoxLoadRegHive(lpSandBoxName);

	//
	//Create Registry SYNC
	//
	BeginSandBoxCreateRegistrySYNC(lpSandBoxName);

	//
	//Clean Callback
	//
	BeginSandBoxCleanCallback(lpSandBoxName);

	//
	//Call Launcher
	//
	wprintfex(
		L"\r\n[NOTIFY] Please select the Application PATH. (Program will be run in SandBox.)\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	WCHAR szFileFullPath[MAX_PATH] = {NULL};
	OPENFILENAME ofn;

	RtlZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"Executable Files\0*.exe;*.com;*.bat;*.pif\0All Files(*.*)\0*.*\0";
	ofn.lpstrFile = szFileFullPath;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_PATHMUSTEXIST|OFN_HIDEREADONLY|OFN_LONGNAMES;

	if(GetOpenFileName(&ofn))
	{
		iRet = BeginSandBoxCallLauncher(lpSandBoxName,szFileFullPath);
	}
	else
	{
		iRet = -1;
	}

	//wprintfex(
	//	L"\r\n[NOTIFY] Please input the Application PATH. (Program will be run in SandBox.)\r\n",
	//	FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	//WCHAR szUserInput[512] = {NULL};

	//_getws_s(szUserInput);

	//iRet = BeginSandBoxCallLauncher(lpSandBoxName,szUserInput);

	//
	//Clean Callback
	//
	BeginSandBoxCleanCallback(lpSandBoxName);

	return iRet;
}

int BeginSandBoxLoadRegHive(IN const WCHAR * lpSandBoxName)
{
	//
	//Get RestorePath
	//
	WCHAR szRestorePath[MAX_PATH] = {NULL};
	DWORD dwType = REG_SZ;
	DWORD dwBufLen = MAX_PATH;

	HKEY hkSandBoxCfg = NULL;
	WCHAR szRegConfigPath[256] = {NULL};

	lstrcpy(szRegConfigPath,L"SOFTWARE\\");
	lstrcat(szRegConfigPath,CONF_SoftwareRegName);
	lstrcat(szRegConfigPath,L"\\");
	lstrcat(szRegConfigPath,CONF_SoftwareReg_Config);

	RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegConfigPath,NULL,KEY_ALL_ACCESS,&hkSandBoxCfg);

	dwType = REG_EXPAND_SZ;
	dwBufLen = MAX_PATH;

	RegQueryValueEx(
					hkSandBoxCfg,
					CONF_SoftwareReg_Config_RestorePath,
					NULL,
					&dwType,
					(LPBYTE)&szRestorePath,
					&dwBufLen
					);

	RegCloseKey(hkSandBoxCfg);

	//
	//Load RegHive
	//
	lstrcat(szRestorePath,L"\\");
	lstrcat(szRestorePath,lpSandBoxName);

	WCHAR szRegHivePath[MAX_PATH] = {NULL};
	lstrcpy(szRegHivePath,szRestorePath);
	lstrcat(szRegHivePath,L"\\");
	lstrcat(szRegHivePath,CONF_SandBox_RegHive_FileName);

	WCHAR szSubKeyName[256] = {NULL};
	lstrcpy(szSubKeyName,CONF_SoftwareReg_SandBox);
	lstrcat(szSubKeyName,L"_");
	lstrcat(szSubKeyName,lpSandBoxName);

	RegLoadKey(HKEY_USERS,szSubKeyName,szRegHivePath);

	return 0;
}

int BeginSandBoxCreateRegistrySYNC(IN const WCHAR * lpSandBoxName)
{
	//
	//Create subkey [HKEY_USERS\SandBox_XXX\SYNC]
	//
	HKEY hkSandBox = NULL;
	WCHAR szSubKeyName[256] = {NULL};
	lstrcpy(szSubKeyName,CONF_SoftwareReg_SandBox);
	lstrcat(szSubKeyName,L"_");
	lstrcat(szSubKeyName,lpSandBoxName);
	lstrcat(szSubKeyName,L"\\");
	lstrcat(szSubKeyName,CONF_SoftwareReg_SandBox_SYNC);

	RegCreateKey(HKEY_USERS,szSubKeyName,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [HKEY_USERS\SandBox_XXX\SYNC\PROC]
	//
	lstrcat(szSubKeyName,L"\\");
	lstrcat(szSubKeyName,CONF_SoftwareReg_SandBox_SYNC_PROC);

	RegCreateKey(HKEY_USERS,szSubKeyName,&hkSandBox);
	RegCloseKey(hkSandBox);

	return 0;
}

int BeginSandBoxCallLauncher(IN const WCHAR * lpSandBoxName, IN const WCHAR * lpApplicationPath)
{
	//Return Value:
	//-1 = error
	//0 = succeed

	int iRet = -1;

	//
	//Set value [StartupPath]
	//
	HKEY hkStartupPath = NULL;
	WCHAR szSubKeyName[256] = {NULL};
	lstrcpy(szSubKeyName,CONF_SoftwareReg_SandBox);
	lstrcat(szSubKeyName,L"_");
	lstrcat(szSubKeyName,lpSandBoxName);
	lstrcat(szSubKeyName,L"\\");
	lstrcat(szSubKeyName,CONF_SoftwareReg_SandBox_SYNC);

	RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkStartupPath);

	RegSetValueEx(
		hkStartupPath,
		CONF_Launcher_StartupPath_ValueName,
		NULL,
		REG_EXPAND_SZ,
		(const BYTE *)lpApplicationPath,
		MAX_PATH
		);

	RegCloseKey(hkStartupPath);

	//
	//Get Launcher FilePath
	//
	WCHAR szLauncherPath[MAX_PATH] = {NULL};
	GetModuleFileName(NULL,szLauncherPath,MAX_PATH);
	PathRemoveFileSpec(szLauncherPath);
	lstrcat(szLauncherPath,L"\\");
	lstrcat(szLauncherPath,CONF_Launcher_FileName);

	//
	//Call Launcher
	//
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);
	StartupInfo.lpDesktop = NULL;
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_SHOW;

	//
	//Command Line
	//
	//(Launcher.exe -Launch [SandBox Name])
	WCHAR szCommandLine[512] = {NULL};
	wsprintf(
		szCommandLine,
		L" %s",
		lpSandBoxName
		);

	//
	//Callback RegNotifyChangeKeyValue
	//
	HKEY hkRegistrySYNC = NULL;

	DWORD dwStartupInfo = 0;
	HANDLE hEventNotifyValue = NULL;
	DWORD dwType = REG_DWORD;
	DWORD dwBufLen = sizeof(DWORD);

	BOOL bIsLauncherCalled = FALSE;

	if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkRegistrySYNC) == ERROR_SUCCESS )
	{
		hEventNotifyValue = CreateEvent(0,TRUE,FALSE,0);
		ResetEvent(hEventNotifyValue);

		for(int i=0;i<20;i++)
		{
			if( RegNotifyChangeKeyValue(
				hkRegistrySYNC,
				FALSE,
				REG_NOTIFY_CHANGE_NAME|REG_NOTIFY_CHANGE_LAST_SET,
				hEventNotifyValue,
				TRUE
				) ==ERROR_SUCCESS )
			{
				if( bIsLauncherCalled != TRUE )
				{
					if( CreateProcess(
						szLauncherPath,
						szCommandLine,
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

						bIsLauncherCalled = TRUE;
					}//CreateProcess
					else
					{
						return -1;
					}
				}

				WaitForSingleObject(hEventNotifyValue, CONF_Launcher_Startup_TimeOut);
				ResetEvent(hEventNotifyValue);

				if( RegQueryValueEx(
								hkRegistrySYNC,
								CONF_Launcher_StartupInfo_ValueName,
								NULL,
								&dwType,
								(LPBYTE)&dwStartupInfo,
								&dwBufLen
								) == ERROR_SUCCESS )
				{
					if( dwStartupInfo == 1 )
					{
						iRet = 0;
					}
					break;
				}//RegQueryValueEx
			}//RegNotifyChangeKeyValue
		}//for
	}//RegOpenKeyEx
	RegCloseKey(hkRegistrySYNC);
	CloseHandle(hEventNotifyValue);

	return iRet;
}

int BeginSandBoxCleanCallback(IN const WCHAR * lpSandBoxName)
{
	//
	//Delete value [StartupPath&StartupInfo]
	//
	HKEY hkStartup = NULL;
	WCHAR szSubKeyName[256] = {NULL};
	lstrcpy(szSubKeyName,CONF_SoftwareReg_SandBox);
	lstrcat(szSubKeyName,L"_");
	lstrcat(szSubKeyName,lpSandBoxName);
	lstrcat(szSubKeyName,L"\\");
	lstrcat(szSubKeyName,CONF_SoftwareReg_SandBox_SYNC);

	RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkStartup);

	//Delete [StartupPath]
	RegDeleteValue(hkStartup,CONF_Launcher_StartupPath_ValueName);

	//Delete [StartupInfo]
	RegDeleteValue(hkStartup,CONF_Launcher_StartupInfo_ValueName);

	RegCloseKey(hkStartup);

	return 0;
}