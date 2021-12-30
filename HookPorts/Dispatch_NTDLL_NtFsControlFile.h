#pragma once

#include "./Dispatch_NTDLL_NtQueryInformationFile.h"

//
// Define an Asynchronous Procedure Call from I/O viewpoint
//

typedef
VOID
(NTAPI *PIO_APC_ROUTINE) (
    IN PVOID ApcContext,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG Reserved
    );
#define PIO_APC_ROUTINE_DEFINED

typedef NTSTATUS (NTAPI * __pfnNtFsControlFile)
(
    IN HANDLE  FileHandle,
    IN HANDLE  Event OPTIONAL,
    IN PIO_APC_ROUTINE  ApcRoutine OPTIONAL,
    IN PVOID  ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN ULONG  FsControlCode,
    IN PVOID  InputBuffer OPTIONAL,
    IN ULONG  InputBufferLength,
    OUT PVOID  OutputBuffer OPTIONAL,
    IN ULONG  OutputBufferLength
    );

extern __pfnNtFsControlFile pfnNtFsControlFile;

NTSTATUS
NTAPI
OnNtFsControlFile(
    IN HANDLE  FileHandle,
    IN HANDLE  Event OPTIONAL,
    IN PIO_APC_ROUTINE  ApcRoutine OPTIONAL,
    IN PVOID  ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN ULONG  FsControlCode,
    IN PVOID  InputBuffer OPTIONAL,
    IN ULONG  InputBufferLength,
    OUT PVOID  OutputBuffer OPTIONAL,
    IN ULONG  OutputBufferLength
    );

NTSTATUS
NTAPI
NtFsControlFile(
    IN HANDLE  FileHandle,
    IN HANDLE  Event OPTIONAL,
    IN PIO_APC_ROUTINE  ApcRoutine OPTIONAL,
    IN PVOID  ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN ULONG  FsControlCode,
    IN PVOID  InputBuffer OPTIONAL,
    IN ULONG  InputBufferLength,
    OUT PVOID  OutputBuffer OPTIONAL,
    IN ULONG  OutputBufferLength
    );