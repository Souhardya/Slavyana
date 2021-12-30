#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtSetValueKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./HookHelp_Reg.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"

#include "./CloneAPI_ADVAPI32.h"



//
//Global
//
__pfnNtSetValueKey pfnNtSetValueKey = NULL;



//
//Dispatch_NTDLL_NtSetValueKey Functions
//
NTSTATUS
NTAPI
OnNtSetValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN ULONG  TitleIndex  OPTIONAL,
	IN ULONG  Type,
	IN PVOID  Data,
	IN ULONG  DataSize
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
		nRet = pfnNtSetValueKey(
			KeyHandle,
			ValueName,
			TitleIndex,
			Type,
			Data,
			DataSize
			);

		return nRet;
	}

	WCHAR szRegKrnlPath[MAX_NAME] = {NULL};
	WCHAR szDispatchRegKrnlPath[MAX_NAME] = {NULL};
	WCHAR szDispatchRegUserPath[MAX_NAME] = {NULL};

	if( GetRegistryPathName(KeyHandle,NULL,NULL,g_szRestoreRegPath,szRegKrnlPath,szDispatchRegKrnlPath,szDispatchRegUserPath) != 0 )
	{
		nRet = pfnNtSetValueKey(
			KeyHandle,
			ValueName,
			TitleIndex,
			Type,
			Data,
			DataSize
			);

		return nRet;
	}

//#ifdef Dbg
//	WCHAR szDebugString[2048] = {0};
//	wsprintf(
//		szDebugString,
//		L"szRegKrnlPath = [%s]\r\nszDispatchRegKrnlPath = [%s]\r\nszDispatchRegUserPath = [%s]",
//		szRegKrnlPath,
//		szDispatchRegKrnlPath,
//		szDispatchRegUserPath
//		);
//	DebugLog(DbgInfo,szDebugString);
//#endif

	//
	//Check if request in SandBox
	//
	if( wcsnicmp(szRegKrnlPath,g_szRestoreFullRegKrnlPath,lstrlenW(g_szRestoreFullRegKrnlPath)) == 0 )
	{
		nRet = pfnNtSetValueKey(
			KeyHandle,
			ValueName,
			TitleIndex,
			Type,
			Data,
			DataSize
			);

		return nRet;
	}

	//
	//Init Subkey
	//
	InitSubkeyForRegDispatch(szDispatchRegUserPath);

	//
	//Dispatch
	//
	HKEY hkKey;
	if( CAPI_RegOpenKeyExW(HKEY_USERS,szDispatchRegUserPath,NULL,KEY_WRITE,&hkKey) == ERROR_SUCCESS )
	{
		nRet = pfnNtSetValueKey(
			hkKey,
			ValueName,
			TitleIndex,
			Type,
			Data,
			DataSize
			);

		RegCloseKey(hkKey);

		return nRet;
	}
	CAPI_RegCloseKey(hkKey);

	nRet = pfnNtSetValueKey(
		KeyHandle,
		ValueName,
		TitleIndex,
		Type,
		Data,
		DataSize
		);

	return nRet;
}

NTSTATUS
NTAPI
NtSetValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN ULONG  TitleIndex  OPTIONAL,
	IN ULONG  Type,
	IN PVOID  Data,
	IN ULONG  DataSize
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtSetValueKey addFun = (__pfnNtSetValueKey)GetProcAddress(hDll,"NtSetValueKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,ValueName,TitleIndex,Type,Data,DataSize);
	}
	return ret;
}