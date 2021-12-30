#pragma once

typedef BOOL (WINAPI * __pfnSetServiceStatus)
(
	SERVICE_STATUS_HANDLE hServiceStatus,
	LPSERVICE_STATUS lpServiceStatus);

extern __pfnSetServiceStatus pfnSetServiceStatus;

BOOL
WINAPI
OnSetServiceStatus(
	SERVICE_STATUS_HANDLE hServiceStatus,
	LPSERVICE_STATUS lpServiceStatus);