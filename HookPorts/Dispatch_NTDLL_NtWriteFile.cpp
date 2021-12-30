#include <windows.h>
#include <ntsecapi.h>
#include <winbase.h>

#include "./Dispatch_NTDLL_NtWriteFile.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./HookHelp_File.h"
#include "./GetProcAddressEx.h"
#include "../Config/Config.h"
#include "./Main.h"
#include "./NativeAPI_NTDLL.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtCreateFile.h"
#include "./Dispatch_NTDLL_NtClose.h"



//
//Global
//
__pfnNtWriteFile pfnNtWriteFile = NULL;



//
//Dispatch_NTDLL_NtWriteFile Functions
//
NTSTATUS
NTAPI
OnNtWriteFile(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	IN PVOID                Buffer,
	IN ULONG                Length,
	IN PLARGE_INTEGER       ByteOffset OPTIONAL,
	IN PULONG               Key OPTIONAL)
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
		nRet = pfnNtWriteFile(
			FileHandle,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			Buffer,
			Length,
			ByteOffset,
			Key
			);

		return nRet;
	}

	//(\\??\\C:\\WINDOWS\\XXX)
	WCHAR szLongNtPath[MAX_NAME] = {NULL};
	//(\\??\\C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)
	WCHAR szDispatchNtPath[MAX_NAME] = {NULL};

	if( GetLongNtPathName(FileHandle,NULL,NULL,g_szRestoreNtPath,szLongNtPath,szDispatchNtPath) != 0 )
	{
		nRet = pfnNtWriteFile(
			FileHandle,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			Buffer,
			Length,
			ByteOffset,
			Key
			);

		return nRet;
	}

	//OutputDebugString(L"\r\nNtWriteFile szLongNtPath=");
	//OutputDebugString(szLongNtPath);

	//
	//Check if request in SandBox
	//
	if( wcsnicmp(szLongNtPath,g_szRestoreNtPath,lstrlenW(g_szRestoreNtPath)) == 0 )
	{
		nRet = pfnNtWriteFile(
			FileHandle,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			Buffer,
			Length,
			ByteOffset,
			Key
			);

		return nRet;
	}

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
	if( iFileDispatch == 1 )
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
			FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			FILE_OPEN,
			FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			NULL
			);

		if( nStatus == STATUS_SUCCESS )
		{
			nRet = pfnNtWriteFile(
				hFile,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
		}

		NtClose(hFile);

		return nRet;
	}

	//
	//Init Directory
	//
	InitDirectoryForFileDispatch(szDispatchNtPath);

	//
	//Copy file
	//
	CloneFileToDispatchPath(szLongNtPath,szDispatchNtPath,FILE_ATTRIBUTE_ARCHIVE);

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
		FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		NULL
		);

	if( nStatus == STATUS_SUCCESS )
	{
		nRet = pfnNtWriteFile(
			hFile,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			Buffer,
			Length,
			ByteOffset,
			Key
			);
	}

	NtClose(hFile);

	return nRet;
}

NTSTATUS
NTAPI
NtWriteFile(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	IN PVOID                Buffer,
	IN ULONG                Length,
	IN PLARGE_INTEGER       ByteOffset OPTIONAL,
	IN PULONG               Key OPTIONAL)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtWriteFile addFun = (__pfnNtWriteFile)GetProcAddress(hDll,"NtWriteFile");
	if(addFun)
	{
		ret = addFun(FileHandle,Event,ApcRoutine,ApcContext,IoStatusBlock,Buffer,Length,ByteOffset,Key);
	}
	return ret;
}