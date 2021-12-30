#pragma once

typedef NTSTATUS (NTAPI * __pfnNtSetValueKey)
(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN ULONG  TitleIndex  OPTIONAL,
	IN ULONG  Type,
	IN PVOID  Data,
	IN ULONG  DataSize
	);

extern __pfnNtSetValueKey pfnNtSetValueKey;

NTSTATUS
NTAPI
OnNtSetValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN ULONG  TitleIndex  OPTIONAL,
	IN ULONG  Type,
	IN PVOID  Data,
	IN ULONG  DataSize
	);

NTSTATUS
NTAPI
NtSetValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN ULONG  TitleIndex  OPTIONAL,
	IN ULONG  Type,
	IN PVOID  Data,
	IN ULONG  DataSize
	);