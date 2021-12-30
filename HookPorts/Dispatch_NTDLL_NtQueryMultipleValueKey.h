#pragma once

#include "./Dispatch_NTDLL_NtEnumerateKey.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef struct _KEY_MULTIPLE_VALUE_INFORMATION {
	PUNICODE_STRING         ValueName;
	ULONG                   DataLength;
	ULONG                   DataOffset;
	ULONG                   Type;
} KEY_MULTIPLE_VALUE_INFORMATION, *PKEY_MULTIPLE_VALUE_INFORMATION;

typedef NTSTATUS (NTAPI * __pfnNtQueryMultipleValueKey)
(
	IN HANDLE               KeyHandle,
	IN OUT PKEY_MULTIPLE_VALUE_INFORMATION ValuesList,
	IN ULONG                NumberOfValues,
	OUT PVOID               DataBuffer,
	IN OUT ULONG            BufferLength,
	OUT PULONG              RequiredLength OPTIONAL
	);

extern __pfnNtQueryMultipleValueKey pfnNtQueryMultipleValueKey;

NTSTATUS
NTAPI
OnNtQueryMultipleValueKey(
	IN HANDLE               KeyHandle,
	IN OUT PKEY_MULTIPLE_VALUE_INFORMATION ValuesList,
	IN ULONG                NumberOfValues,
	OUT PVOID               DataBuffer,
	IN OUT ULONG            BufferLength,
	OUT PULONG              RequiredLength OPTIONAL
	);

NTSTATUS
NTAPI
NtQueryMultipleValueKey(
	IN HANDLE               KeyHandle,
	IN OUT PKEY_MULTIPLE_VALUE_INFORMATION ValuesList,
	IN ULONG                NumberOfValues,
	OUT PVOID               DataBuffer,
	IN OUT ULONG            BufferLength,
	OUT PULONG              RequiredLength OPTIONAL
	);