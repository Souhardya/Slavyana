#pragma once

#include "./Dispatch_NTDLL_NtFsControlFile.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef NTSTATUS (NTAPI * __pfnNtNotifyChangeMultipleKeys)
(
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
    );

extern __pfnNtNotifyChangeMultipleKeys pfnNtNotifyChangeMultipleKeys;

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
    );

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
    );