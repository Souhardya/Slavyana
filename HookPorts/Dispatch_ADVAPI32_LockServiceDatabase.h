#pragma once

typedef SC_LOCK (WINAPI * __pfnLockServiceDatabase)
(
	SC_HANDLE hSCManager);

extern __pfnLockServiceDatabase pfnLockServiceDatabase;

SC_LOCK
WINAPI
OnLockServiceDatabase(
	SC_HANDLE hSCManager);