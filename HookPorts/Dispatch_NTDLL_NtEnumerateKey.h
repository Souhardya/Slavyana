#pragma once

typedef enum _KEY_INFORMATION_CLASS {
    KeyBasicInformation,
    KeyNodeInformation,
    KeyFullInformation,
    KeyNameInformation
} KEY_INFORMATION_CLASS;

typedef NTSTATUS (NTAPI * __pfnNtEnumerateKey)
(
	IN HANDLE  KeyHandle,
	IN ULONG  Index,
	IN KEY_INFORMATION_CLASS  KeyInformationClass,
	OUT PVOID  KeyInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
    );

extern __pfnNtEnumerateKey pfnNtEnumerateKey;

NTSTATUS
NTAPI
OnNtEnumerateKey(
	IN HANDLE  KeyHandle,
	IN ULONG  Index,
	IN KEY_INFORMATION_CLASS  KeyInformationClass,
	OUT PVOID  KeyInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
    );

NTSTATUS
NTAPI
NtEnumerateKey(
	IN HANDLE  KeyHandle,
	IN ULONG  Index,
	IN KEY_INFORMATION_CLASS  KeyInformationClass,
	OUT PVOID  KeyInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
    );