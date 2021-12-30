#include <windows.h>
#include <stdio.h>

#include "./DebugLog.h"



//
//Usage
//
//#ifdef Dbg
//DebugLog(DbgInfo,L"XXX");
//#endif



//
//Define
//
#define DbgLine "\r\n%s-%s-%d:    "



void DebugLog(LPCSTR lpFILE, LPCSTR lpFUNCTION, int lpLINE, LPCWSTR lpOutputString)
{
#ifdef Dbg
	char strDbgLine[512] = {0};

	wsprintfA(strDbgLine,DbgLine,lpFILE,lpFUNCTION,lpLINE);

	OutputDebugStringA(strDbgLine);
	OutputDebugStringW(lpOutputString);
#endif

	return;
}