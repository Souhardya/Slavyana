#pragma once

#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef struct _FILE_NETWORK_OPEN_INFORMATION {
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
} FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION;

typedef NTSTATUS (NTAPI * __pfnNtQueryFullAttributesFile)
(
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PFILE_NETWORK_OPEN_INFORMATION  FileInformation);

extern __pfnNtQueryFullAttributesFile pfnNtQueryFullAttributesFile;

NTSTATUS
NTAPI
OnNtQueryFullAttributesFile(
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PFILE_NETWORK_OPEN_INFORMATION  FileInformation);

NTSTATUS
NTAPI
NtQueryFullAttributesFile(
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PFILE_NETWORK_OPEN_INFORMATION  FileInformation);