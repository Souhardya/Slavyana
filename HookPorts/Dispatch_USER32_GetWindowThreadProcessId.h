#pragma once

typedef DWORD (WINAPI * __pfnGetWindowThreadProcessId)
(
    IN HWND hWnd,
    OUT LPDWORD lpdwProcessId);

extern __pfnGetWindowThreadProcessId pfnGetWindowThreadProcessId;

DWORD
WINAPI
OnGetWindowThreadProcessId(
    IN HWND hWnd,
    OUT LPDWORD lpdwProcessId);