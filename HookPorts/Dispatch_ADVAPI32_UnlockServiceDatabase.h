#pragma once

typedef BOOL (WINAPI * __pfnUnlockServiceDatabase)
(
	SC_LOCK ScLock);

extern __pfnUnlockServiceDatabase pfnUnlockServiceDatabase;

BOOL
WINAPI
OnUnlockServiceDatabase(
	SC_LOCK ScLock);