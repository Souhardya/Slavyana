#pragma once

#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"
#include "./Dispatch_NTDLL_NtCreateFile.h"

typedef NTSTATUS (NTAPI * __pfnNtOpenFile)
(
	OUT PHANDLE  FileHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PIO_STATUS_BLOCK  IoStatusBlock,
	IN ULONG  ShareAccess,
	IN ULONG  OpenOptions);

extern __pfnNtOpenFile pfnNtOpenFile;

NTSTATUS
NTAPI
OnNtOpenFile(
	OUT PHANDLE  FileHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PIO_STATUS_BLOCK  IoStatusBlock,
	IN ULONG  ShareAccess,
	IN ULONG  OpenOptions);

NTSTATUS
NTAPI
NtOpenFile(
	OUT PHANDLE  FileHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PIO_STATUS_BLOCK  IoStatusBlock,
	IN ULONG  ShareAccess,
	IN ULONG  OpenOptions);