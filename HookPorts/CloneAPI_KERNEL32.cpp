#include <windows.h>

#include "./CloneAPI_KERNEL32.h"
#include "./Main.h"



//
//CloneAPI_KERNEL32 Functions
//
HANDLE
WINAPI
CAPI_OpenProcess(
    IN DWORD dwDesiredAccess,
    IN BOOL bInheritHandle,
    IN DWORD dwProcessId
    )
{
	HANDLE ret = NULL;
	typedef HANDLE (WINAPI *lpAddFun)(DWORD,BOOL,DWORD);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"OpenProcess");
	if( addFun != NULL )
	{
		ret = addFun(dwDesiredAccess,bInheritHandle,dwProcessId);
	}
	return ret;
}

LPVOID
WINAPI
CAPI_VirtualAllocEx(
    IN HANDLE hProcess,
    IN LPVOID lpAddress,
    IN SIZE_T dwSize,
    IN DWORD flAllocationType,
    IN DWORD flProtect
    )
{
	LPVOID ret = NULL;
	typedef LPVOID (WINAPI *lpAddFun)(HANDLE,LPVOID,SIZE_T,DWORD,DWORD);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"VirtualAllocEx");
	if( addFun != NULL )
	{
		ret = addFun(hProcess,lpAddress,dwSize,flAllocationType,flProtect);
	}
	return ret;
}

BOOL
WINAPI
CAPI_WriteProcessMemory(
    IN HANDLE hProcess,
    IN LPVOID lpBaseAddress,
    IN LPCVOID lpBuffer,
    IN SIZE_T nSize,
    OUT SIZE_T * lpNumberOfBytesWritten
    )
{
	BOOL ret = NULL;
	typedef BOOL (WINAPI *lpAddFun)(HANDLE,LPVOID,LPCVOID,SIZE_T,SIZE_T *);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"WriteProcessMemory");
	if( addFun != NULL )
	{
		ret = addFun(hProcess,lpBaseAddress,lpBuffer,nSize,lpNumberOfBytesWritten);
	}
	return ret;
}

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
    )
{
	HANDLE ret = NULL;
	typedef HANDLE (WINAPI *lpAddFun)(HANDLE,LPSECURITY_ATTRIBUTES,SIZE_T,LPTHREAD_START_ROUTINE,LPVOID,DWORD,LPDWORD);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"CreateRemoteThread");
	if( addFun != NULL )
	{
		ret = addFun(hProcess,lpThreadAttributes,dwStackSize,lpStartAddress,lpParameter,dwCreationFlags,lpThreadId);
	}
	return ret;
}

DWORD
WINAPI
CAPI_WaitForSingleObject(
    IN HANDLE hHandle,
    IN DWORD dwMilliseconds
    )
{
	DWORD ret = NULL;
	typedef DWORD (WINAPI *lpAddFun)(HANDLE,DWORD);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"WaitForSingleObject");
	if( addFun != NULL )
	{
		ret = addFun(hHandle,dwMilliseconds);
	}
	return ret;
}

BOOL
WINAPI
CAPI_GetExitCodeThread(
    IN HANDLE hThread,
    OUT LPDWORD lpExitCode
    )
{
	BOOL ret = NULL;
	typedef BOOL (WINAPI *lpAddFun)(HANDLE,LPDWORD);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"GetExitCodeThread");
	if( addFun != NULL )
	{
		ret = addFun(hThread,lpExitCode);
	}
	return ret;
}

BOOL
WINAPI
CAPI_CloseHandle(
    IN OUT HANDLE hObject
    )
{
	BOOL ret = NULL;
	typedef BOOL (WINAPI *lpAddFun)(HANDLE);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"CloseHandle");
	if( addFun != NULL )
	{
		ret = addFun(hObject);
	}
	return ret;
}

BOOL
WINAPI
CAPI_VirtualFree(
    IN LPVOID lpAddress,
    IN SIZE_T dwSize,
    IN DWORD dwFreeType
    )
{
	BOOL ret = NULL;
	typedef BOOL (WINAPI *lpAddFun)(LPVOID,SIZE_T,DWORD);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"VirtualFree");
	if( addFun != NULL )
	{
		ret = addFun(lpAddress,dwSize,dwFreeType);
	}
	return ret;
}

HANDLE
WINAPI
CAPI_OpenThread(
    DWORD dwDesiredAccess,
    BOOL bInheritHandle,
    DWORD dwThreadId
    )
{
	HANDLE ret = NULL;
	typedef HANDLE (WINAPI *lpAddFun)(DWORD,BOOL,DWORD);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"OpenThread");
	if( addFun != NULL )
	{
		ret = addFun(dwDesiredAccess,bInheritHandle,dwThreadId);
	}
	return ret;
}

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
	PHANDLE hNewToken)
{
	BOOL ret = NULL;
	typedef BOOL (WINAPI *lpAddFun)(HANDLE,LPCWSTR,LPWSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,BOOL,DWORD,LPVOID,LPCWSTR,LPSTARTUPINFOW,LPPROCESS_INFORMATION,PHANDLE);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"CreateProcessInternalW");
	if( addFun != NULL )
	{
		ret = addFun(hToken,lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation,hNewToken);
	}
	return ret;
}

DWORD
WINAPI
CAPI_QueryDosDeviceW(
    IN LPCWSTR lpDeviceName,
    OUT LPWSTR lpTargetPath,
    IN DWORD ucchMax
    )
{
	DWORD ret = NULL;
	typedef DWORD (WINAPI *lpAddFun)(LPCWSTR,LPWSTR,DWORD);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"QueryDosDeviceW");
	if( addFun != NULL )
	{
		ret = addFun(lpDeviceName,lpTargetPath,ucchMax);
	}
	return ret;
}

DWORD
WINAPI
CAPI_GetLongPathNameW(
    IN LPCWSTR lpszShortPath,
    OUT LPWSTR  lpszLongPath,
    IN DWORD    cchBuffer
    )
{
	DWORD ret = NULL;
	typedef DWORD (WINAPI *lpAddFun)(LPCWSTR,LPWSTR,DWORD);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"GetLongPathNameW");
	if( addFun != NULL )
	{
		ret = addFun(lpszShortPath,lpszLongPath,cchBuffer);
	}
	return ret;
}

HANDLE
WINAPI
CAPI_FindFirstFileW(
    IN LPCWSTR lpFileName,
    OUT LPWIN32_FIND_DATAW lpFindFileData
    )
{
	HANDLE ret = NULL;
	typedef HANDLE (WINAPI *lpAddFun)(LPCWSTR,LPWIN32_FIND_DATAW);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"FindFirstFileW");
	if( addFun != NULL )
	{
		ret = addFun(lpFileName,lpFindFileData);
	}
	return ret;
}

BOOL
WINAPI
CAPI_FindNextFileW(
    IN HANDLE hFindFile,
    OUT LPWIN32_FIND_DATAW lpFindFileData
    )
{
	BOOL ret = NULL;
	typedef BOOL (WINAPI *lpAddFun)(HANDLE,LPWIN32_FIND_DATAW);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"FindNextFileW");
	if( addFun != NULL )
	{
		ret = addFun(hFindFile,lpFindFileData);
	}
	return ret;
}

BOOL
WINAPI
CAPI_FindClose(
    IN OUT HANDLE hFindFile
    )
{
	BOOL ret = NULL;
	typedef BOOL (WINAPI *lpAddFun)(HANDLE);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"FindClose");
	if( addFun != NULL )
	{
		ret = addFun(hFindFile);
	}
	return ret;
}

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
    )
{
	HANDLE ret = NULL;
	typedef HANDLE (WINAPI *lpAddFun)(LPCWSTR,DWORD,DWORD,LPSECURITY_ATTRIBUTES,DWORD,DWORD,HANDLE);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"CreateFileW");
	if( addFun != NULL )
	{
		ret = addFun(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
	}
	return ret;
}

BOOL
WINAPI
CAPI_ReadFile(
    IN HANDLE hFile,
    OUT LPVOID lpBuffer,
    IN DWORD nNumberOfBytesToRead,
    OUT LPDWORD lpNumberOfBytesRead,
    IN LPOVERLAPPED lpOverlapped
    )
{
	BOOL ret = NULL;
	typedef BOOL (WINAPI *lpAddFun)(HANDLE,LPVOID,DWORD,LPDWORD,LPOVERLAPPED);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"ReadFile");
	if( addFun != NULL )
	{
		ret = addFun(hFile,lpBuffer,nNumberOfBytesToRead,lpNumberOfBytesRead,lpOverlapped);
	}
	return ret;
}

BOOL
WINAPI
CAPI_WriteFile(
    IN HANDLE hFile,
    IN LPCVOID lpBuffer,
    IN DWORD nNumberOfBytesToWrite,
    OUT LPDWORD lpNumberOfBytesWritten,
    IN LPOVERLAPPED lpOverlapped
    )
{
	BOOL ret = NULL;
	typedef BOOL (WINAPI *lpAddFun)(HANDLE,LPCVOID,DWORD,LPDWORD,LPOVERLAPPED);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"WriteFile");
	if( addFun != NULL )
	{
		ret = addFun(hFile,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten,lpOverlapped);
	}
	return ret;
}

BOOL
WINAPI
CAPI_CopyFileW(
    IN LPCWSTR lpExistingFileName,
    IN LPCWSTR lpNewFileName,
    IN BOOL bFailIfExists
    )
{
	BOOL ret = NULL;
	typedef BOOL (WINAPI *lpAddFun)(LPCWSTR,LPCWSTR,BOOL);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"CopyFileW");
	if( addFun != NULL )
	{
		ret = addFun(lpExistingFileName,lpNewFileName,bFailIfExists);
	}
	return ret;
}