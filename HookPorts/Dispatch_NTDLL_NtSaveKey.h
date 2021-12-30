#pragma once

typedef NTSTATUS (NTAPI * __pfnNtSaveKey)
(
	IN HANDLE               KeyHandle,
	IN HANDLE               FileHandle
	);

extern __pfnNtSaveKey pfnNtSaveKey;

NTSTATUS
NTAPI
OnNtSaveKey(
	IN HANDLE               KeyHandle,
	IN HANDLE               FileHandle
	);

NTSTATUS
NTAPI
NtSaveKey(
	IN HANDLE               KeyHandle,
	IN HANDLE               FileHandle
	);