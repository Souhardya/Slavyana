#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtNotifyChangeKey.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtNotifyChangeKey pfnNtNotifyChangeKey = NULL;



//
//Dispatch_NTDLL_NtNotifyChangeKey Functions
//
NTSTATUS
NTAPI
OnNtNotifyChangeKey(
	IN HANDLE               KeyHandle,
	IN HANDLE               EventHandle,
	IN PIO_APC_ROUTINE      ApcRoutine,
	IN PVOID                ApcRoutineContext,
	IN PIO_STATUS_BLOCK     IoStatusBlock,
	IN ULONG                NotifyFilter,
	IN BOOLEAN              WatchSubtree,
	OUT PVOID               RegChangesDataBuffer,
	IN ULONG                RegChangesDataBufferLength,
	IN BOOLEAN              Asynchronous
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
		nRet = pfnNtNotifyChangeKey(
			KeyHandle,
			EventHandle,
			ApcRoutine,
			ApcRoutineContext,
			IoStatusBlock,
			NotifyFilter,
			WatchSubtree,
			RegChangesDataBuffer,
			RegChangesDataBufferLength,
			Asynchronous
			);

		return nRet;
	}

	nRet = pfnNtNotifyChangeKey(
		KeyHandle,
		EventHandle,
		ApcRoutine,
		ApcRoutineContext,
		IoStatusBlock,
		NotifyFilter,
		WatchSubtree,
		RegChangesDataBuffer,
		RegChangesDataBufferLength,
		Asynchronous
		);

	return nRet;
}

NTSTATUS
NTAPI
NtNotifyChangeKey(
	IN HANDLE               KeyHandle,
	IN HANDLE               EventHandle,
	IN PIO_APC_ROUTINE      ApcRoutine,
	IN PVOID                ApcRoutineContext,
	IN PIO_STATUS_BLOCK     IoStatusBlock,
	IN ULONG                NotifyFilter,
	IN BOOLEAN              WatchSubtree,
	OUT PVOID               RegChangesDataBuffer,
	IN ULONG                RegChangesDataBufferLength,
	IN BOOLEAN              Asynchronous
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtNotifyChangeKey addFun = (__pfnNtNotifyChangeKey)GetProcAddress(hDll,"NtNotifyChangeKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,EventHandle,ApcRoutine,ApcRoutineContext,IoStatusBlock,NotifyFilter,WatchSubtree,RegChangesDataBuffer,RegChangesDataBufferLength,Asynchronous);
	}
	return ret;
}