#include <windows.h>
#include <ntsecapi.h>

#include "./HookHelp_Reg.h"
#include "./Main.h"
#include "../Config/Config.h"
#include "../Common/DebugLog.h"
#include "./NativeAPI_NTDLL.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"
#include "./Dispatch_NTDLL_NtOpenKey.h"
#include "./Dispatch_NTDLL_NtCreateFile.h"
#include "./Dispatch_NTDLL_NtClose.h"

#include "./CloneAPI_ADVAPI32.h"



//
//Global
//



//
//HookHelp Functions
//
int GetRegistryPathName(
	IN HANDLE hKeyHandle,
	IN HANDLE hRootDirectory,
	IN WCHAR * lpszObjectName,
	IN WCHAR * lpszRestoreRegUserPath,
	OUT WCHAR * lpszRegKrnlPath,
	OUT WCHAR * lpszDispatchRegKrnlPath,
	OUT WCHAR * lpszDispatchRegUserPath
)
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = 0;

	WCHAR szKeyName[MAX_NAME] = {NULL};//(\\REGISTRY\\MACHINE\\XXX)
	WCHAR szSubKeyName[MAX_NAME] = {NULL};//(MACHINE\\XXX)

	WCHAR szDispatchRegKrnlPath[MAX_NAME] = {NULL};//(\\REGISTRY\\USERS\\SandBox_DefaultBox\\MACHINE\\XXX)
	WCHAR szDispatchRegUserPath[MAX_NAME] = {NULL};//(SandBox_DefaultBox\\MACHINE\\XXX)

	__try
	{
		if( hKeyHandle != NULL )
		{
			if( QueryNameInfoByNtQueryObject(hKeyHandle,szKeyName) == -1 )
			{
				return -1;
			}

			//
			//Check if is registry path
			//
			if( wcsnicmp(szKeyName,L"\\REGISTRY\\",lstrlenW(L"\\REGISTRY\\")) != 0 )
			{
				return -1;
			}

			lstrcpyW(lpszRegKrnlPath,szKeyName);

			lstrcpyW(szSubKeyName,&szKeyName[lstrlenW(L"\\REGISTRY\\")]);

			wsprintfW(
				szDispatchRegKrnlPath,
				L"\\REGISTRY\\%s\\%s\\%s",
				CONF_SoftwareReg_SandBox_HKEY_USERS,
				lpszRestoreRegUserPath,
				szSubKeyName
				);

			lstrcpyW(lpszDispatchRegKrnlPath,szDispatchRegKrnlPath);

			lstrcpyW(szDispatchRegUserPath,&szDispatchRegKrnlPath[lstrlenW(L"\\REGISTRY\\USER\\")]);

			lstrcpyW(lpszDispatchRegUserPath,szDispatchRegUserPath);

			return iRet;
		}

		if( hRootDirectory != NULL )
		{
			if( QueryNameInfoByNtQueryObject(hRootDirectory,szKeyName) == -1 )
			{
				return -1;
			}

			//
			//Check if is registry path
			//
			if( wcsnicmp(szKeyName,L"\\REGISTRY\\",lstrlenW(L"\\REGISTRY\\")) != 0 )
			{
				return -1;
			}

			lstrcatW(szKeyName,L"\\");
			lstrcatW(szKeyName,lpszObjectName);

			lstrcpyW(lpszRegKrnlPath,szKeyName);

			lstrcpyW(szSubKeyName,&szKeyName[lstrlenW(L"\\REGISTRY\\")]);

			wsprintfW(
				szDispatchRegKrnlPath,
				L"\\REGISTRY\\%s\\%s\\%s",
				CONF_SoftwareReg_SandBox_HKEY_USERS,
				lpszRestoreRegUserPath,
				szSubKeyName
				);

			lstrcpyW(lpszDispatchRegKrnlPath,szDispatchRegKrnlPath);

			lstrcpyW(szDispatchRegUserPath,&szDispatchRegKrnlPath[lstrlenW(L"\\REGISTRY\\USER\\")]);

			lstrcpyW(lpszDispatchRegUserPath,szDispatchRegUserPath);

			return iRet;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		iRet = -1;
	}

	return -1;
}

int InitSubkeyForRegDispatch( IN WCHAR * lpszDispatchRegUserPath )
{
	//Return Value:
	//-1 = Error
	//0 = Succeed
	int iRet = -1;

	if( lpszDispatchRegUserPath == NULL )
	{
		return iRet;
	}

	//
	//Init Subkey
	//
	HKEY hkKey;

	CAPI_RegCreateKeyW(HKEY_USERS,lpszDispatchRegUserPath,&hkKey);

	CAPI_RegCloseKey(hkKey);

	return iRet;
}

int QueryDispatchedRegValue( IN WCHAR * lpszDispatchRegUserPath, IN WCHAR * lpszValueName )
{
	//Return Value:
	//-1 = Error or MarkAsDeleted
	//0 = Not Dispatched
	//1 = Dispatched
	int iRet = 0;

	if( lpszDispatchRegUserPath == NULL ||
		lpszValueName == NULL
		)
	{
		return -1;
	}

	HKEY hkKey;
	DWORD dwBufLen = 1;
	LPBYTE lpData = (LPBYTE)LocalAlloc(LPTR, dwBufLen);

	if( CAPI_RegOpenKeyExW(HKEY_USERS,lpszDispatchRegUserPath,NULL,KEY_READ,&hkKey) == ERROR_SUCCESS )
	{
		LONG lRet = -1;
		lRet = CAPI_RegQueryValueExW(hkKey,lpszValueName,NULL,NULL,lpData,&dwBufLen);
		if( lRet == ERROR_SUCCESS ||
			lRet == ERROR_MORE_DATA
			)
		{
			iRet = 1;
		}
		else
		{
//#ifdef Dbg
//			WCHAR szDebugString[2048] = {0};
//			wsprintf(
//				szDebugString,
//				L"RegQueryValueExW error code lRet = [%d]",
//				lRet
//				);
//			DebugLog(DbgInfo,szDebugString);
//#endif
		}
	}
	CAPI_RegCloseKey(hkKey);

	LocalFree(lpData);

	return iRet;
}