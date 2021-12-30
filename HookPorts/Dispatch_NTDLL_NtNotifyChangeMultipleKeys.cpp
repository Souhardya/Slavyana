#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtNotifyChangeMultipleKeys.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtNotifyChangeMultipleKeys pfnNtNotifyChangeMultipleKeys = NULL;



//
//Dispatch_NTDLL_NtNotifyChangeMultipleKeys Functions
//
NTSTATUS
NTAPI
OnNtNotifyChangeMultipleKeys(
	IN HANDLE               MasterKeyHandle,
	IN ULONG			    Count,
	IN OBJECT_ATTRIBUTES    SubordinateObjects,
	IN HANDLE               Event,
	IN PIO_APC_ROUTINE		ApcRoutine,
	IN PVOID				ApcContext,
	OUT PIO_STATUS_BLOCK	IoStatusBlock,
	IN ULONG				CompletionFilter,
	IN BOOLEAN				WatchTree,
	OUT PVOID				Buffer,
	IN ULONG				BufferSize,
	IN BOOLEAN				Asynchronous
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
		nRet = pfnNtNotifyChangeMultipleKeys(
			MasterKeyHandle,
			Count,
			SubordinateObjects,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			CompletionFilter,
			WatchTree,
			Buffer,
			BufferSize,
			Asynchronous
			);

		return nRet;
	}

	nRet = pfnNtNotifyChangeMultipleKeys(
		MasterKeyHandle,
		Count,
		SubordinateObjects,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		CompletionFilter,
		WatchTree,
		Buffer,
		BufferSize,
		Asynchronous
		);

	return nRet;
}

NTSTATUS
NTAPI
NtNotifyChangeMultipleKeys(
	IN HANDLE               MasterKeyHandle,
	IN ULONG			    Count,
	IN OBJECT_ATTRIBUTES    SubordinateObjects,
	IN HANDLE               Event,
	IN PIO_APC_ROUTINE		ApcRoutine,
	IN PVOID				ApcContext,
	OUT PIO_STATUS_BLOCK	IoStatusBlock,
	IN ULONG				CompletionFilter,
	IN BOOLEAN				WatchTree,
	OUT PVOID				Buffer,
	IN ULONG				BufferSize,
	IN BOOLEAN				Asynchronous
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtNotifyChangeMultipleKeys addFun = (__pfnNtNotifyChangeMultipleKeys)GetProcAddress(hDll,"NtNotifyChangeMultipleKeys");
	if(addFun)
	{
		ret = addFun(MasterKeyHandle,Count,SubordinateObjects,Event,ApcRoutine,ApcContext,IoStatusBlock,CompletionFilter,WatchTree,Buffer,BufferSize,Asynchronous);
	}
	return ret;
}