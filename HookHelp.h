#pragma once

int GetCurrentModuleInfo( IN HMODULE hModule );

int GetCloenAPIModuleInfo(void);

BOOL IsBypassCaller( IN DWORD lpdwReturnAddress );

BOOL IsProcessPatched( IN DWORD dwProcessId ,IN BOOL bQueryInCached );

BOOL InitPatchedProcessTable(void);

BOOL AddProcessIdToProcTable( IN DWORD dwProcessId );

BOOL DelProcessIdFromProcTable( IN DWORD dwProcessId );

int ScanCurrentProcModule(void);



//
//File Delete
//
int GetFileDeleteTraceCount( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName );

int QueryFileDeleteTrace( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName ,IN WCHAR * lpszFileName, IN ULONG lpuNameLength );

//int AddFileDeleteTrace( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName );

int DelFileDeleteTrace( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName );

int TestFileDeleteTrace( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName );



//
//File Dispatch
//
int AddFileDispatchTrace( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName ,OUT WCHAR * lpszNewFileNtPath );

int QueryFileDispatchTrace( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName ,OUT WCHAR * lpszNewFileNtPath );