#pragma once

#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"

typedef NTSTATUS (NTAPI * __pfnNtQueryAttributesFile)
(
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	OUT PFILE_BASIC_INFORMATION FileAttributes);

extern __pfnNtQueryAttributesFile pfnNtQueryAttributesFile;

NTSTATUS
NTAPI
OnNtQueryAttributesFile(
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	OUT PFILE_BASIC_INFORMATION FileAttributes);

NTSTATUS
NTAPI
NtQueryAttributesFile(
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	OUT PFILE_BASIC_INFORMATION FileAttributes);