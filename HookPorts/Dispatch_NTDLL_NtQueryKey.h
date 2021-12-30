#pragma once

#include "./Dispatch_NTDLL_NtEnumerateKey.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef NTSTATUS (NTAPI * __pfnNtQueryKey)
(
	IN HANDLE  KeyHandle,
	IN KEY_INFORMATION_CLASS  KeyInformationClass,
	OUT PVOID  KeyInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
	);

extern __pfnNtQueryKey pfnNtQueryKey;

NTSTATUS
NTAPI
OnNtQueryKey(
	IN HANDLE  KeyHandle,
	IN KEY_INFORMATION_CLASS  KeyInformationClass,
	OUT PVOID  KeyInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
	);

NTSTATUS
NTAPI
NtQueryKey(
	IN HANDLE  KeyHandle,
	IN KEY_INFORMATION_CLASS  KeyInformationClass,
	OUT PVOID  KeyInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
	);