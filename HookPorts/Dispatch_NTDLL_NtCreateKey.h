#pragma once

#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef NTSTATUS (NTAPI * __pfnNtCreateKey)
(
	OUT PHANDLE  KeyHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	IN ULONG  TitleIndex,
	IN PUNICODE_STRING  Class  OPTIONAL,
	IN ULONG  CreateOptions,
	OUT PULONG  Disposition  OPTIONAL
    );

extern __pfnNtCreateKey pfnNtCreateKey;

NTSTATUS
NTAPI
OnNtCreateKey(
	OUT PHANDLE  KeyHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	IN ULONG  TitleIndex,
	IN PUNICODE_STRING  Class  OPTIONAL,
	IN ULONG  CreateOptions,
	OUT PULONG  Disposition  OPTIONAL
    );

NTSTATUS
NTAPI
NtCreateKey(
	OUT PHANDLE  KeyHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	IN ULONG  TitleIndex,
	IN PUNICODE_STRING  Class  OPTIONAL,
	IN ULONG  CreateOptions,
	OUT PULONG  Disposition  OPTIONAL
    );