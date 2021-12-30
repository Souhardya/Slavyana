#pragma once

typedef BOOL (WINAPI * __pfnChangeServiceConfigA)
(
	SC_HANDLE    hService,
	DWORD        dwServiceType,
	DWORD        dwStartType,
	DWORD        dwErrorControl,
	LPCSTR     lpBinaryPathName,
	LPCSTR     lpLoadOrderGroup,
	LPDWORD      lpdwTagId,
	LPCSTR     lpDependencies,
	LPCSTR     lpServiceStartName,
	LPCSTR     lpPassword,
	LPCSTR     lpDisplayName);

extern __pfnChangeServiceConfigA pfnChangeServiceConfigA;

BOOL
WINAPI
OnChangeServiceConfigA(
	SC_HANDLE    hService,
	DWORD        dwServiceType,
	DWORD        dwStartType,
	DWORD        dwErrorControl,
	LPCSTR     lpBinaryPathName,
	LPCSTR     lpLoadOrderGroup,
	LPDWORD      lpdwTagId,
	LPCSTR     lpDependencies,
	LPCSTR     lpServiceStartName,
	LPCSTR     lpPassword,
	LPCSTR     lpDisplayName);



typedef BOOL (WINAPI * __pfnChangeServiceConfigW)
(
	SC_HANDLE    hService,
	DWORD        dwServiceType,
	DWORD        dwStartType,
	DWORD        dwErrorControl,
	LPCWSTR     lpBinaryPathName,
	LPCWSTR     lpLoadOrderGroup,
	LPDWORD      lpdwTagId,
	LPCWSTR     lpDependencies,
	LPCWSTR     lpServiceStartName,
	LPCWSTR     lpPassword,
	LPCWSTR     lpDisplayName);

extern __pfnChangeServiceConfigW pfnChangeServiceConfigW;

BOOL
WINAPI
OnChangeServiceConfigW(
	SC_HANDLE    hService,
	DWORD        dwServiceType,
	DWORD        dwStartType,
	DWORD        dwErrorControl,
	LPCWSTR     lpBinaryPathName,
	LPCWSTR     lpLoadOrderGroup,
	LPDWORD      lpdwTagId,
	LPCWSTR     lpDependencies,
	LPCWSTR     lpServiceStartName,
	LPCWSTR     lpPassword,
	LPCWSTR     lpDisplayName);