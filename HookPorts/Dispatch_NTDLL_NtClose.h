#pragma once

typedef NTSTATUS (NTAPI * __pfnNtClose)
(
    IN HANDLE  Handle
    );

extern __pfnNtClose pfnNtClose;

NTSTATUS
NTAPI
OnNtClose(
    IN HANDLE  Handle
    );

NTSTATUS
NTAPI
NtClose(
    IN HANDLE  Handle
    );