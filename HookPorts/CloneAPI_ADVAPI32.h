#pragma once

LONG
WINAPI
CAPI_RegCreateKeyW(
    IN HKEY hKey,
    IN LPCWSTR lpSubKey,
    OUT PHKEY phkResult
    );

LONG
WINAPI
CAPI_RegCloseKey(
    IN HKEY hKey
    );

LONG
WINAPI
CAPI_RegOpenKeyExW(
    IN HKEY hKey,
    IN LPCWSTR lpSubKey,
    IN DWORD ulOptions,
    IN REGSAM samDesired,
    OUT PHKEY phkResult
    );

LONG
WINAPI
CAPI_RegQueryValueExW(
    IN HKEY hKey,
    IN LPCWSTR lpValueName,
    IN LPDWORD lpReserved,
    OUT LPDWORD lpType,
    IN OUT LPBYTE lpData,
    IN OUT LPDWORD lpcbData
    );