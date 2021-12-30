#pragma once

NTSTATUS
NTAPI
RtlInitUnicodeString(
	PUNICODE_STRING DestinationString,
	PCWSTR SourceString
	);

NTSTATUS
NTAPI
RtlFreeUnicodeString(
	IN PUNICODE_STRING UnicodeString
	);

typedef struct _CURDIR
{
	UNICODE_STRING DosPath;
	HANDLE Handle;
} CURDIR;

NTSTATUS
NTAPI
RtlDosPathNameToNtPathName_U(
	IN PCWSTR DosPathName,
	OUT PUNICODE_STRING NtPathName,
	OUT PCWSTR *NtFileNamePart,
	OUT CURDIR *DirectoryInfo
	);