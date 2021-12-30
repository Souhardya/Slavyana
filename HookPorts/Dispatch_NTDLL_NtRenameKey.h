#pragma once

#include "./Dispatch_NTDLL_NtEnumerateKey.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef NTSTATUS (NTAPI * __pfnNtRenameKey)
(
	IN HANDLE			KeyHandle,
	IN PUNICODE_STRING	NewName
	);

extern __pfnNtRenameKey pfnNtRenameKey;

NTSTATUS
NTAPI
OnNtRenameKey(
	IN HANDLE			KeyHandle,
	IN PUNICODE_STRING	NewName
	);

NTSTATUS
NTAPI
NtRenameKey(
	IN HANDLE			KeyHandle,
	IN PUNICODE_STRING	NewName
	);