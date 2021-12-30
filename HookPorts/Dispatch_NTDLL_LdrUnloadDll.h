#pragma once

typedef NTSTATUS (NTAPI * __pfnLdrUnloadDll)
(  IN HANDLE               ModuleHandle );

extern __pfnLdrUnloadDll pfnLdrUnloadDll;

NTSTATUS
NTAPI
OnLdrUnloadDll(
	IN HANDLE               ModuleHandle );