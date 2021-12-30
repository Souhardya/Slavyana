#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQueryValueKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./HookHelp_Reg.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"

#include "./CloneAPI_ADVAPI32.h"



//
//Global
//
__pfnNtQueryValueKey pfnNtQueryValueKey = NULL;



//
//Dispatch_NTDLL_NtQueryValueKey Functions
//
NTSTATUS
NTAPI
OnNtQueryValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInformationClass,
	OUT PVOID  KeyValueInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
	)
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
		nRet = pfnNtQueryValueKey(
			KeyHandle,
			ValueName,
			KeyValueInformationClass,
			KeyValueInformation,
			Length,
			ResultLength
			);

		return nRet;
	}

	WCHAR szRegKrnlPath[MAX_NAME] = {NULL};
	WCHAR szDispatchRegKrnlPath[MAX_NAME] = {NULL};
	WCHAR szDispatchRegUserPath[MAX_NAME] = {NULL};

	if( GetRegistryPathName(KeyHandle,NULL,NULL,g_szRestoreRegPath,szRegKrnlPath,szDispatchRegKrnlPath,szDispatchRegUserPath) != 0 )
	{
		nRet = pfnNtQueryValueKey(
			KeyHandle,
			ValueName,
			KeyValueInformationClass,
			KeyValueInformation,
			Length,
			ResultLength
			);

		return nRet;
	}

	//
	//Check if request in SandBox
	//
	if( wcsnicmp(szRegKrnlPath,g_szRestoreFullRegKrnlPath,lstrlenW(g_szRestoreFullRegKrnlPath)) == 0 )
	{
		nRet = pfnNtQueryValueKey(
			KeyHandle,
			ValueName,
			KeyValueInformationClass,
			KeyValueInformation,
			Length,
			ResultLength
			);

		return nRet;
	}

//#ifdef Dbg
//		WCHAR szDebugString[2048] = {0};
//		wsprintf(
//			szDebugString,
//			L"szRegKrnlPath = [%s]\r\nszDispatchRegKrnlPath = [%s]\r\nszDispatchRegUserPath = [%s]\r\nszValueName = [%s]",
//			szRegKrnlPath,
//			szDispatchRegKrnlPath,
//			szDispatchRegUserPath,
//			szValueName
//			);
//		DebugLog(DbgInfo,szDebugString);
//#endif

	WCHAR szValueName[MAX_NAME] = {NULL};
	lstrcpyW(szValueName,ValueName->Buffer);

	if( QueryDispatchedRegValue(szDispatchRegUserPath,szValueName) == 1 )
	{
		//
		//Dispatch
		//
		HKEY hkKey;
		if( CAPI_RegOpenKeyExW(HKEY_USERS,szDispatchRegUserPath,NULL,KEY_READ,&hkKey) == ERROR_SUCCESS )
		{
			nRet = pfnNtQueryValueKey(
				hkKey,
				ValueName,
				KeyValueInformationClass,
				KeyValueInformation,
				Length,
				ResultLength
				);
		}
		CAPI_RegCloseKey(hkKey);

		return nRet;
	}

	nRet = pfnNtQueryValueKey(
		KeyHandle,
		ValueName,
		KeyValueInformationClass,
		KeyValueInformation,
		Length,
		ResultLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtQueryValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInformationClass,
	OUT PVOID  KeyValueInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryValueKey addFun = (__pfnNtQueryValueKey)GetProcAddress(hDll,"NtQueryValueKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,ValueName,KeyValueInformationClass,KeyValueInformation,Length,ResultLength);
	}
	return ret;
}