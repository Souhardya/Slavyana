#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "../Config/Config.h"
#include "./DisplayTools.h"
#include "./StopSandBox.h"
#include "./BeginSandBox.h"
#include "./Initalization.h"



//
//StopSandBox Functions
//
int StopSandBoxRoutine(void)
{
	//
	//Adjust Privileges
	//
	AdjustPrivileges();

	//
	//List SandBox
	//
	int iRet = 0;

	iRet = ListSandBoxToStop();
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
		if( StopSandBoxDispatch(szUserInput) == -1 )
		{
			wprintfex(
				L"\r\n[NOTIFY] Stop SandBox failed!\r\n",
				FOREGROUND_INTENSITY | FOREGROUND_RED);
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
		L"\r\n[NOTIFY] SandBox stop sucessfully!\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	return 0;
}

int ListSandBoxToStop(void)
{
	//Return Value:
	//-1 = error
	//0 = succeed

	wprintfex(
		L"\r\n[NOTIFY] Choose a SandBox from list to stop.\r\n",
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

int StopSandBoxDispatch(IN const WCHAR * lpString)
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
					if( StopSandBoxByName(szSandBoxName) == -1 )
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
					if( StopSandBoxByName(szSandBoxName) == -1 )
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

int StopSandBoxByName(IN const WCHAR * lpSandBoxName)
{
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
	//Remove Registry SYNC
	//
	StopSandBoxRemoveRegistrySYNC(lpSandBoxName);

	//
	//Save RegHive
	//
	StopSandBoxSaveRegHive(lpSandBoxName);

	//
	//UnLoad RegHive
	//
	StopSandBoxUnLoadRegHive(lpSandBoxName);

	//
	//Update RegHive
	//
	StopSandBoxUpdateRegHive(lpSandBoxName);

	return 0;
}

int StopSandBoxUnLoadRegHive(IN const WCHAR * lpSandBoxName)
{
	//
	//UnLoad RegHive
	//
	WCHAR szSubKeyName[256] = {NULL};
	lstrcpy(szSubKeyName,CONF_SoftwareReg_SandBox);
	lstrcat(szSubKeyName,L"_");
	lstrcat(szSubKeyName,lpSandBoxName);

	RegUnLoadKey(HKEY_USERS,szSubKeyName);

	return 0;
}

int StopSandBoxRemoveRegistrySYNC(IN const WCHAR * lpSandBoxName)
{
	//
	//Remove subkey [HKEY_USERS\SandBox_XXX\SYNC]
	//
	HKEY hkSandBox = NULL;
	WCHAR szSubKeyName[256] = {NULL};
	lstrcpy(szSubKeyName,CONF_SoftwareReg_SandBox);
	lstrcat(szSubKeyName,L"_");
	lstrcat(szSubKeyName,lpSandBoxName);
	lstrcat(szSubKeyName,L"\\");
	lstrcat(szSubKeyName,CONF_SoftwareReg_SandBox_SYNC);

	SHDeleteKey(HKEY_USERS,szSubKeyName);

	return 0;
}

int StopSandBoxSaveRegHive(IN const WCHAR * lpSandBoxName)
{
	//
	//Save RegHive
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

	lstrcat(szRestorePath,L"\\");
	lstrcat(szRestorePath,lpSandBoxName);
	lstrcat(szRestorePath,L"\\");
	lstrcat(szRestorePath,CONF_SandBox_RegHive_UpdateName);

	HKEY hkRegistry = NULL;
	WCHAR szSubKeyName[256] = {NULL};
	lstrcpy(szSubKeyName,CONF_SoftwareReg_SandBox);
	lstrcat(szSubKeyName,L"_");
	lstrcat(szSubKeyName,lpSandBoxName);

	if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkRegistry) == ERROR_SUCCESS )
	{
		RegSaveKey(hkRegistry,szRestorePath,NULL);
	}

	RegCloseKey(hkRegistry);

	return 0;
}

int StopSandBoxUpdateRegHive(IN const WCHAR * lpSandBoxName)
{
	//
	//Update RegHive
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

	lstrcat(szRestorePath,L"\\");
	lstrcat(szRestorePath,lpSandBoxName);
	lstrcat(szRestorePath,L"\\");

	WCHAR szOriginalRegHivePath[MAX_PATH] = {NULL};
	WCHAR szUpdateRegHivePath[MAX_PATH] = {NULL};

	lstrcpy(szOriginalRegHivePath,szRestorePath);
	lstrcat(szOriginalRegHivePath,CONF_SandBox_RegHive_FileName);

	lstrcpy(szUpdateRegHivePath,szRestorePath);
	lstrcat(szUpdateRegHivePath,CONF_SandBox_RegHive_UpdateName);

	MoveFileEx(szUpdateRegHivePath,szOriginalRegHivePath,MOVEFILE_REPLACE_EXISTING);

	return 0;
}