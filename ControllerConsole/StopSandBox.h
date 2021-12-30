#pragma once

int StopSandBoxRoutine(void);

int ListSandBoxToStop(void);

int StopSandBoxDispatch(IN const WCHAR * lpString);

int StopSandBoxByName(IN const WCHAR * lpSandBoxName);

int StopSandBoxUnLoadRegHive(IN const WCHAR * lpSandBoxName);

int StopSandBoxRemoveRegistrySYNC(IN const WCHAR * lpSandBoxName);

int StopSandBoxSaveRegHive(IN const WCHAR * lpSandBoxName);

int StopSandBoxUpdateRegHive(IN const WCHAR * lpSandBoxName);