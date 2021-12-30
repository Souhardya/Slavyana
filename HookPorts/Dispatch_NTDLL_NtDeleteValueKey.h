#pragma once

typedef NTSTATUS (NTAPI * __pfnNtDeleteValueKey)
(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName
    );

extern __pfnNtDeleteValueKey pfnNtDeleteValueKey;

NTSTATUS
NTAPI
OnNtDeleteValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName
    );

NTSTATUS
NTAPI
NtDeleteValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName
    );