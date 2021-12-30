#include <windows.h>
#include <ntsecapi.h>
#include <winbase.h>

#include "./Dispatch_NTDLL_NtCreateFile.h"
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
#include "./Dispatch_NTDLL_NtSetInformationFile.h"



//
//Global
//
__pfnNtCreateFile pfnNtCreateFile = NULL;



//
//Dispatch_NTDLL_NtCreateFile Functions
//
NTSTATUS
NTAPI
OnNtCreateFile(
    OUT PHANDLE  FileHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN PLARGE_INTEGER  AllocationSize  OPTIONAL,
    IN ULONG  FileAttributes,
    IN ULONG  ShareAccess,
    IN ULONG  CreateDisposition,
    IN ULONG  CreateOptions,
    IN PVOID  EaBuffer  OPTIONAL,
    IN ULONG  EaLength)
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
		nRet = pfnNtCreateFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			AllocationSize,
			FileAttributes,
			ShareAccess,
			CreateDisposition,
			CreateOptions,
			EaBuffer,
			EaLength
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
		nRet = pfnNtCreateFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			AllocationSize,
			FileAttributes,
			ShareAccess,
			CreateDisposition,
			CreateOptions,
			EaBuffer,
			EaLength
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

		nRet = pfnNtCreateFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			AllocationSize,
			FileAttributes,
			ShareAccess,
			CreateDisposition,
			CreateOptions,
			EaBuffer,
			EaLength
			);

		return nRet;
	}

	//
	//Init UNICODE_STRING
	//
	RtlInitUnicodeString(&usDispatchNtPath,szDispatchNtPath);

	////OutputDebugString(L"\r\nNtCreateFile szLongNtPath=");
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

		nRet = pfnNtCreateFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			AllocationSize,
			FileAttributes,
			ShareAccess,
			CreateDisposition,
			CreateOptions,
			EaBuffer,
			EaLength
			);

		return nRet;
	}

	//
	//Check if file dispatched
	//
	HANDLE hFile = NULL;
	OBJECT_ATTRIBUTES oaObjAttrib = *(POBJECT_ATTRIBUTES)ObjectAttributes;
	IO_STATUS_BLOCK IoSB;
	NTSTATUS nCreateFile;

	int iFileDispatch = -1;
	iFileDispatch = QueryDispatchedFile(szDispatchNtPath);
	if( iFileDispatch == -1 )
	{
#ifdef Dbg
		//DebugLog(DbgInfo,L"iFileDispatch == -1! szLongNtPath=");
		//OutputDebugString(szLongNtPath);
#endif

		//
		//Check if CreateDisposition includes [FILE_SUPERSEDE]\[FILE_CREATE]\[FILE_OPEN_IF]\[FILE_OVERWRITE_IF]
		//
		if( CreateDisposition == FILE_SUPERSEDE ||
			CreateDisposition == FILE_CREATE ||
			CreateDisposition == FILE_OPEN_IF ||
			CreateDisposition == FILE_OVERWRITE_IF									
			)
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
			//DebugLog(DbgInfo,L"Deleted file Dispatch! szLongNtPath=");
			//OutputDebugString(szLongNtPath);
#endif

			nRet = pfnNtCreateFile(
				FileHandle,
				DesiredAccess,
				&oaObjAttrib,
				IoStatusBlock,
				AllocationSize,
				FileAttributes,
				ShareAccess,
				CreateDisposition,
				CreateOptions,
				EaBuffer,
				EaLength
				);

			//
			//Set new CreationTime
			//
			NTSTATUS nStatus;
			FILE_BASIC_INFORMATION stFileBasicInfo = {NULL};

			if( nRet == STATUS_SUCCESS )
			{
				nStatus = NtQueryInformationFile(
					*(PHANDLE)FileHandle,
					&IoSB,
					&stFileBasicInfo,
					sizeof(FILE_BASIC_INFORMATION),
					FileBasicInformation);

				if( nStatus == STATUS_SUCCESS )
				{
					stFileBasicInfo.CreationTime.QuadPart = stFileBasicInfo.LastAccessTime.QuadPart;

					nStatus = NtSetInformationFile(
						*(PHANDLE)FileHandle,
						&IoSB,
						&stFileBasicInfo,
						sizeof(FILE_BASIC_INFORMATION),
						FileBasicInformation
						);
				}
			}

			return nRet;
		}

#ifdef Dbg
		//DebugLog(DbgInfo,L"Deleted file FILE_DOES_NOT_EXIST! szLongNtPath=");
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

		nRet = pfnNtCreateFile(
			FileHandle,
			DesiredAccess,
			&oaObjAttrib,
			IoStatusBlock,
			AllocationSize,
			FileAttributes,
			ShareAccess,
			CreateDisposition,
			CreateOptions,
			EaBuffer,
			EaLength
			);

		return nRet;
	}

	//
	//Get FileName
	//
	if( nStatusQuery == STATUS_OBJECT_NAME_NOT_FOUND )//FILE_DOES_NOT_EXIST
	{
		nCreateFile = NtCreateFile(
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
		if( nCreateFile == STATUS_SUCCESS &&
			IoSB.Information == FILE_CREATED
			)//Get FileName succeed
		{
			NtClose(hFile);
			NtDeleteFile(&oaObjAttrib);

#ifdef Dbg
			//DebugLog(DbgInfo,L"Get FileName succeed! szLongNtPath=");
			//OutputDebugString(szLongNtPath);
#endif

			//
			//Check if CreateDisposition includes [FILE_SUPERSEDE]\[FILE_CREATE]\[FILE_OPEN_IF]\[FILE_OVERWRITE_IF]
			//
			if( CreateDisposition == FILE_SUPERSEDE ||
				CreateDisposition == FILE_CREATE ||
				CreateDisposition == FILE_OPEN_IF ||
				CreateDisposition == FILE_OVERWRITE_IF									
				)
			{
				//
				//Init Directory
				//
				InitDirectoryForFileDispatch(szDispatchNtPath);

				//
				//The file is a directory
				//
				if( CreateOptions & FILE_DIRECTORY_FILE )
				{
					nRet = pfnNtCreateFile(
						FileHandle,
						DesiredAccess,
						ObjectAttributes,
						IoStatusBlock,
						AllocationSize,
						FileAttributes,
						ShareAccess,
						CreateDisposition,
						CreateOptions,
						EaBuffer,
						EaLength
						);

					//return nRet;
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

#ifdef Dbg
				//DebugLog(DbgInfo,L"File create dispatch! szLongNtPath=");
				//OutputDebugString(szLongNtPath);
				//OutputDebugString(szDispatchNtPath);
#endif

				nRet = pfnNtCreateFile(
					FileHandle,
					DesiredAccess,
					&oaObjAttrib,
					IoStatusBlock,
					AllocationSize,
					FileAttributes,
					ShareAccess,
					CreateDisposition,
					CreateOptions,
					EaBuffer,
					EaLength
					);

				if( CreateOptions & FILE_DELETE_ON_CLOSE )
				{
					//DebugLog(DbgInfo,L"FUCK! FILE_DELETE_ON_CLOSE!!!");
				}

#ifdef Dbg
				WCHAR szStatusCode[128] = {0};
				wsprintf(szStatusCode,L"\r\n nRet = [0x%08X]",nRet);
				//DebugLog(DbgInfo,szStatusCode);
#endif

				return nRet;
			}

#ifdef Dbg
			//DebugLog(DbgInfo,L"CreateDisposition does not includes CREATE! szLongNtPath=");
			//OutputDebugString(szLongNtPath);
#endif
		}
		else//Get FileName failed
		{
#ifdef Dbg
			//DebugLog(DbgInfo,L"Get FileName failed! szLongNtPath=");
			//OutputDebugString(szLongNtPath);
#endif
		}
		NtClose(hFile);
	}
	else
	if( nStatusQuery == STATUS_SUCCESS )//FILE_EXISTS
	{
#ifdef Dbg
		//DebugLog(DbgInfo,L"nStatusQuery == STATUS_SUCCESS! szLongNtPath=");
		//OutputDebugString(szLongNtPath);
#endif
		//
		//Check if the file is a directory
		//Check if DesiredAccess includes [FILE_WRITE_DATA]\[FILE_WRITE_ATTRIBUTES]\[FILE_WRITE_EA]\[FILE_APPEND_DATA]
		//
		if( (CreateOptions & FILE_DIRECTORY_FILE ) == 0 &&
			CreateDisposition == FILE_OVERWRITE_IF
			)
		{
			//
			//Init Directory
			//
			InitDirectoryForFileDispatch(szDispatchNtPath);

			//
			//Copy file
			//
			CloneFileToDispatchPath(szLongNtPath,szDispatchNtPath,FILE_ATTRIBUTE_ARCHIVE);

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
			//DebugLog(DbgInfo,L"File write dispatch! szLongNtPath=");
			//OutputDebugString(szLongNtPath);
#endif

			nRet = pfnNtCreateFile(
				FileHandle,
				DesiredAccess,
				&oaObjAttrib,
				IoStatusBlock,
				AllocationSize,
				FileAttributes,
				ShareAccess,
				CreateDisposition,
				CreateOptions,
				EaBuffer,
				EaLength
				);

			return nRet;
		}

#ifdef Dbg
		//DebugLog(DbgInfo,L"Not includes FILE_OVERWRITE_IF! szLongNtPath=");
		//OutputDebugString(szLongNtPath);
#endif
	}

	nRet = pfnNtCreateFile(
		FileHandle,
		DesiredAccess,
		ObjectAttributes,
		IoStatusBlock,
		AllocationSize,
		FileAttributes,
		ShareAccess,
		CreateDisposition,
		CreateOptions,
		EaBuffer,
		EaLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtCreateFile(
    OUT PHANDLE  FileHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN PLARGE_INTEGER  AllocationSize  OPTIONAL,
    IN ULONG  FileAttributes,
    IN ULONG  ShareAccess,
    IN ULONG  CreateDisposition,
    IN ULONG  CreateOptions,
    IN PVOID  EaBuffer  OPTIONAL,
    IN ULONG  EaLength)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtCreateFile addFun = (__pfnNtCreateFile)GetProcAddress(hDll,"NtCreateFile");
	if(addFun)
	{
		ret = addFun(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,AllocationSize,FileAttributes,ShareAccess,CreateDisposition,CreateOptions,EaBuffer,EaLength);
	}
	return ret;
}