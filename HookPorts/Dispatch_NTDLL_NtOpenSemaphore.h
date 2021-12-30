#pragma once

#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef NTSTATUS (NTAPI * __pfnNtOpenSemaphore)
(
	OUT PHANDLE             SemaphoreHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes);

extern __pfnNtOpenSemaphore pfnNtOpenSemaphore;

NTSTATUS
NTAPI
OnNtOpenSemaphore(
	OUT PHANDLE             SemaphoreHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes);

NTSTATUS
NTAPI
NtOpenSemaphore(
	OUT PHANDLE             SemaphoreHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes);