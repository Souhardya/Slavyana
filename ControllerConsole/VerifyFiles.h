#pragma once

DWORD CheckFileTrust(IN LPCWSTR lpFileName);

BOOL _LoadFunc(void);

BOOL _CatCalcHash(IN LPCWSTR lpFileName, IN DWORD* pcbHash, IN BYTE** ppbHash);

HRESULT VerifySignatureOfPEFile(IN LPCWSTR lpFileName);