#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtFsControlFile.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtFsControlFile pfnNtFsControlFile = NULL;



//
//Dispatch_NTDLL_NtFsControlFile Functions
//
NTSTATUS
NTAPI
OnNtFsControlFile(
    IN HANDLE  FileHandle,
    IN HANDLE  Event OPTIONAL,
    IN PIO_APC_ROUTINE  ApcRoutine OPTIONAL,
    IN PVOID  ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN ULONG  FsControlCode,
    IN PVOID  InputBuffer OPTIONAL,
    IN ULONG  InputBufferLength,
    OUT PVOID  OutputBuffer OPTIONAL,
    IN ULONG  OutputBufferLength
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
		nRet = pfnNtFsControlFile(
			FileHandle,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			FsControlCode,
			InputBuffer,
			InputBufferLength,
			OutputBuffer,
			OutputBufferLength
			);

		return nRet;
	}

	nRet = pfnNtFsControlFile(
		FileHandle,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		FsControlCode,
		InputBuffer,
		InputBufferLength,
		OutputBuffer,
		OutputBufferLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtFsControlFile(
    IN HANDLE  FileHandle,
    IN HANDLE  Event OPTIONAL,
    IN PIO_APC_ROUTINE  ApcRoutine OPTIONAL,
    IN PVOID  ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN ULONG  FsControlCode,
    IN PVOID  InputBuffer OPTIONAL,
    IN ULONG  InputBufferLength,
    OUT PVOID  OutputBuffer OPTIONAL,
    IN ULONG  OutputBufferLength
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtFsControlFile addFun = (__pfnNtFsControlFile)GetProcAddress(hDll,"NtFsControlFile");
	if(addFun)
	{
		ret = addFun(FileHandle,Event,ApcRoutine,ApcContext,IoStatusBlock,FsControlCode,InputBuffer,InputBufferLength,OutputBuffer,OutputBufferLength);
	}
	return ret;
}