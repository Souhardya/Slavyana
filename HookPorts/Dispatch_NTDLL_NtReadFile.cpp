#include <windows.h>
#include <ntsecapi.h>
#include <winbase.h>

#include "./Dispatch_NTDLL_NtReadFile.h"
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
__pfnNtReadFile pfnNtReadFile = NULL;



//
//Dispatch_NTDLL_NtReadFile Functions
//
NTSTATUS
NTAPI
OnNtReadFile(
    IN HANDLE  FileHandle,
    IN HANDLE  Event  OPTIONAL,
    IN PIO_APC_ROUTINE  ApcRoutine  OPTIONAL,
    IN PVOID  ApcContext  OPTIONAL,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    OUT PVOID  Buffer,
    IN ULONG  Length,
    IN PLARGE_INTEGER  ByteOffset  OPTIONAL,
    IN PULONG  Key  OPTIONAL)
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
		nRet = pfnNtReadFile(
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
		nRet = pfnNtReadFile(
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

	//OutputDebugString(L"\r\nNtReadFile szLongNtPath=");
	//OutputDebugString(szLongNtPath);

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
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			FILE_OPEN,
			FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			NULL
			);

		if( nStatus == STATUS_SUCCESS )
		{
			nRet = pfnNtReadFile(
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

	nRet = pfnNtReadFile(
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

NTSTATUS
NTAPI
NtReadFile(
    IN HANDLE  FileHandle,
    IN HANDLE  Event  OPTIONAL,
    IN PIO_APC_ROUTINE  ApcRoutine  OPTIONAL,
    IN PVOID  ApcContext  OPTIONAL,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    OUT PVOID  Buffer,
    IN ULONG  Length,
    IN PLARGE_INTEGER  ByteOffset  OPTIONAL,
    IN PULONG  Key  OPTIONAL)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtReadFile addFun = (__pfnNtReadFile)GetProcAddress(hDll,"NtReadFile");
	if(addFun)
	{
		ret = addFun(FileHandle,Event,ApcRoutine,ApcContext,IoStatusBlock,Buffer,Length,ByteOffset,Key);
	}
	return ret;
}