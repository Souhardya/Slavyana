#pragma once

void wprintfex(IN WCHAR * lpText,IN WORD color);

void printfex(IN char * strText,IN WORD color);

void ResetConsoleTextAttribute(void);

void DisableForceClose(void);

void EnableForceClose(void);