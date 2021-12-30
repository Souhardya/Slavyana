#pragma once



//
//NOTICE
//
#define Dbg



//
//Define
//
#define DbgInfo __FILE__,__FUNCTION__,__LINE__



void DebugLog(LPCSTR lpFILE, LPCSTR lpFUNCTION, int lpLINE, LPCWSTR lpOutputString);