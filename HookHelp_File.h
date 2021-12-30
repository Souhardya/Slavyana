#pragma once



int GetLongNtPathName(
	IN HANDLE hFileHandle,				//Handle to the file whose path is being requested
	IN HANDLE hRootDirectory,			//Handle to the directory whose path is being requested
	IN WCHAR * lpszFileName,			//Pointer to the file name
	IN WCHAR * lpszRestoreNtPath,		//Pointer to the restore path
	OUT WCHAR * lpszLongNtPath,			//Pointer to the buffer to receive the nt path
	OUT WCHAR * lpszDispatchNtPath		//Pointer to the buffer to receive the dispatch path
);

ULONG QueryFileAttributes( IN WCHAR * lpszLongNtPath );

int SetFileDeletedMark( IN WCHAR * lpszLongNtPath, IN WCHAR * lpszRestoreNtPath ,IN ULONG lpuFileAttributes );

int NtPathExist( IN WCHAR * lpszLongNtPath );

int GetDirectoryMarkAsDeletedFileCount( IN WCHAR * lpszLongNtPath );

int GetFileDeletedMark( IN WCHAR * lpszRestoreNtPath, IN WCHAR * lpszFileName, IN ULONG lpuFileNameLength );

int InitDirectoryForFileDispatch( IN WCHAR * lpszRestoreNtPath );

int QueryDispatchedFile( IN WCHAR * lpszRestoreNtPath );

BOOL CloneFileToDispatchPath( IN WCHAR * lpszLongNtPath, IN WCHAR * lpszRestoreNtPath, IN ULONG lpuFileAttributes );

BOOL ReadWriteCopyFile( IN LPCWSTR lpExistingFileName, IN LPCWSTR lpNewFileName );

int CopyDirectory( IN LPCWSTR lpExistingPathName, IN LPCWSTR lpNewPathName );