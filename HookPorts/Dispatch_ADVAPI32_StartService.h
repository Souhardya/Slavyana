#pragma once

typedef BOOL (WINAPI * __pfnStartServiceA)
(
	SC_HANDLE            hService,
	DWORD                dwNumServiceArgs,
	LPCSTR             *lpServiceArgVectors);

extern __pfnStartServiceA pfnStartServiceA;

BOOL
WINAPI
OnStartServiceA(
	SC_HANDLE            hService,
	DWORD                dwNumServiceArgs,
	LPCSTR             *lpServiceArgVectors);



typedef BOOL (WINAPI * __pfnStartServiceW)
(
    SC_HANDLE            hService,
    DWORD                dwNumServiceArgs,
    LPCWSTR             *lpServiceArgVectors);

extern __pfnStartServiceW pfnStartServiceW;

BOOL
WINAPI
OnStartServiceW(
    SC_HANDLE            hService,
    DWORD                dwNumServiceArgs,
    LPCWSTR             *lpServiceArgVectors);