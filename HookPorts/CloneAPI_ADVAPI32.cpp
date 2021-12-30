#include <windows.h>

#include "./CloneAPI_ADVAPI32.h"
#include "./Main.h"



//
//CloneAPI_ADVAPI32 Functions
//
LONG
WINAPI
CAPI_RegCreateKeyW(
    IN HKEY hKey,
    IN LPCWSTR lpSubKey,
    OUT PHKEY phkResult
    )
{
	LONG ret = NULL;
	typedef LONG (WINAPI *lpAddFun)(HKEY,LPCWSTR,PHKEY);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneADVAPI32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"RegCreateKeyW");
	if( addFun != NULL )
	{
		ret = addFun(hKey,lpSubKey,phkResult);
	}
	return ret;
}

LONG
WINAPI
CAPI_RegCloseKey(
    IN HKEY hKey
    )
{
	LONG ret = NULL;
	typedef LONG (WINAPI *lpAddFun)(HKEY);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneADVAPI32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"RegCloseKey");
	if( addFun != NULL )
	{
		ret = addFun(hKey);
	}
	return ret;
}

LONG
WINAPI
CAPI_RegOpenKeyExW(
    IN HKEY hKey,
    IN LPCWSTR lpSubKey,
    IN DWORD ulOptions,
    IN REGSAM samDesired,
    OUT PHKEY phkResult
    )
{
	LONG ret = NULL;
	typedef LONG (WINAPI *lpAddFun)(HKEY,LPCWSTR,DWORD,REGSAM,PHKEY);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneADVAPI32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"RegOpenKeyExW");
	if( addFun != NULL )
	{
		ret = addFun(hKey,lpSubKey,ulOptions,samDesired,phkResult);
	}
	return ret;
}

LONG
WINAPI
CAPI_RegQueryValueExW(
    IN HKEY hKey,
    IN LPCWSTR lpValueName,
    IN LPDWORD lpReserved,
    OUT LPDWORD lpType,
    IN OUT LPBYTE lpData,
    IN OUT LPDWORD lpcbData
    )
{
	LONG ret = NULL;
	typedef LONG (WINAPI *lpAddFun)(HKEY,LPCWSTR,LPDWORD,LPDWORD,LPBYTE,LPDWORD);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneADVAPI32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"RegQueryValueExW");
	if( addFun != NULL )
	{
		ret = addFun(hKey,lpValueName,lpReserved,lpType,lpData,lpcbData);
	}
	return ret;
}