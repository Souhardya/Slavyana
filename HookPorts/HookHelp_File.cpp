#include <windows.h>
#include <ntsecapi.h>
#include <stdio.h>
#include <shlobj.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "./HookHelp_File.h"
#include "./Main.h"
#include "../Config/Config.h"
#include "../Common/DebugLog.h"

#include "./CloneAPI_KERNEL32.h"
#include "./CloneAPI_FLTLIB.h"
#include "./CloneAPI_SHELL32.h"

#include "./NativeAPI_NTDLL.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"
#include "./Dispatch_NTDLL_NtSetInformationFile.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"
#include "./Dispatch_NTDLL_NtCreateFile.h"
#include "./Dispatch_NTDLL_NtQueryAttributesFile.h"
#include "./Dispatch_NTDLL_NtClose.h"
#include "./Dispatch_NTDLL_NtDeleteFile.h"



//
//Global
//



//
//HookHelp Functions
//
int GetLongNtPathName(
	IN HANDLE hFileHandle,				//Handle to the file whose path is being requested
	IN HANDLE hRootDirectory,			//Handle to the directory whose path is being requested
	IN WCHAR * lpszFileName,			//Pointer to the file name
	IN WCHAR * lpszRestoreNtPath,		//Pointer to the restore path
	OUT WCHAR * lpszLongNtPath,			//Pointer to the buffer to receive the nt path
	OUT WCHAR * lpszDispatchNtPath		//Pointer to the buffer to receive the dispatch path
)
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = 0;

	WCHAR szDeviceName[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1)
	WCHAR szVolumeName[MAX_NAME] = {NULL};//(C:)
	WCHAR szFileDosName[MAX_NAME] = {NULL};//(\\WINDOWS\\XXX)

	WCHAR szLinkFileDevPath[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1\\WINDOWS\\XXX)
	WCHAR szLinkFileDosPath[MAX_NAME] = {NULL};//(C:\\WINDOWS\\XXX)

	////(\\??\\C:\\WINDOWS\\XXX)
	WCHAR szLongNtPath[MAX_NAME] = {NULL};
	////(\\??\\C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)
	WCHAR szDispatchNtPath[MAX_NAME] = {NULL};

	WCHAR szRestoreNtPath[MAX_NAME] = {NULL};

	wsprintfW(szRestoreNtPath,L"%s\\%s\\",lpszRestoreNtPath,CONF_SandBox_Drive_Name);

	//
	//Check if the [hFileHandle] is NULL
	//
	if( hFileHandle != NULL )
	{
		//
		//Get file dos name(\\WINDOWS\\XXX)
		//
		if( QueryFileNameByNtQueryInformationFile(hFileHandle,szFileDosName) == -1 )
		{
			return -1;
		}

		//
		//Get file device name(\\Device\\HarddiskVolume1\\WINDOWS\\XXX)
		//
		if( QueryNameInfoByNtQueryObject(hFileHandle,szLinkFileDevPath) == -1 )
		{
			return -1;
		}

		lstrcpynW(szDeviceName,szLinkFileDevPath,lstrlenW(szLinkFileDevPath) - lstrlenW(szFileDosName) + 1);

		//
		//Convert [DeviceName] to [DosName]
		//
		CAPI_FilterGetDosName(szDeviceName,szVolumeName,MAX_NAME);

		wsprintfW(szLinkFileDosPath,L"%s%s",szVolumeName,szFileDosName);

		if( wcsnicmp(&szLinkFileDosPath[lstrlenW(L"C")],L":\\",lstrlenW(L":\\")) != 0 )
		{
			return -1;
		}

		//
		//Convert [DosPathName] to [NtPathName]
		//
		UNICODE_STRING usNtFileName;
		RtlDosPathNameToNtPathName_U(szLinkFileDosPath,&usNtFileName,NULL,NULL);

		lstrcpyW(lpszLongNtPath,usNtFileName.Buffer);
		lstrcpyW(szLongNtPath,usNtFileName.Buffer);

		//
		//Free UNICODE_STRING
		//
		RtlFreeUnicodeString(&usNtFileName);

		//
		//Make lpszDispatchNtPath
		//
		WCHAR szRestorePath[MAX_NAME] = {NULL};

		lstrcpynW(szRestorePath,szVolumeName,lstrlenW(szVolumeName));
		lstrcatW(szRestorePath,&szLinkFileDosPath[lstrlenW(szVolumeName)]);

		lstrcatW(szRestoreNtPath,szRestorePath);

		lstrcpyW(lpszDispatchNtPath,szRestoreNtPath);
		lstrcpyW(szDispatchNtPath,szRestoreNtPath);
	}

	//
	//Check if the [hRootDirectory] is NULL
	//
	if( hRootDirectory != NULL )
	{
		//
		//Get file dos name(\\WINDOWS\\XXX)
		//
		if( QueryFileNameByNtQueryInformationFile(hRootDirectory,szFileDosName) == -1 )
		{
			return -1;
		}

		//
		//Get file device name(\\Device\\HarddiskVolume1\\WINDOWS\\XXX)
		//
		if( QueryNameInfoByNtQueryObject(hRootDirectory,szLinkFileDevPath) == -1 )
		{
			return -1;
		}

		lstrcpynW(szDeviceName,szLinkFileDevPath,lstrlenW(szLinkFileDevPath) - lstrlenW(szFileDosName) + 1);

		//
		//Convert [DeviceName] to [DosName]
		//
		CAPI_FilterGetDosName(szDeviceName,szVolumeName,MAX_NAME);

		wsprintfW(szLinkFileDosPath,L"%s%s\\%s",szVolumeName,szFileDosName,lpszFileName);

		if( wcsnicmp(&szLinkFileDosPath[lstrlenW(L"C")],L":\\",lstrlenW(L":\\")) != 0 )
		{
			return -1;
		}

		//
		//Convert [DosPathName] to [NtPathName]
		//
		UNICODE_STRING usNtFileName;
		RtlDosPathNameToNtPathName_U(szLinkFileDosPath,&usNtFileName,NULL,NULL);

		lstrcpyW(lpszLongNtPath,usNtFileName.Buffer);
		lstrcpyW(szLongNtPath,usNtFileName.Buffer);

		//
		//Free UNICODE_STRING
		//
		RtlFreeUnicodeString(&usNtFileName);

		//
		//Make lpszDispatchNtPath
		//
		WCHAR szRestorePath[MAX_NAME] = {NULL};

		lstrcpynW(szRestorePath,szVolumeName,lstrlenW(szVolumeName));
		lstrcatW(szRestorePath,&szLinkFileDosPath[lstrlenW(szVolumeName)]);

		lstrcatW(szRestoreNtPath,szRestorePath);

		lstrcpyW(lpszDispatchNtPath,szRestoreNtPath);
		lstrcpyW(szDispatchNtPath,szRestoreNtPath);
	}

	//
	//Check if the [hFileHandle]&[hRootDirectory] is NULL
	//
	if( hFileHandle == NULL &&
		hRootDirectory == NULL
		)
	{
		//
		//Check if is NtPathName
		//
		lstrcpyW(szLinkFileDevPath,lpszFileName);

		if( wcsnicmp(szLinkFileDevPath,L"\\??\\",lstrlenW(L"\\??\\")) != 0 )
		{
			return -1;
		}
		if( wcsnicmp(&szLinkFileDevPath[lstrlenW(L"\\??\\C")],L":\\",lstrlenW(L":\\")) != 0 )
		{
			return -1;
		}

		lstrcpyW(szLinkFileDosPath,&szLinkFileDevPath[lstrlenW(L"\\??\\")]);

		//
		//Convert [DosPathName] to [NtPathName]
		//
		UNICODE_STRING usNtFileName;
		RtlDosPathNameToNtPathName_U(szLinkFileDosPath,&usNtFileName,NULL,NULL);

		lstrcpyW(lpszLongNtPath,usNtFileName.Buffer);
		lstrcpyW(szLongNtPath,usNtFileName.Buffer);

		//
		//Free UNICODE_STRING
		//
		RtlFreeUnicodeString(&usNtFileName);

		//
		//Make lpszDispatchNtPath
		//
		WCHAR szRestorePath[MAX_NAME] = {NULL};

		lstrcpynW(szRestorePath,szLinkFileDosPath,lstrlenW(L"C:"));
		lstrcatW(szRestorePath,&szLinkFileDosPath[lstrlenW(L"C:")]);

		lstrcatW(szRestoreNtPath,szRestorePath);

		lstrcpyW(lpszDispatchNtPath,szRestoreNtPath);
		lstrcpyW(szDispatchNtPath,szRestoreNtPath);
	}

	//
	//Check if is FileNtPath
	//
	if( wcsnicmp(&szLongNtPath[lstrlenW(L"\\??\\C")],L":\\",lstrlenW(L":\\")) != 0 )
	{
		return -1;
	}

	//
	//Check if is ShortPath
	//
	if( wcsstr(szLinkFileDosPath,L"~") )
	{
		WCHAR szLinkFileDosLongPath[MAX_NAME] = {NULL};
		lstrcpyW(szLinkFileDosLongPath,szLinkFileDosPath);

		//
		//Convert [DosPathName] to [NtPathName]
		//
		UNICODE_STRING usNtFileName;
		RtlDosPathNameToNtPathName_U(szLinkFileDosPath,&usNtFileName,NULL,NULL);

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
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			FILE_OPEN_IF,
			FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			NULL
			);
		if( nStatus != STATUS_SUCCESS )
		{
			nStatus = NtCreateFile(
				&hFile,
				FILE_GENERIC_READ | FILE_GENERIC_WRITE | SYNCHRONIZE,
				&oaObjAttrib,
				&IoSB,
				NULL,
				FILE_ATTRIBUTE_NORMAL,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				FILE_OPEN,
				FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
				NULL,
				NULL
				);
		}

		if( nStatus == STATUS_SUCCESS )
		{
			iRet = 0;

			//
			//Get file dos name(\\WINDOWS\\XXX)
			//
			QueryFileNameByNtQueryInformationFile(hFile,szFileDosName);

			//
			//Get file device name(\\Device\\HarddiskVolume1\\WINDOWS\\XXX)
			//
			QueryNameInfoByNtQueryObject(hFileHandle,szLinkFileDevPath);

			lstrcpynW(szDeviceName,szLinkFileDevPath,lstrlenW(szLinkFileDevPath) - lstrlenW(szFileDosName) + 1);

			//
			//Convert [DeviceName] to [DosName]
			//
			CAPI_FilterGetDosName(szDeviceName,szVolumeName,MAX_NAME);

			wsprintfW(szLinkFileDosPath,L"%s%s",szVolumeName,szFileDosName);

			//
			//Convert [ShortPathName] to [LongPathName]
			//
			if( CAPI_GetLongPathNameW(szLinkFileDosPath,szLinkFileDosLongPath,MAX_NAME) )
			{
				if( lstrlenW(szLinkFileDosPath) < lstrlenW(szLinkFileDosLongPath) )
				{
					//
					//Convert ShortPath to LongPath
					//
					lstrcpyW(szLinkFileDosPath,szLinkFileDosLongPath);
				}
			}

			//
			//Convert [DosPathName] to [NtPathName]
			//
			UNICODE_STRING usNewNtFileName;
			RtlDosPathNameToNtPathName_U(szLinkFileDosPath,&usNewNtFileName,NULL,NULL);

			lstrcpyW(lpszLongNtPath,usNewNtFileName.Buffer);

			//
			//Free UNICODE_STRING
			//
			RtlFreeUnicodeString(&usNewNtFileName);

			//
			//Make lpszDispatchNtPath
			//
			WCHAR szRestorePath[MAX_NAME] = {NULL};

			wsprintfW(szRestoreNtPath,L"%s\\%s\\",lpszRestoreNtPath,CONF_SandBox_Drive_Name);

			lstrcpynW(szRestorePath,szLinkFileDosPath,lstrlenW(L"C:"));
			lstrcatW(szRestorePath,&szLinkFileDosPath[lstrlenW(L"C:")]);

			lstrcatW(szRestoreNtPath,szRestorePath);

			lstrcpyW(lpszDispatchNtPath,szRestoreNtPath);
		}
		else
		{
			iRet = -1;
		}

		NtClose(hFile);

		if( IoSB.Information == FILE_CREATED )
		{
			NtDeleteFile(&oaObjAttrib);
		}

		//
		//Free UNICODE_STRING
		//
		RtlFreeUnicodeString(&usNtFileName);
	}

	return iRet;
}

ULONG QueryFileAttributes( IN WCHAR * lpszLongNtPath )
{
	//Return Value:
	//-1 = error
	//0 = Not existed
	//n = Attributes Values
	ULONG uRet = -1;

	if( lpszLongNtPath == NULL )
	{
		return uRet;
	}

	WCHAR szLongNtPath[MAX_NAME] = {NULL};
	lstrcpyW(szLongNtPath,lpszLongNtPath);

	//
	//Init UNICODE_STRING
	//
	UNICODE_STRING usNtFileName;
	RtlInitUnicodeString(&usNtFileName,szLongNtPath);

	NTSTATUS nStatusQuery;
	OBJECT_ATTRIBUTES oaObjAttrib;
	FILE_BASIC_INFORMATION fbiFileAttributes;

	InitializeObjectAttributes(
		&oaObjAttrib,
		&usNtFileName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);

	nStatusQuery = NtQueryAttributesFile(&oaObjAttrib,&fbiFileAttributes);

	if( nStatusQuery == STATUS_OBJECT_NAME_NOT_FOUND )//FILE_DOES_NOT_EXIST
	{
		uRet = 0;
	}
	else
	if( nStatusQuery == STATUS_SUCCESS )//FILE_EXISTS
	{
		uRet = fbiFileAttributes.FileAttributes;
	}

	//
	//Free UNICODE_STRING
	//
	//RtlFreeUnicodeString(&usNtFileName);

	return uRet;
}

int SetFileDeletedMark( IN WCHAR * lpszLongNtPath, IN WCHAR * lpszRestoreNtPath ,IN ULONG lpuFileAttributes )
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = -1;

	if( lpszLongNtPath == NULL ||
		lpszRestoreNtPath == NULL
		)
	{
		return iRet;
	}

	WCHAR szRestoreNtPath[MAX_NAME] = {NULL};//(\\??\\C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)
	WCHAR szRestorePath[MAX_NAME] = {NULL};//(C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)

	lstrcpyW(szRestoreNtPath,lpszRestoreNtPath);
	lstrcpyW(szRestorePath,&szRestoreNtPath[lstrlenW(L"\\??\\")]);

	PathRemoveFileSpec(szRestorePath);

	//
	//Init Directory
	//
	CAPI_SHCreateDirectoryExW(NULL,szRestorePath,NULL);

	//
	//Check if the file is a directory
	//
	ULONG uCreateOptions = FILE_SYNCHRONOUS_IO_NONALERT;
	if( lpuFileAttributes == FILE_ATTRIBUTE_DIRECTORY )
	{
		uCreateOptions = FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT;
	}

	//
	//Init UNICODE_STRING
	//
	UNICODE_STRING usNtFileName;
	RtlInitUnicodeString(&usNtFileName,szRestoreNtPath);

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
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN_IF,
		uCreateOptions,
		NULL,
		NULL
		);

	if( nStatus == STATUS_SUCCESS )
	{
		nStatus = NtQueryInformationFile(
			hFile,
			&IoSB,
			&stFileBasicInfo,
			sizeof(FILE_BASIC_INFORMATION),
			FileBasicInformation);

		if( nStatus == STATUS_SUCCESS )
		{
			stFileBasicInfo.CreationTime.QuadPart = CONF_SoftwareReg_SandBox_DeletedFileMark;

			nStatus = NtSetInformationFile(
				hFile,
				&IoSB,
				&stFileBasicInfo,
				sizeof(FILE_BASIC_INFORMATION),
				FileBasicInformation
				);

			if( nStatus == STATUS_SUCCESS )
			{
				iRet = 0;
			}
		}
	}
	NtClose(hFile);

	//
	//Free UNICODE_STRING
	//
	//RtlFreeUnicodeString(&usNtFileName);

	return iRet;
}

int NtPathExist( IN WCHAR * lpszLongNtPath )
{
	//Return Value:
	//-1 = error
	//0 = Not existed
	//1 = Existed
	int iRet = -1;

	if( lpszLongNtPath == NULL )
	{
		return iRet;
	}

	WCHAR szLongNtPath[MAX_NAME] = {NULL};
	lstrcpyW(szLongNtPath,lpszLongNtPath);

	//
	//Init UNICODE_STRING
	//
	UNICODE_STRING usNtFileName;
	RtlInitUnicodeString(&usNtFileName,szLongNtPath);

	HANDLE hFile = NULL;
	NTSTATUS nStatusQuery;
	NTSTATUS nCreateFile;
	OBJECT_ATTRIBUTES oaObjAttrib;
	IO_STATUS_BLOCK IoSB;
	FILE_BASIC_INFORMATION fbiFileAttributes;

	InitializeObjectAttributes(
		&oaObjAttrib,
		&usNtFileName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);

	nStatusQuery = NtQueryAttributesFile(&oaObjAttrib,&fbiFileAttributes);

	if( nStatusQuery != STATUS_SUCCESS )//FILE_DOES_NOT_EXIST
	{
		return iRet;
	}

	nCreateFile = NtCreateFile(
				&hFile,
				FILE_LIST_DIRECTORY | SYNCHRONIZE,
				&oaObjAttrib,
				&IoSB,
				NULL,
				FILE_ATTRIBUTE_NORMAL,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				FILE_OPEN,
				FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
				NULL,
				NULL
				);

	if( nCreateFile == STATUS_OBJECT_NAME_NOT_FOUND )//FILE_DOES_NOT_EXIST
	{
		iRet = 0;
	}
	else
	if( nCreateFile == STATUS_SUCCESS )//FILE_EXISTS
	{
		iRet = 1;
	}
	NtClose(hFile);

	//
	//Free UNICODE_STRING
	//
	//RtlFreeUnicodeString(&usNtFileName);

	return iRet;
}

int GetDirectoryMarkAsDeletedFileCount( IN WCHAR * lpszLongNtPath )
{
	int iRet = 0;

	if( lpszLongNtPath == NULL )
	{
		return iRet;
	}

	WCHAR szNtPath[MAX_NAME] = {NULL};//(\\??\\C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)
	WCHAR szPath[MAX_NAME] = {NULL};//(C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)

	lstrcpyW(szNtPath,lpszLongNtPath);
	lstrcpyW(szPath,&szNtPath[lstrlenW(L"\\??\\")]);

	WCHAR szSearchName[MAX_PATH]={0};
	wsprintf(szSearchName,L"%s\\*",szPath);

	WIN32_FIND_DATAW wFindData;
	HANDLE hFindFile = CAPI_FindFirstFileW(szSearchName,&wFindData);

	if( hFindFile != INVALID_HANDLE_VALUE )
	{
		if( wcsicmp(wFindData.cFileName,L".") != 0 &&
			wcsicmp(wFindData.cFileName,L"..") != 0
			)
		{
			WCHAR szFilePath[MAX_NAME] = {NULL};
			wsprintf(szFilePath,L"%s\\%s",szPath,wFindData.cFileName);

			//
			//Init UNICODE_STRING
			//
			UNICODE_STRING usNtFileName;
			RtlDosPathNameToNtPathName_U(szFilePath,&usNtFileName,NULL,NULL);

			NTSTATUS nStatus;
			NTSTATUS nStatusQuery;
			HANDLE hFile = NULL;
			IO_STATUS_BLOCK IoSB;
			FILE_BASIC_INFORMATION stFileBasicInfo = {NULL};
			OBJECT_ATTRIBUTES oaObjAttrib;
			FILE_BASIC_INFORMATION fbiFileAttributes;

			InitializeObjectAttributes(
				&oaObjAttrib,
				&usNtFileName,
				OBJ_CASE_INSENSITIVE,
				NULL,
				NULL);

			nStatusQuery = NtQueryAttributesFile(&oaObjAttrib,&fbiFileAttributes);

			if( nStatusQuery == STATUS_SUCCESS )//FILE_EXISTS
			{
				nStatus = NtCreateFile(
					&hFile,
					FILE_GENERIC_READ | SYNCHRONIZE,
					&oaObjAttrib,
					&IoSB,
					NULL,
					FILE_ATTRIBUTE_NORMAL,
					FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
					FILE_OPEN,
					FILE_SYNCHRONOUS_IO_NONALERT,
					NULL,
					NULL
					);
				if( nStatus != STATUS_SUCCESS )
				{
					nStatus = NtCreateFile(
						&hFile,
						FILE_GENERIC_READ | SYNCHRONIZE,
						&oaObjAttrib,
						&IoSB,
						NULL,
						FILE_ATTRIBUTE_NORMAL,
						FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
						FILE_OPEN,
						FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
						NULL,
						NULL
						);
				}

				if( nStatus == STATUS_SUCCESS )
				{
					nStatus = NtQueryInformationFile(
						hFile,
						&IoSB,
						&stFileBasicInfo,
						sizeof(FILE_BASIC_INFORMATION),
						FileBasicInformation);

					if( nStatus == STATUS_SUCCESS )
					{
						if( stFileBasicInfo.CreationTime.QuadPart == CONF_SoftwareReg_SandBox_DeletedFileMark )
						{
							iRet++;
						}
					}
				}
				NtClose(hFile);

				//
				//Free UNICODE_STRING
				//
				//RtlFreeUnicodeString(&usNtFileName);
			}//FILE_EXISTS
		}

		while( CAPI_FindNextFileW(hFindFile, &wFindData) != NULL )
		{
			if( wcsicmp(wFindData.cFileName,L".") != 0 &&
				wcsicmp(wFindData.cFileName,L"..") != 0
				)
			{
				WCHAR szFilePath[MAX_NAME] = {NULL};
				wsprintf(szFilePath,L"%s\\%s",szPath,wFindData.cFileName);

				//
				//Init UNICODE_STRING
				//
				UNICODE_STRING usNtFileName;
				RtlDosPathNameToNtPathName_U(szFilePath,&usNtFileName,NULL,NULL);

				NTSTATUS nStatus;
				NTSTATUS nStatusQuery;
				HANDLE hFile = NULL;
				IO_STATUS_BLOCK IoSB;
				FILE_BASIC_INFORMATION stFileBasicInfo = {NULL};
				OBJECT_ATTRIBUTES oaObjAttrib;
				FILE_BASIC_INFORMATION fbiFileAttributes;

				InitializeObjectAttributes(
					&oaObjAttrib,
					&usNtFileName,
					OBJ_CASE_INSENSITIVE,
					NULL,
					NULL);

				nStatusQuery = NtQueryAttributesFile(&oaObjAttrib,&fbiFileAttributes);

				if( nStatusQuery == STATUS_SUCCESS )//FILE_EXISTS
				{
					nStatus = NtCreateFile(
						&hFile,
						FILE_GENERIC_READ | SYNCHRONIZE,
						&oaObjAttrib,
						&IoSB,
						NULL,
						FILE_ATTRIBUTE_NORMAL,
						FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
						FILE_OPEN,
						FILE_SYNCHRONOUS_IO_NONALERT,
						NULL,
						NULL
						);
					if( nStatus != STATUS_SUCCESS )
					{
						nStatus = NtCreateFile(
							&hFile,
							FILE_GENERIC_READ | SYNCHRONIZE,
							&oaObjAttrib,
							&IoSB,
							NULL,
							FILE_ATTRIBUTE_NORMAL,
							FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
							FILE_OPEN,
							FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
							NULL,
							NULL
							);
					}

					if( nStatus == STATUS_SUCCESS )
					{
						nStatus = NtQueryInformationFile(
							hFile,
							&IoSB,
							&stFileBasicInfo,
							sizeof(FILE_BASIC_INFORMATION),
							FileBasicInformation);

						if( nStatus == STATUS_SUCCESS )
						{
							if( stFileBasicInfo.CreationTime.QuadPart == CONF_SoftwareReg_SandBox_DeletedFileMark )
							{
								iRet++;
							}
						}
					}
					NtClose(hFile);

					//
					//Free UNICODE_STRING
					//
					//RtlFreeUnicodeString(&usNtFileName);
				}//FILE_EXISTS
			}
		}
		CAPI_FindClose(hFindFile);
	}

	return iRet;
}

int GetFileDeletedMark( IN WCHAR * lpszRestoreNtPath, IN WCHAR * lpszFileName, IN ULONG lpuFileNameLength )
{
	//Return Value:
	//0 = Not existed
	//1 = Existed
	int iRet = 0;

	if( lpszRestoreNtPath == NULL ||
		lpszFileName == NULL
		)
	{
		return iRet;
	}

	WCHAR szRestoreNtPath[MAX_NAME] = {NULL};
	WCHAR szFileName[MAX_PATH] = {NULL};

	lstrcpyW(szFileName,lpszFileName);
	lstrcpynW(szFileName,szFileName,(lpuFileNameLength/2)+1);

	wsprintfW(szRestoreNtPath,L"%s\\%s",lpszRestoreNtPath,szFileName);

	//
	//Init UNICODE_STRING
	//
	UNICODE_STRING usNtFileName;
	RtlInitUnicodeString(&usNtFileName,szRestoreNtPath);

	NTSTATUS nStatus;
	NTSTATUS nStatusQuery;
	HANDLE hFile = NULL;
	IO_STATUS_BLOCK IoSB;
	FILE_BASIC_INFORMATION stFileBasicInfo = {NULL};
	OBJECT_ATTRIBUTES oaObjAttrib;
	FILE_BASIC_INFORMATION fbiFileAttributes;

	InitializeObjectAttributes(
		&oaObjAttrib,
		&usNtFileName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);

	nStatusQuery = NtQueryAttributesFile(&oaObjAttrib,&fbiFileAttributes);

	if( nStatusQuery != STATUS_SUCCESS )//FILE_DOES_NOT_EXIST
	{
		return iRet;;
	}

	nStatus = NtCreateFile(
		&hFile,
		FILE_GENERIC_READ | SYNCHRONIZE,
		&oaObjAttrib,
		&IoSB,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		NULL
		);
	if( nStatus != STATUS_SUCCESS )
	{
		nStatus = NtCreateFile(
			&hFile,
			FILE_GENERIC_READ | SYNCHRONIZE,
			&oaObjAttrib,
			&IoSB,
			NULL,
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			FILE_OPEN,
			FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			NULL
			);
	}

	if( nStatus == STATUS_SUCCESS )
	{
		nStatus = NtQueryInformationFile(
			hFile,
			&IoSB,
			&stFileBasicInfo,
			sizeof(FILE_BASIC_INFORMATION),
			FileBasicInformation);

		if( nStatus == STATUS_SUCCESS )
		{
			if( stFileBasicInfo.CreationTime.QuadPart == CONF_SoftwareReg_SandBox_DeletedFileMark )
			{
				iRet = 1;
			}
		}
	}
	NtClose(hFile);

	//
	//Free UNICODE_STRING
	//
	//RtlFreeUnicodeString(&usNtFileName);

	return iRet;
}

int InitDirectoryForFileDispatch( IN WCHAR * lpszRestoreNtPath )
{
	//Return Value:
	//-1 = Error
	//0 = Succeed
	int iRet = -1;

	if( lpszRestoreNtPath == NULL )
	{
		return iRet;
	}

	WCHAR szRestoreNtPath[MAX_NAME] = {NULL};//(\\??\\C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)
	WCHAR szRestorePath[MAX_NAME] = {NULL};//(C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)

	lstrcpyW(szRestoreNtPath,lpszRestoreNtPath);
	lstrcpyW(szRestorePath,&szRestoreNtPath[lstrlenW(L"\\??\\")]);

	PathRemoveFileSpec(szRestorePath);

	//
	//Init Directory
	//
	iRet = CAPI_SHCreateDirectoryExW(NULL,szRestorePath,NULL);

	return iRet;
}

int QueryDispatchedFile( IN WCHAR * lpszRestoreNtPath )
{
	//Return Value:
	//-1 = Error or MarkAsDeleted
	//0 = Not Dispatched
	//1 = Dispatched
	int iRet = 0;

	if( lpszRestoreNtPath == NULL )
	{
		return iRet;
	}

	WCHAR szRestoreNtPath[MAX_NAME] = {NULL};
	lstrcpyW(szRestoreNtPath,lpszRestoreNtPath);

	//
	//Init UNICODE_STRING
	//
	UNICODE_STRING usNtFileName;
	RtlInitUnicodeString(&usNtFileName,szRestoreNtPath);

	NTSTATUS nStatus;
	NTSTATUS nStatusQuery;
	HANDLE hFile = NULL;
	IO_STATUS_BLOCK IoSB;
	FILE_BASIC_INFORMATION stFileBasicInfo = {NULL};
	OBJECT_ATTRIBUTES oaObjAttrib;
	FILE_BASIC_INFORMATION fbiFileAttributes;

	InitializeObjectAttributes(
		&oaObjAttrib,
		&usNtFileName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);

	nStatusQuery = NtQueryAttributesFile(&oaObjAttrib,&fbiFileAttributes);

	if( nStatusQuery != STATUS_SUCCESS )//FILE_DOES_NOT_EXIST
	{
		return iRet;
	}

	nStatus = NtCreateFile(
		&hFile,
		FILE_GENERIC_READ | SYNCHRONIZE,
		&oaObjAttrib,
		&IoSB,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		NULL
		);
	if( nStatus != STATUS_SUCCESS )
	{
		nStatus = NtCreateFile(
			&hFile,
			FILE_GENERIC_READ | SYNCHRONIZE,
			&oaObjAttrib,
			&IoSB,
			NULL,
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			FILE_OPEN,
			FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			NULL
			);
	}

	if( nStatus == STATUS_SUCCESS )
	{
		nStatus = NtQueryInformationFile(
			hFile,
			&IoSB,
			&stFileBasicInfo,
			sizeof(FILE_BASIC_INFORMATION),
			FileBasicInformation);

		if( nStatus == STATUS_SUCCESS )
		{
			if( stFileBasicInfo.CreationTime.QuadPart == CONF_SoftwareReg_SandBox_DeletedFileMark )
			{
				iRet = -1;
			}
			else
			{
				iRet = 1;
			}
		}
		else
		{
			iRet = 0;
		}
	}
	NtClose(hFile);

	//
	//Free UNICODE_STRING
	//
	//RtlFreeUnicodeString(&usNtFileName);

	return iRet;
}

BOOL CloneFileToDispatchPath( IN WCHAR * lpszLongNtPath, IN WCHAR * lpszRestoreNtPath, IN ULONG lpuFileAttributes )
{
	BOOL bRet = FALSE;

	if( lpszLongNtPath == NULL ||
		lpszRestoreNtPath == NULL
		)
	{
		return bRet;
	}

	WCHAR szLongNtPath[MAX_NAME] = {NULL};//(\\??\\C:\\WINDOWS\\XXX)
	WCHAR szLongPath[MAX_NAME] = {NULL};//(C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)

	WCHAR szRestoreNtPath[MAX_NAME] = {NULL};//(\\??\\C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)
	WCHAR szRestorePath[MAX_NAME] = {NULL};//(C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)

	lstrcpyW(szLongNtPath,lpszLongNtPath);
	lstrcpyW(szLongPath,&szLongNtPath[lstrlenW(L"\\??\\")]);

	lstrcpyW(szRestoreNtPath,lpszRestoreNtPath);
	lstrcpyW(szRestorePath,&szRestoreNtPath[lstrlenW(L"\\??\\")]);

	//
	//Copy file
	//
	//bRet = ReadWriteCopyFile(szLongPath,szRestorePath);

	if( lpuFileAttributes == FILE_ATTRIBUTE_DIRECTORY )
	{
		bRet = CopyDirectory(szLongPath,szRestorePath);
	}
	else
	{
		bRet = CAPI_CopyFileW(szLongPath,szRestorePath,TRUE);
	}

	return bRet;
}

BOOL ReadWriteCopyFile( IN LPCWSTR lpExistingFileName, IN LPCWSTR lpNewFileName )
{
	if( lpExistingFileName == NULL ||
		lpNewFileName == NULL
		)
	{
		return FALSE;
	}

	HANDLE hExist = INVALID_HANDLE_VALUE;
	HANDLE hNew = INVALID_HANDLE_VALUE;

	hExist = CAPI_CreateFileW( lpExistingFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if ( hExist == INVALID_HANDLE_VALUE )
	{
		CAPI_CloseHandle( hExist );
		return FALSE;
	}

	hNew = CAPI_CreateFileW( lpNewFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if ( hNew == INVALID_HANDLE_VALUE )
	{
		CAPI_CloseHandle( hExist );
		CAPI_CloseHandle( hNew );
		return FALSE;
	}

	BYTE buffer[0x1000];
	DWORD dwRead = 0;
	DWORD dwWrite = 0;

	while ( CAPI_ReadFile( hExist, buffer, 0x1000, &dwRead, NULL) )
	{
		if ( !CAPI_WriteFile( hNew, buffer, dwRead, &dwWrite, NULL) )
		{
			CloseHandle( hNew );
			CloseHandle( hExist);
			return FALSE;
		}

		if ( dwRead < 0x1000 )
		{
			break;
		}
	}

	CAPI_CloseHandle( hNew );
	CAPI_CloseHandle( hExist);

	return TRUE;
}

int CopyDirectory( IN LPCWSTR lpExistingPathName, IN LPCWSTR lpNewPathName )
{
	if( lpExistingPathName == NULL ||
		lpNewPathName == NULL
		)
	{
		return -1;
	}

	SHFILEOPSTRUCT fo = 
	{
		NULL,
		FO_COPY,
		lpExistingPathName,
		lpNewPathName,
		FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION,
		FALSE,
		NULL,
		NULL
	};

	return SHFileOperation(&fo);
}