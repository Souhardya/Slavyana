#pragma once

typedef NTSTATUS (NTAPI * __pfnNtDeleteKey)
(
	IN HANDLE  KeyHandle
    );

extern __pfnNtDeleteKey pfnNtDeleteKey;

NTSTATUS
NTAPI
OnNtDeleteKey(
	IN HANDLE  KeyHandle
    );

NTSTATUS
NTAPI
NtDeleteKey(
	IN HANDLE  KeyHandle
    );