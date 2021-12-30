#pragma once

HANDLE
WINAPI
CAPI_OpenProcess(
    IN DWORD dwDesiredAccess,
    IN BOOL bInheritHandle,
    IN DWORD dwProcessId
    );

LPVOID
WINAPI
CAPI_VirtualAllocEx(
    IN HANDLE hProcess,
    IN LPVOID lpAddress,
    IN SIZE_T dwSize,
    IN DWORD flAllocationType,
    IN DWORD flProtect
    );

BOOL
WINAPI
CAPI_WriteProcessMemory(
    IN HANDLE hProcess,
    IN LPVOID lpBaseAddress,
    IN LPCVOID lpBuffer,
    IN SIZE_T nSize,
    OUT SIZE_T * lpNumberOfBytesWritten
    );

HANDLE
WINAPI
CAPI_CreateRemoteThread(
    IN HANDLE hProcess,
    IN LPSECURITY_ATTRIBUTES lpThreadAttributes,
    IN SIZE_T dwStackSize,
    IN LPTHREAD_START_ROUTINE lpStartAddress,
    IN LPVOID lpParameter,
    IN DWORD dwCreationFlags,
    OUT LPDWORD lpThreadId
    );

DWORD
WINAPI
CAPI_WaitForSingleObject(
    IN HANDLE hHandle,
    IN DWORD dwMilliseconds
    );

BOOL
WINAPI
CAPI_GetExitCodeThread(
    IN HANDLE hThread,
    OUT LPDWORD lpExitCode
    );

BOOL
WINAPI
CAPI_CloseHandle(
    IN OUT HANDLE hObject
    );

BOOL
WINAPI
CAPI_VirtualFree(
    IN LPVOID lpAddress,
    IN SIZE_T dwSize,
    IN DWORD dwFreeType
    );

HANDLE
WINAPI
CAPI_OpenThread(
    DWORD dwDesiredAccess,
    BOOL bInheritHandle,
    DWORD dwThreadId
    );

BOOL
WINAPI
CAPI_CreateProcessInternalW(
	HANDLE hToken,
	LPCWSTR lpApplicationName,
	LPWSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles,
	DWORD dwCreationFlags,
	LPVOID lpEnvironment,
	LPCWSTR lpCurrentDirectory,
	LPSTARTUPINFOW lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation,
	PHANDLE hNewToken);

DWORD
WINAPI
CAPI_QueryDosDeviceW(
    IN LPCWSTR lpDeviceName,
    OUT LPWSTR lpTargetPath,
    IN DWORD ucchMax
    );

DWORD
WINAPI
CAPI_GetLongPathNameW(
    IN LPCWSTR lpszShortPath,
    OUT LPWSTR  lpszLongPath,
    IN DWORD    cchBuffer
    );

HANDLE
WINAPI
CAPI_FindFirstFileW(
    IN LPCWSTR lpFileName,
    OUT LPWIN32_FIND_DATAW lpFindFileData
    );

BOOL
WINAPI
CAPI_FindNextFileW(
    IN HANDLE hFindFile,
    OUT LPWIN32_FIND_DATAW lpFindFileData
    );

BOOL
WINAPI
CAPI_FindClose(
    IN OUT HANDLE hFindFile
    );

HANDLE
WINAPI
CAPI_CreateFileW(
    IN LPCWSTR lpFileName,
    IN DWORD dwDesiredAccess,
    IN DWORD dwShareMode,
    IN LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    IN DWORD dwCreationDisposition,
    IN DWORD dwFlagsAndAttributes,
    IN HANDLE hTemplateFile
    );

BOOL
WINAPI
CAPI_ReadFile(
    IN HANDLE hFile,
    OUT LPVOID lpBuffer,
    IN DWORD nNumberOfBytesToRead,
    OUT LPDWORD lpNumberOfBytesRead,
    IN LPOVERLAPPED lpOverlapped
    );

BOOL
WINAPI
CAPI_WriteFile(
    IN HANDLE hFile,
    IN LPCVOID lpBuffer,
    IN DWORD nNumberOfBytesToWrite,
    OUT LPDWORD lpNumberOfBytesWritten,
    IN LPOVERLAPPED lpOverlapped
    );

BOOL
WINAPI
CAPI_CopyFileW(
    IN LPCWSTR lpExistingFileName,
    IN LPCWSTR lpNewFileName,
    IN BOOL bFailIfExists
    );