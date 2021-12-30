#pragma once

#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef NTSTATUS (NTAPI * __pfnNtOpenKey)
(
	OUT PHANDLE  KeyHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes
    );

extern __pfnNtOpenKey pfnNtOpenKey;

NTSTATUS
NTAPI
OnNtOpenKey(
	OUT PHANDLE  KeyHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes
    );

NTSTATUS
NTAPI
NtOpenKey(
	OUT PHANDLE  KeyHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes
    );