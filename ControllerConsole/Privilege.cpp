#include <windows.h>
#include <stdio.h>

#include "./DisplayTools.h"
#include "./Privilege.h"



//
//Privilege Functions
//

BOOL EnableSpecificPrivilege(IN BOOL bEnable,IN WCHAR * Name)
{
	BOOL bResult = FALSE;
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES TokenPrivileges;

	if( OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,&hToken) == 0 )
	{
		return FALSE;
	}

	TokenPrivileges.PrivilegeCount = 1;
	TokenPrivileges.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
	bResult = LookupPrivilegeValue(NULL,Name,&TokenPrivileges.Privileges[0].Luid);

	if( !bResult )
	{
		CloseHandle(hToken);
		return FALSE;
	}

	bResult = AdjustTokenPrivileges(hToken,FALSE,&TokenPrivileges,sizeof(TOKEN_PRIVILEGES),NULL,NULL);

	if(GetLastError() != ERROR_SUCCESS || !bResult)
	{
		CloseHandle(hToken);
		return FALSE;
	}

	CloseHandle(hToken);
	return TRUE;
}

BOOL SetAllPrivileges(IN BOOL bEnable)
{
	EnableSpecificPrivilege(bEnable,SE_ASSIGNPRIMARYTOKEN_NAME);
	EnableSpecificPrivilege(bEnable,SE_AUDIT_NAME);
	EnableSpecificPrivilege(bEnable,SE_BACKUP_NAME);
	EnableSpecificPrivilege(bEnable,SE_CHANGE_NOTIFY_NAME);
	EnableSpecificPrivilege(bEnable,SE_CREATE_PAGEFILE_NAME);
	EnableSpecificPrivilege(bEnable,SE_CREATE_PERMANENT_NAME);
	EnableSpecificPrivilege(bEnable,SE_CREATE_TOKEN_NAME);
	EnableSpecificPrivilege(bEnable,SE_DEBUG_NAME);
	EnableSpecificPrivilege(bEnable,SE_INC_BASE_PRIORITY_NAME);
	EnableSpecificPrivilege(bEnable,SE_INCREASE_QUOTA_NAME);
	EnableSpecificPrivilege(bEnable,SE_LOAD_DRIVER_NAME);
	EnableSpecificPrivilege(bEnable,SE_LOCK_MEMORY_NAME);
	EnableSpecificPrivilege(bEnable,SE_PROF_SINGLE_PROCESS_NAME);
	EnableSpecificPrivilege(bEnable,SE_REMOTE_SHUTDOWN_NAME);
	EnableSpecificPrivilege(bEnable,SE_RESTORE_NAME);
	EnableSpecificPrivilege(bEnable,SE_SECURITY_NAME);
	EnableSpecificPrivilege(bEnable,SE_SHUTDOWN_NAME);
	EnableSpecificPrivilege(bEnable,SE_SYSTEM_ENVIRONMENT_NAME);
	EnableSpecificPrivilege(bEnable,SE_SYSTEM_PROFILE_NAME);
	EnableSpecificPrivilege(bEnable,SE_SYSTEMTIME_NAME);
	EnableSpecificPrivilege(bEnable,SE_TAKE_OWNERSHIP_NAME);
	EnableSpecificPrivilege(bEnable,SE_TCB_NAME);
	EnableSpecificPrivilege(bEnable,SE_UNSOLICITED_INPUT_NAME);
	EnableSpecificPrivilege(bEnable,SE_MACHINE_ACCOUNT_NAME);

	return TRUE;
}