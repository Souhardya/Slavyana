#pragma once

#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtCreateProcess.h"

typedef NTSTATUS (NTAPI * __pfnNtDeleteFile)
(
    IN POBJECT_ATTRIBUTES   ObjectAttributes);

extern __pfnNtDeleteFile pfnNtDeleteFile;

NTSTATUS
NTAPI
OnNtDeleteFile(
    IN POBJECT_ATTRIBUTES   ObjectAttributes);

NTSTATUS
NTAPI
NtDeleteFile(
    IN POBJECT_ATTRIBUTES   ObjectAttributes);