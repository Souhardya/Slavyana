#pragma once

typedef BOOL (WINAPI * __pfnExitWindowsEx)
(
    IN UINT uFlags,
    IN DWORD dwReserved);

extern __pfnExitWindowsEx pfnExitWindowsEx;

BOOL
WINAPI
OnExitWindowsEx(
    IN UINT uFlags,
    IN DWORD dwReserved);