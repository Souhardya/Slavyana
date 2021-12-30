#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQueryFullAttributesFile.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./HookHelp_File.h"
#include "./GetProcAddressEx.h"
#include "./Main.h"



//
//Global
//
__pfnNtQueryFullAttributesFile pfnNtQueryFullAttributesFile = NULL;



//
//Dispatch_NTDLL_NtQueryFullAttributesFile Functions
//
NTSTATUS
NTAPI
OnNtQueryFullAttributesFile(
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PFILE_NETWORK_OPEN_INFORMATION  FileInformation)
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
		nRet = pfnNtQueryFullAttributesFile(
			ObjectAttributes,
			FileInformation
			);

		return nRet;
	}

	//(\\??\\C:\\WINDOWS\\XXX)
	WCHAR szLongNtPath[MAX_NAME] = {NULL};
	//(\\??\\C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)
	WCHAR szDispatchNtPath[MAX_NAME] = {NULL};

	WCHAR szBuffer[MAX_NAME] = {NULL};
	lstrcpyW(szBuffer,ObjectAttributes->ObjectName->Buffer);

	if( GetLongNtPathName(NULL,ObjectAttributes->RootDirectory,szBuffer,g_szRestoreNtPath,szLongNtPath,szDispatchNtPath) != 0 )
	{
		nRet = pfnNtQueryFullAttributesFile(
			ObjectAttributes,
			FileInformation
			);

		return nRet;
	}

	//
	//Check if request in SandBox
	//
	if( wcsnicmp(szLongNtPath,g_szRestoreNtPath,lstrlenW(g_szRestoreNtPath)) == 0 )
	{
		nRet = pfnNtQueryFullAttributesFile(
			ObjectAttributes,
			FileInformation
			);

		return nRet;
	}

	//
	//Check if file dispatched
	//
	HANDLE hFile = NULL;
	OBJECT_ATTRIBUTES oaObjAttrib = *(POBJECT_ATTRIBUTES)ObjectAttributes;

	int iFileDispatch = -1;
	iFileDispatch = QueryDispatchedFile(szDispatchNtPath);
	if( iFileDispatch == -1 )
	{
		nRet = STATUS_UNSUCCESSFUL;
		return nRet;
	}
	else
	if( iFileDispatch == 1 )
	{
		//
		//Dispatch
		//
		if( oaObjAttrib.RootDirectory != NULL )
		{
			oaObjAttrib.RootDirectory = NULL;
		}
		oaObjAttrib.ObjectName->Length = (USHORT) lstrlenW(szDispatchNtPath)*2;
		oaObjAttrib.ObjectName->MaximumLength =  (USHORT) lstrlenW(szDispatchNtPath)*2;
		oaObjAttrib.ObjectName->Buffer = szDispatchNtPath;

		nRet = pfnNtQueryFullAttributesFile(
			&oaObjAttrib,
			FileInformation
			);

		return nRet;
	}

	nRet = pfnNtQueryFullAttributesFile(
		ObjectAttributes,
		FileInformation
		);

	return nRet;
}

NTSTATUS
NTAPI
NtQueryFullAttributesFile(
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PFILE_NETWORK_OPEN_INFORMATION  FileInformation)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryFullAttributesFile addFun = (__pfnNtQueryFullAttributesFile)GetProcAddress(hDll,"NtQueryFullAttributesFile");
	if(addFun)
	{
		ret = addFun(ObjectAttributes,FileInformation);
	}
	return ret;
}