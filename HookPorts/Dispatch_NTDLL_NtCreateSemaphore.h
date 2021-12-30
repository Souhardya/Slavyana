#pragma once

#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef NTSTATUS (NTAPI * __pfnNtCreateSemaphore)
(
	OUT PHANDLE             SemaphoreHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN ULONG                InitialCount,
	IN ULONG                MaximumCount);

extern __pfnNtCreateSemaphore pfnNtCreateSemaphore;

NTSTATUS
NTAPI
OnNtCreateSemaphore(
	OUT PHANDLE             SemaphoreHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN ULONG                InitialCount,
	IN ULONG                MaximumCount);

NTSTATUS
NTAPI
NtCreateSemaphore(
	OUT PHANDLE             SemaphoreHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN ULONG                InitialCount,
	IN ULONG                MaximumCount);