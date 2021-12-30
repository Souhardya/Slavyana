#pragma once

#include "./Dispatch_NTDLL_NtEnumerateKey.h"
#include "./Dispatch_NTDLL_NtEnumerateValueKey.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef NTSTATUS (NTAPI * __pfnNtQueryValueKey)
(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInformationClass,
	OUT PVOID  KeyValueInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
	);

extern __pfnNtQueryValueKey pfnNtQueryValueKey;

NTSTATUS
NTAPI
OnNtQueryValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInformationClass,
	OUT PVOID  KeyValueInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
	);

NTSTATUS
NTAPI
NtQueryValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInformationClass,
	OUT PVOID  KeyValueInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
	);