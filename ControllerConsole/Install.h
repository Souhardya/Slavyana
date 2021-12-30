#pragma once

int CheckIsInstalled(void);

int InstallSandBox(void);

int CreateSoftwareRegistry(void);

int SetupSoftwareRegistry(void);

int SetupRegSandBox(IN const WCHAR * lpSandBoxName);

int SetupRegConfig(void);

int SetupRegRegistry(void);

int CreateCloneAPIDirectory(void);