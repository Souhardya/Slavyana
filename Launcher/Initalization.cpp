#include <windows.h>
#include <stdio.h>

#include "./Initalization.h"
#include "../Config/Config.h"
#include "./Privilege.h"



//
//Initalization Functions
//
int LauncherInitalization(IN const WCHAR * lpSandBoxName)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	int iRet = 0;

	//
	//Adjust the privileges
	//
	if( AdjustPrivileges() == -1 )
	{
		iRet = -1;
	}

	//
	//Log Process Information to RegistrySYNC
	//
	LogProcessInfoToRegistrySYNC(lpSandBoxName,GetCurrentProcessId());

	//
	//Self check
	//
	if( SelfCheck() == -1 )
	{
		iRet = -1;
	}

	return iRet;
}

int AdjustPrivileges(void)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	int iRet = 1;

	//
	//Adjust privileges
	//
	if( EnableSpecificPrivilege(TRUE,SE_BACKUP_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_CHANGE_NOTIFY_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_CREATE_PAGEFILE_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_DEBUG_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_INC_BASE_PRIORITY_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_INCREASE_QUOTA_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_LOAD_DRIVER_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_PROF_SINGLE_PROCESS_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_RESTORE_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_SECURITY_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_SHUTDOWN_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_SYSTEM_ENVIRONMENT_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_SYSTEMTIME_NAME) == FALSE )
	{
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_TAKE_OWNERSHIP_NAME) == FALSE )
	{
		iRet = -1;
	}

	return iRet;
}

int LogProcessInfoToRegistrySYNC(IN const WCHAR * lpSandBoxName, IN DWORD dwProcessId)
{
	//
	//Create subkey [HKEY_USERS\SandBox_XXX\SYNC\PROC\(PID)]
	//
	HKEY hkSandBox = NULL;
	WCHAR szSubKeyName[256] = {NULL};

	wsprintf(
		szSubKeyName,
		L"%s_%s\\%s\\%s\\%d",
		CONF_SoftwareReg_SandBox,
		lpSandBoxName,
		CONF_SoftwareReg_SandBox_SYNC,
		CONF_SoftwareReg_SandBox_SYNC_PROC,
		dwProcessId
		);

	RegCreateKey(HKEY_USERS,szSubKeyName,&hkSandBox);

	RegCloseKey(hkSandBox);

	return 0;
}

int SelfCheck(void)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	return 0;
}