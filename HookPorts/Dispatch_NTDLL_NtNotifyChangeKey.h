#pragma once

#include "./Dispatch_NTDLL_NtFsControlFile.h"

typedef NTSTATUS (NTAPI * __pfnNtNotifyChangeKey)
(
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
    );

extern __pfnNtNotifyChangeKey pfnNtNotifyChangeKey;

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
    );

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
    );