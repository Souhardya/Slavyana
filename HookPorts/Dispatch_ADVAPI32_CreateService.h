#pragma once

typedef SC_HANDLE (WINAPI * __pfnCreateServiceA)
(
	SC_HANDLE    hSCManager,
	LPCSTR     lpServiceName,
	LPCSTR     lpDisplayName,
	DWORD        dwDesiredAccess,
	DWORD        dwServiceType,
	DWORD        dwStartType,
	DWORD        dwErrorControl,
	LPCSTR     lpBinaryPathName,
	LPCSTR     lpLoadOrderGroup,
	LPDWORD      lpdwTagId,
	LPCSTR     lpDependencies,
	LPCSTR     lpServiceStartName,
	LPCSTR     lpPassword);

extern __pfnCreateServiceA pfnCreateServiceA;

SC_HANDLE
WINAPI
OnCreateServiceA(
	SC_HANDLE    hSCManager,
	LPCSTR     lpServiceName,
	LPCSTR     lpDisplayName,
	DWORD        dwDesiredAccess,
	DWORD        dwServiceType,
	DWORD        dwStartType,
	DWORD        dwErrorControl,
	LPCSTR     lpBinaryPathName,
	LPCSTR     lpLoadOrderGroup,
	LPDWORD      lpdwTagId,
	LPCSTR     lpDependencies,
	LPCSTR     lpServiceStartName,
	LPCSTR     lpPassword);



typedef SC_HANDLE (WINAPI * __pfnCreateServiceW)
(
	SC_HANDLE    hSCManager,
	LPCWSTR     lpServiceName,
	LPCWSTR     lpDisplayName,
	DWORD        dwDesiredAccess,
	DWORD        dwServiceType,
	DWORD        dwStartType,
	DWORD        dwErrorControl,
	LPCWSTR     lpBinaryPathName,
	LPCWSTR     lpLoadOrderGroup,
	LPDWORD      lpdwTagId,
	LPCWSTR     lpDependencies,
	LPCWSTR     lpServiceStartName,
	LPCWSTR     lpPassword);

extern __pfnCreateServiceW pfnCreateServiceW;

SC_HANDLE
WINAPI
OnCreateServiceW(
	SC_HANDLE    hSCManager,
	LPCWSTR     lpServiceName,
	LPCWSTR     lpDisplayName,
	DWORD        dwDesiredAccess,
	DWORD        dwServiceType,
	DWORD        dwStartType,
	DWORD        dwErrorControl,
	LPCWSTR     lpBinaryPathName,
	LPCWSTR     lpLoadOrderGroup,
	LPDWORD      lpdwTagId,
	LPCWSTR     lpDependencies,
	LPCWSTR     lpServiceStartName,
	LPCWSTR     lpPassword);