#pragma once

int LauncherInitalization(IN const WCHAR * lpSandBoxName);

int AdjustPrivileges(void);

int LogProcessInfoToRegistrySYNC(IN const WCHAR * lpSandBoxName, IN DWORD dwProcessId);

int SelfCheck(void);