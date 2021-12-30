#include <windows.h>
#include <ntsecapi.h>
#include <winbase.h>

#include "./Dispatch_NTDLL_NtOpenFile.h"
#include "../Common///DebugLog.h"
#include "./HookHelp.h"
#include "./HookHelp_File.h"
#include "./GetProcAddressEx.h"
#include "../Config/Config.h"
#include "./Main.h"
#include "./NativeAPI_NTDLL.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtQueryAttributesFile.h"
#include "./Dispatch_NTDLL_NtClose.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"
#include "./Dispatch_NTDLL_NtDeleteFile.h"



//
//Global
//
__pfnNtOpenFile pfnNtOpenFile = NULL;



//
//Dispatch_NTDLL_NtOpenFile Functions
//
NTSTATUS
NTAPI
OnNtOpenFile(
	OUT PHANDLE  FileHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PIO_STATUS_BLOCK  IoStatusBlock,
	IN ULONG  ShareAccess,
	IN ULONG  OpenOptions)
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
		nRet = pfnNtOpenFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			ShareAccess,
			OpenOptions
			);

		return nRet;
	}

	NTSTATUS nStatusQuery;
	FILE_BASIC_INFORMATION fbiFileAttributes;
	nStatusQuery = NtQueryAttributesFile(ObjectAttributes,&fbiFileAttributes);
	if( nStatusQuery != STATUS_OBJECT_NAME_NOT_FOUND &&
		nStatusQuery != STATUS_SUCCESS
		)
	{
		nRet = pfnNtOpenFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			ShareAccess,
			OpenOptions
			);

		return nRet;
	}

	//(\\??\\C:\\WINDOWS\\XXX)
	WCHAR szLongNtPath[MAX_NAME] = {NULL};
	//(\\??\\C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)
	WCHAR szDispatchNtPath[MAX_NAME] = {NULL};
	UNICODE_STRING usDispatchNtPath;

	WCHAR szBuffer[MAX_NAME] = {NULL};
	lstrcpyW(szBuffer,ObjectAttributes->ObjectName->Buffer);

	if( GetLongNtPathName(NULL,ObjectAttributes->RootDirectory,szBuffer,g_szRestoreNtPath,szLongNtPath,szDispatchNtPath) != 0 )
	{
#ifdef Dbg
		//DebugLog(DbgInfo,L"GetLongNtPathName failed! szLongNtPath=");
		//OutputDebugString(szLongNtPath);
#endif

		nRet = pfnNtOpenFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			ShareAccess,
			OpenOptions
			);

		return nRet;
	}

	//
	//Init UNICODE_STRING
	//
	RtlInitUnicodeString(&usDispatchNtPath,szDispatchNtPath);

	////OutputDebugString(L"\r\nNtOpenFile szLongNtPath=");
	////OutputDebugString(szLongNtPath);

	//
	//Check if request in SandBox
	//
	if( wcsnicmp(szLongNtPath,g_szRestoreNtPath,lstrlenW(g_szRestoreNtPath)) == 0 )
	{
#ifdef Dbg
		//DebugLog(DbgInfo,L"request in SandBox! szLongNtPath=");
		//OutputDebugString(szLongNtPath);
#endif

		nRet = pfnNtOpenFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			ShareAccess,
			OpenOptions
			);

		return nRet;
	}

	//
	//Get FileName
	//
	OBJECT_ATTRIBUTES oaObjAttrib = *(POBJECT_ATTRIBUTES)ObjectAttributes;

	if( nStatusQuery == STATUS_OBJECT_NAME_NOT_FOUND )//FILE_DOES_NOT_EXIST
	{
		int iFileDispatch = -1;
		iFileDispatch = QueryDispatchedFile(szDispatchNtPath);
		if( iFileDispatch == -1 )
		{
#ifdef Dbg
			//DebugLog(DbgInfo,L"iFileDispatch == -1! szLongNtPath=");
			//OutputDebugString(szLongNtPath);
#endif

			nRet = FILE_DOES_NOT_EXIST;
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
			oaObjAttrib.ObjectName->Length = usDispatchNtPath.Length;
			oaObjAttrib.ObjectName->MaximumLength = usDispatchNtPath.MaximumLength;
			oaObjAttrib.ObjectName->Buffer = usDispatchNtPath.Buffer;

#ifdef Dbg
			//DebugLog(DbgInfo,L"Dispatch! szLongNtPath=");
			//OutputDebugString(szLongNtPath);
#endif

			nRet = pfnNtOpenFile(
				FileHandle,
				DesiredAccess,
				&oaObjAttrib,
				IoStatusBlock,
				ShareAccess,
				OpenOptions
				);

			return nRet;
		}
	}
	else
	if(	nStatusQuery == STATUS_SUCCESS )//FILE_EXISTS
	{
#ifdef Dbg
		//DebugLog(DbgInfo,L"nStatusQuery == STATUS_SUCCESS! szLongNtPath=");
		//OutputDebugString(szLongNtPath);
#endif

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
			HANDLE hFile = NULL;
			IO_STATUS_BLOCK IoSB;
			NTSTATUS nCreateFile;

			//
			//Check if the file is a directory
			//
			nCreateFile = NtCreateFile(
						&hFile,
						FILE_GENERIC_READ | FILE_GENERIC_WRITE | SYNCHRONIZE,
						&oaObjAttrib,
						&IoSB,
						NULL,
						FILE_ATTRIBUTE_NORMAL,
						FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
						FILE_OPEN,
						FILE_SYNCHRONOUS_IO_NONALERT | FILE_DIRECTORY_FILE,
						NULL,
						NULL
						);
			if( nCreateFile == STATUS_SUCCESS &&
				IoSB.Information == FILE_OPENED
				)
			{
				NtClose(hFile);

				nRet = pfnNtOpenFile(
					FileHandle,
					DesiredAccess,
					&oaObjAttrib,
					IoStatusBlock,
					ShareAccess,
					OpenOptions
					);

				return nRet;
			}

			//
			//Dispatch
			//
			if( oaObjAttrib.RootDirectory != NULL )
			{
				oaObjAttrib.RootDirectory = NULL;
			}
			oaObjAttrib.ObjectName->Length = usDispatchNtPath.Length;
			oaObjAttrib.ObjectName->MaximumLength = usDispatchNtPath.MaximumLength;
			oaObjAttrib.ObjectName->Buffer = usDispatchNtPath.Buffer;

			nRet = pfnNtOpenFile(
				FileHandle,
				DesiredAccess,
				&oaObjAttrib,
				IoStatusBlock,
				ShareAccess,
				OpenOptions
				);

			return nRet;
		}
	}

	nRet = pfnNtOpenFile(
		FileHandle,
		DesiredAccess,
		ObjectAttributes,
		IoStatusBlock,
		ShareAccess,
		OpenOptions
		);

	return nRet;
}

NTSTATUS
NTAPI
NtOpenFile(
	OUT PHANDLE  FileHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PIO_STATUS_BLOCK  IoStatusBlock,
	IN ULONG  ShareAccess,
	IN ULONG  OpenOptions)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtOpenFile addFun = (__pfnNtOpenFile)GetProcAddress(hDll,"NtOpenFile");
	if(addFun)
	{
		ret = addFun(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,ShareAccess,OpenOptions);
	}
	return ret;
}