#pragma once

#include "./Dispatch_NTDLL_NtResumeThread.h"

typedef NTSTATUS (NTAPI * __pfnNtLoadDriver)
(
	IN PUNICODE_STRING DriverServiceName );

extern __pfnNtLoadDriver pfnNtLoadDriver;

NTSTATUS
NTAPI
OnNtLoadDriver(
	IN PUNICODE_STRING DriverServiceName );

NTSTATUS
NTAPI
NtLoadDriver(
	IN PUNICODE_STRING DriverServiceName );