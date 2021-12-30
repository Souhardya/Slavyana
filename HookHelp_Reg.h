#pragma once



int GetRegistryPathName(
	IN HANDLE hKeyHandle,
	IN HANDLE hRootDirectory,
	IN WCHAR * lpszObjectName,
	IN WCHAR * lpszRestoreRegUserPath,
	OUT WCHAR * lpszRegKrnlPath,
	OUT WCHAR * lpszDispatchRegKrnlPath,
	OUT WCHAR * lpszDispatchRegUserPath
);

int InitSubkeyForRegDispatch( IN WCHAR * lpszDispatchRegUserPath );

int QueryDispatchedRegValue( IN WCHAR * lpszDispatchRegUserPath, IN WCHAR * lpszValueName );