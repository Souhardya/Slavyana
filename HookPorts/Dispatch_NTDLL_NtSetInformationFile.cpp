#include <windows.h>
#include <ntsecapi.h>
#include <winbase.h>
#include <stdio.h>

#include "./Dispatch_NTDLL_NtSetInformationFile.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./HookHelp_File.h"
#include "./GetProcAddressEx.h"
#include "../Config/Config.h"
#include "./Main.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"
#include "./Dispatch_NTDLL_NtCreateFile.h"
#include "./Dispatch_NTDLL_NtClose.h"
#include "./NativeAPI_NTDLL.h"



//
//Global
//
__pfnNtSetInformationFile pfnNtSetInformationFile = NULL;



//
//Dispatch_NTDLL_NtSetInformationFile Functions
//
NTSTATUS
NTAPI
OnNtSetInformationFile(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PVOID FileInformation,
    IN ULONG Length,
    IN FILE_INFORMATION_CLASS FileInformationClass)
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
		nRet = pfnNtSetInformationFile(
			FileHandle,
			IoStatusBlock,
			FileInformation,
			Length,
			FileInformationClass
			);

		return nRet;
	}

	//(\\??\\C:\\WINDOWS\\XXX)
	WCHAR szLongNtPath[MAX_NAME] = {NULL};
	//(\\??\\C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)
	WCHAR szDispatchNtPath[MAX_NAME] = {NULL};

	if( GetLongNtPathName(FileHandle,NULL,NULL,g_szRestoreNtPath,szLongNtPath,szDispatchNtPath) != 0 )
	{
		nRet = pfnNtSetInformationFile(
			FileHandle,
			IoStatusBlock,
			FileInformation,
			Length,
			FileInformationClass
			);

		return nRet;
	}

	//
	//Check if request in SandBox
	//
	if( wcsnicmp(szLongNtPath,g_szRestoreNtPath,lstrlenW(g_szRestoreNtPath)) == 0 )
	{
		nRet = pfnNtSetInformationFile(
			FileHandle,
			IoStatusBlock,
			FileInformation,
			Length,
			FileInformationClass
			);

		return nRet;
	}

	//
	//Set right access for FileAttributes
	//
	ULONG uFileAttributes = FILE_ALL_ACCESS;
	if( FileInformationClass == FileAllocationInformation ||
		FileInformationClass == FileEndOfFileInformation
		)
	{
		uFileAttributes = FILE_ALL_ACCESS;
	}
	else
	{
		uFileAttributes = FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES;
	}

//#ifdef Dbg
//	WCHAR szDebugString[512] = {0};
//	wsprintf(
//		szDebugString,
//		L"szLongNtPath = [%s]\r\ndwFileAttributes = [%d]  FileInformationClass = [%d]",
//		szLongNtPath,
//		uFileAttributes,
//		FileInformationClass
//		);
//	DebugLog(DbgInfo,szDebugString);
//#endif

	//
	//Check if file dispatched
	//
	int iFileDispatch = -1;
	iFileDispatch = QueryDispatchedFile(szDispatchNtPath);
	if( iFileDispatch == -1 )
	{
		nRet = STATUS_UNSUCCESSFUL;
		return nRet;
	}
	else
	if( iFileDispatch == 1 &&
		FileInformationClass != FileDispositionInformation
		)
	{
		//
		//Dispatch
		//
		//
		//Init UNICODE_STRING
		//
		UNICODE_STRING usNtFileName;
		RtlInitUnicodeString(&usNtFileName,szDispatchNtPath);

		NTSTATUS nStatus;
		HANDLE hFile = NULL;
		IO_STATUS_BLOCK IoSB;
		FILE_BASIC_INFORMATION stFileBasicInfo = {NULL};
		OBJECT_ATTRIBUTES oaObjAttrib;

		InitializeObjectAttributes(
			&oaObjAttrib,
			&usNtFileName,
			OBJ_CASE_INSENSITIVE,
			NULL,
			NULL);

		nStatus = NtCreateFile(
			&hFile,
			FILE_GENERIC_READ | FILE_GENERIC_WRITE | SYNCHRONIZE,
			&oaObjAttrib,
			&IoSB,
			NULL,
			uFileAttributes,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			FILE_OPEN,
			FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			NULL
			);

		if( nStatus == STATUS_SUCCESS )
		{
			nRet = pfnNtSetInformationFile(
				hFile,
				IoStatusBlock,
				FileInformation,
				Length,
				FileInformationClass
				);
		}
		else
		{
			nRet = STATUS_UNSUCCESSFUL;
		}

		NtClose(hFile);

		return nRet;
	}

	//
	//Check if is a [FileDispositionInformation] request
	//
	if( FileInformationClass == FileDispositionInformation )
	{
		ULONG uFileAttributes = QueryFileAttributes(szLongNtPath);

//#ifdef Dbg
//		WCHAR szDebugString[512] = {0};
//		wsprintf(
//			szDebugString,
//			L"szLongNtPath = [%s]\r\ndwFileAttributes = [%d]  FileInformationClass = [%d]",
//			szLongNtPath,
//			uFileAttributes,
//			FileInformationClass
//			);
//		DebugLog(DbgInfo,szDebugString);
//#endif

		if( SetFileDeletedMark(szLongNtPath,szDispatchNtPath,uFileAttributes) == 0 )
		{
			nRet = STATUS_SUCCESS;
		}
		else
		{
			nRet = STATUS_ACCESS_DENIED;
		}

		return nRet;
	}

	//
	//Check if is a [FileRenameInformation] request
	//
	if( FileInformationClass == FileRenameInformation )
	{
		PFILE_RENAME_INFORMATION lpFileRenameInfo = NULL;
		lpFileRenameInfo = (PFILE_RENAME_INFORMATION)FileInformation;

		if( lpFileRenameInfo->RootDirectory != NULL )
		{
#ifdef Dbg
			DebugLog(DbgInfo,L"lpFileRenameInfo->RootDirectory != NULL!!! ERROR!");
#endif

			nRet = STATUS_ACCESS_DENIED;
			return nRet;
		}

		WCHAR szNewNtFileName[MAX_NAME] = {NULL};
		lstrcpynW(szNewNtFileName,lpFileRenameInfo->FileName,(lpFileRenameInfo->FileNameLength/2)+1);

		WCHAR szNewLongNtFileName[MAX_NAME] = {NULL};
		WCHAR szNewDispatchNtFileName[MAX_NAME] = {NULL};

		GetLongNtPathName(NULL,NULL,szNewNtFileName,g_szRestoreNtPath,szNewLongNtFileName,szNewDispatchNtFileName);

		ULONG uFileAttributes = QueryFileAttributes(szLongNtPath);

		//
		//Init Directory
		//
		InitDirectoryForFileDispatch(szDispatchNtPath);

		//
		//Close handle
		//
		NtClose(FileHandle);

		//
		//Copy file
		//
		CloneFileToDispatchPath(szLongNtPath,szNewDispatchNtFileName,uFileAttributes);

		SetFileDeletedMark(szLongNtPath,szDispatchNtPath,uFileAttributes);

		nRet = STATUS_SUCCESS;
		return nRet;
	}

	//
	//Check if is a [FileBasicInformation] request
	//
	if( FileInformationClass == FileBasicInformation )
	{
		nRet = STATUS_SUCCESS;

		return nRet;
	}

	//
	//Dispatch list
	//
	//FileDispositionInformation
	//FileBasicInformation
	//FileLinkInformation
	//FileStreamInformation
	//FileReparsePointInformation
	//FileAccessInformation
	//FileRenameInformation
	//FileAllocationInformation
	//FileEndOfFileInformation

	if(	FileInformationClass == FileLinkInformation ||
		FileInformationClass == FileStreamInformation ||
		FileInformationClass == FileReparsePointInformation ||
		FileInformationClass == FileAccessInformation ||
		FileInformationClass == FileAllocationInformation ||
		FileInformationClass == FileEndOfFileInformation
		)
	{
		ULONG uFileAttributes = QueryFileAttributes(szLongNtPath);

		//
		//Init Directory
		//
		InitDirectoryForFileDispatch(szDispatchNtPath);

		//
		//Copy file
		//
		CloneFileToDispatchPath(szLongNtPath,szDispatchNtPath,uFileAttributes);

		//
		//Init UNICODE_STRING
		//
		UNICODE_STRING usNtFileName;
		RtlInitUnicodeString(&usNtFileName,szDispatchNtPath);

		NTSTATUS nStatus;
		HANDLE hFile = NULL;
		IO_STATUS_BLOCK IoSB;
		FILE_BASIC_INFORMATION stFileBasicInfo = {NULL};
		OBJECT_ATTRIBUTES oaObjAttrib;

		InitializeObjectAttributes(
			&oaObjAttrib,
			&usNtFileName,
			OBJ_CASE_INSENSITIVE,
			NULL,
			NULL);

		nStatus = NtCreateFile(
			&hFile,
			FILE_GENERIC_READ | FILE_GENERIC_WRITE | SYNCHRONIZE,
			&oaObjAttrib,
			&IoSB,
			NULL,
			uFileAttributes,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			FILE_OPEN,
			FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			NULL
			);

		if( nStatus == STATUS_SUCCESS )
		{
			nRet = pfnNtSetInformationFile(
				hFile,
				IoStatusBlock,
				FileInformation,
				Length,
				FileInformationClass
				);
		}
		else
		{
			nRet = STATUS_UNSUCCESSFUL;
		}

		NtClose(hFile);

		return nRet;
	}

	nRet = pfnNtSetInformationFile(
		FileHandle,
		IoStatusBlock,
		FileInformation,
		Length,
		FileInformationClass
		);

	return nRet;
}

NTSTATUS
NTAPI
NtSetInformationFile(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PVOID FileInformation,
    IN ULONG Length,
    IN FILE_INFORMATION_CLASS FileInformationClass)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtSetInformationFile addFun = (__pfnNtSetInformationFile)GetProcAddress(hDll,"NtSetInformationFile");
	if(addFun)
	{
		ret = addFun(FileHandle,IoStatusBlock,FileInformation,Length,FileInformationClass);
	}
	return ret;
}