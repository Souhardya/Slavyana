#pragma once

int RemoveSandBox(void);

int RemoveSoftwareRegistry(void);

int RemoveRestorePath(void);

int RemoveDirectoryEx(IN const WCHAR * lpPathName);

int RemoveCloneAPIDirectory(void);