#include <windows.h>
#include <ntsecapi.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "./Dispatch_NTDLL_NtCreateKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./HookHelp_Reg.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"
#include "./NativeAPI_NTDLL.h"
#include "./Dispatch_NTDLL_NtCreateFile.h"
#include "./Dispatch_NTDLL_NtOpenKey.h"
#include "./Dispatch_NTDLL_NtClose.h"



//
//Global
//
__pfnNtCreateKey pfnNtCreateKey = NULL;



//
//Dispatch_NTDLL_NtCreateKey Functions
//
NTSTATUS
NTAPI
OnNtCreateKey(
	OUT PHANDLE  KeyHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	IN ULONG  TitleIndex,
	IN PUNICODE_STRING  Class  OPTIONAL,
	IN ULONG  CreateOptions,
	OUT PULONG  Disposition  OPTIONAL
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
		nRet = pfnNtCreateKey(
			KeyHandle,
			DesiredAccess,
			ObjectAttributes,
			TitleIndex,
			Class,
			CreateOptions,
			Disposition
			);

		return nRet;
	}

	WCHAR szRegKrnlPath[MAX_NAME] = {NULL};
	WCHAR szDispatchRegKrnlPath[MAX_NAME] = {NULL};
	WCHAR szDispatchRegUserPath[MAX_NAME] = {NULL};

	WCHAR szObjectName[MAX_NAME] = {NULL};
	lstrcpyW(szObjectName,ObjectAttributes->ObjectName->Buffer);

	if( GetRegistryPathName(NULL,ObjectAttributes->RootDirectory,szObjectName,g_szRestoreRegPath,szRegKrnlPath,szDispatchRegKrnlPath,szDispatchRegUserPath) != 0 )
	{
		nRet = pfnNtCreateKey(
			KeyHandle,
			DesiredAccess,
			ObjectAttributes,
			TitleIndex,
			Class,
			CreateOptions,
			Disposition
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
		nRet = pfnNtCreateKey(
			KeyHandle,
			DesiredAccess,
			ObjectAttributes,
			TitleIndex,
			Class,
			CreateOptions,
			Disposition
			);

		return nRet;
	}

	//
	//Check if opens an existing key
	//
	NTSTATUS nOpenKey;
	HANDLE hKeyHandle = NULL;

	nOpenKey = NtOpenKey(&hKeyHandle, KEY_READ, ObjectAttributes);

	if( nOpenKey == STATUS_SUCCESS )//key exists
	{
		NtClose(hKeyHandle);

		nRet = pfnNtCreateKey(
			KeyHandle,
			DesiredAccess,
			ObjectAttributes,
			TitleIndex,
			Class,
			CreateOptions,
			Disposition
			);

		return nRet;
	}
	NtClose(hKeyHandle);

	////
	////Dispatch
	////
	//UNICODE_STRING usDispatchRegPath;

	////
	////Init UNICODE_STRING
	////
	//RtlInitUnicodeString(&usDispatchRegPath,szDispatchRegKrnlPath);

	//OBJECT_ATTRIBUTES oaObjAttrib;

	//InitializeObjectAttributes(
	//	&oaObjAttrib,
	//	&usDispatchRegPath,
	//	OBJ_CASE_INSENSITIVE,
	//	NULL,
	//	NULL);

	//nRet = pfnNtCreateKey(
	//	KeyHandle,
	//	DesiredAccess,
	//	&oaObjAttrib,
	//	TitleIndex,
	//	Class,
	//	CreateOptions,
	//	Disposition
	//	);

	nRet = pfnNtCreateKey(
		KeyHandle,
		DesiredAccess,
		ObjectAttributes,
		TitleIndex,
		Class,
		CreateOptions,
		Disposition
		);

	return nRet;
}

NTSTATUS
NTAPI
NtCreateKey(
	OUT PHANDLE  KeyHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	IN ULONG  TitleIndex,
	IN PUNICODE_STRING  Class  OPTIONAL,
	IN ULONG  CreateOptions,
	OUT PULONG  Disposition  OPTIONAL
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtCreateKey addFun = (__pfnNtCreateKey)GetProcAddress(hDll,"NtCreateKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,DesiredAccess,ObjectAttributes,TitleIndex,Class,CreateOptions,Disposition);
	}
	return ret;
}