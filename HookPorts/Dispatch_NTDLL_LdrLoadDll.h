#pragma once

typedef NTSTATUS (NTAPI * __pfnLdrLoadDll)
(
	IN PWCHAR               PathToFile OPTIONAL,
	IN ULONG                Flags OPTIONAL,
	IN PUNICODE_STRING      ModuleFileName,
	OUT PHANDLE             ModuleHandle );

extern __pfnLdrLoadDll pfnLdrLoadDll;

NTSTATUS
NTAPI
OnLdrLoadDll(
	IN PWCHAR               PathToFile OPTIONAL,
	IN ULONG                Flags OPTIONAL,
	IN PUNICODE_STRING      ModuleFileName,
	OUT PHANDLE             ModuleHandle );