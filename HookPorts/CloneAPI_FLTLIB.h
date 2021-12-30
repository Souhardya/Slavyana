#pragma once

HRESULT
WINAPI
CAPI_FilterGetDosName(
	IN LPCWSTR  lpVolumeName,
	IN OUT LPWSTR  lpDosName,
	IN DWORD  dwDosNameBufferSize
	);