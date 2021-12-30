#pragma once



//
//NOTICE
//
//Example
//typedef int (WINAPI * __pfnMessageBoxA)
//(
//    IN HWND hWnd,
//    IN LPCSTR lpText,
//    IN LPCSTR lpCaption,
//    IN UINT uType);
//int
//WINAPI
//OnMessageBoxA(
//    IN HWND hWnd,
//    IN LPCSTR lpText,
//    IN LPCSTR lpCaption,
//    IN UINT uType);
//__pfnMessageBoxA pfnMessageBoxA = NULL;
//
//HINSTANCE hUSER32 = NULL;
//hUSER32 = LoadLibrary(L"user32.dll");
//InlineHook((__pfnMessageBoxA)GetProcAddress(hUSER32,"MessageBoxA"), OnMessageBoxA, (void **)&pfnMessageBoxA);
//UnInlineHook((__pfnMessageBoxA)GetProcAddress(hUSER32,"MessageBoxA"), pfnMessageBoxA);



//
//Define
//
BOOL
WriteReadOnlyMemory(
	LPBYTE	lpDest,
	LPBYTE	lpSource,
	ULONG	Length
	);

BOOL 
GetPatchSize(
	IN	void *Proc,
	IN	DWORD dwNeedSize,
	OUT LPDWORD lpPatchSize
	);

BOOL
InlineHook(
	IN	void *OrgProc,		/* ��ҪHook�ĺ�����ַ */
	IN	void *NewProc,		/* ���汻Hook�����ĵ�ַ */
	OUT	void **RealProc		/* ����ԭʼ��������ڵ�ַ */
	);

void UnInlineHook(
	void *OrgProc,  /* ��Ҫ�ָ�Hook�ĺ�����ַ */
	void *RealProc  /* ԭʼ��������ڵ�ַ */
	);