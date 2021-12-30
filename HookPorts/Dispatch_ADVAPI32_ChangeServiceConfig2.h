#pragma once

typedef BOOL (WINAPI * __pfnChangeServiceConfig2A)
(
	SC_HANDLE    hService,
	DWORD        dwInfoLevel,
	LPVOID       lpInfo);

extern __pfnChangeServiceConfig2A pfnChangeServiceConfig2A;

BOOL
WINAPI
OnChangeServiceConfig2A(
	SC_HANDLE    hService,
	DWORD        dwInfoLevel,
	LPVOID       lpInfo);



typedef BOOL (WINAPI * __pfnChangeServiceConfig2W)
(
	SC_HANDLE    hService,
	DWORD        dwInfoLevel,
	LPVOID       lpInfo);

extern __pfnChangeServiceConfig2W pfnChangeServiceConfig2W;

BOOL
WINAPI
OnChangeServiceConfig2W(
	SC_HANDLE    hService,
	DWORD        dwInfoLevel,
	LPVOID       lpInfo);