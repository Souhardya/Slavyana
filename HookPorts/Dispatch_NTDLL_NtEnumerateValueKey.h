#pragma once

typedef enum _KEY_VALUE_INFORMATION_CLASS {
    KeyValueBasicInformation,
    KeyValueFullInformation,
    KeyValuePartialInformation,
    KeyValueFullInformationAlign64,
    KeyValuePartialInformationAlign64
} KEY_VALUE_INFORMATION_CLASS;

typedef NTSTATUS (NTAPI * __pfnNtEnumerateValueKey)
(
	IN HANDLE  KeyHandle,
	IN ULONG  Index,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInformationClass,
	OUT PVOID  KeyValueInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
    );

extern __pfnNtEnumerateValueKey pfnNtEnumerateValueKey;

NTSTATUS
NTAPI
OnNtEnumerateValueKey(
	IN HANDLE  KeyHandle,
	IN ULONG  Index,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInformationClass,
	OUT PVOID  KeyValueInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
    );

NTSTATUS
NTAPI
NtEnumerateValueKey(
	IN HANDLE  KeyHandle,
	IN ULONG  Index,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInformationClass,
	OUT PVOID  KeyValueInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
    );