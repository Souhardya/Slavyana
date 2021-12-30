#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQueryAttributesFile.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./HookHelp_File.h"
#include "./GetProcAddressEx.h"
#include "../Config/Config.h"
#include "./Main.h"



//
//Global
//
__pfnNtQueryAttributesFile pfnNtQueryAttributesFile = NULL;



//
//Dispatch_NTDLL_NtQueryAttributesFile Functions
//
NTSTATUS
NTAPI
OnNtQueryAttributesFile(
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	OUT PFILE_BASIC_INFORMATION FileAttributes)
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
		nRet = pfnNtQueryAttributesFile(
			ObjectAttributes,
			FileAttributes
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
		nRet = pfnNtQueryAttributesFile(
			ObjectAttributes,
			FileAttributes
			);

		return nRet;
	}

	//
	//Check if request in SandBox
	//
	if( wcsnicmp(szLongNtPath,g_szRestoreNtPath,lstrlenW(g_szRestoreNtPath)) == 0 )
	{
		nRet = pfnNtQueryAttributesFile(
			ObjectAttributes,
			FileAttributes
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

		nRet = pfnNtQueryAttributesFile(
			&oaObjAttrib,
			FileAttributes
			);

		return nRet;
	}

	nRet = pfnNtQueryAttributesFile(
		ObjectAttributes,
		FileAttributes
		);

	return nRet;
}

NTSTATUS
NTAPI
NtQueryAttributesFile(
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	OUT PFILE_BASIC_INFORMATION FileAttributes)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryAttributesFile addFun = (__pfnNtQueryAttributesFile)GetProcAddress(hDll,"NtQueryAttributesFile");
	if(addFun)
	{
		ret = addFun(ObjectAttributes,FileAttributes);
	}
	return ret;
}