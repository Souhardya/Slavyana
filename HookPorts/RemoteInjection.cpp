#include <windows.h>
#include <ntsecapi.h>
#include <tlhelp32.h>

#include "./RemoteInjection.h"
#include "../Common/DebugLog.h"
#include "./Main.h"
#include "./GetProcAddressEx.h"

#include "./CloneAPI_KERNEL32.h"

#include "./Dispatch_NTDLL_NtAllocateVirtualMemory.h"
#include "./Dispatch_NTDLL_NtWriteVirtualMemory.h"
#include "./Dispatch_NTDLL_NtProtectVirtualMemory.h"
#include "./Dispatch_NTDLL_NtGetContextThread.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"



//
//Define
//
DWORD Inject
(
	IN DWORD PID,
	IN void* pfnRemoteFunc,
	IN DWORD dwFuncSize,
	IN void* pRemoteParam,
	IN DWORD dwParamSize,
	IN DWORD dwMilliseconds,
	IN BOOL bFreeRemoteThread
);

typedef struct tagThreadParam
{
	DWORD dwRVA_pfnDestDllExport;
	DWORD dwRVA_pfnRtlInitUnicodeString;
	DWORD dwRVA_pfnLdrLoadDll;
	WCHAR szDllFileName[MAX_PATH];
    char szFunctionName[MAX_PATH];
	OBJECTCONFIG lp;
}ThreadParam, *PThreadParam;

DWORD WINAPI RemoteLoadLibrary(IN LPVOID lpParameter);



//
//RemoteInjection Functions
//
int InjectProcess(IN HMODULE hModule, IN DWORD dwProcessId, IN LPOBJECTCONFIG lp, IN LPCSTR szFnName)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	int ret = -1;
	ThreadParam tp;
	DWORD dwFuncSize = 1024*2;
	HINSTANCE hNTDLL = NULL;

	hNTDLL = GetModuleHandleW(L"ntdll.dll");

	//
	//Set [Relative Virtual Addresses] of [Dest!DllExport]
	//
	tp.dwRVA_pfnDestDllExport = (DWORD) ((DWORD)GetProcAddress(g_hinstDLL,szFnName) - (DWORD)g_hinstDLL);

	//
	//Set [Relative Virtual Addresses] of [ntdll!RtlInitUnicodeString]
	//
	tp.dwRVA_pfnRtlInitUnicodeString = (DWORD) ((DWORD)GetProcAddress(hNTDLL,"RtlInitUnicodeString") - (DWORD)hNTDLL);

	//
	//Set [Relative Virtual Addresses] of [ntdll!LdrLoadDll]
	//
	tp.dwRVA_pfnLdrLoadDll = (DWORD) ((DWORD)GetProcAddress(hNTDLL,"LdrLoadDll") - (DWORD)hNTDLL);

	//
	//Set Dll Export
	//
	strcpy(tp.szFunctionName, szFnName);

	//
	//Set Dll Path
	//
	GetModuleFileName(hModule, tp.szDllFileName, MAX_PATH);

	//
	//Set Dll Parameter
	//
	memcpy(&tp.lp, lp, sizeof(OBJECTCONFIG));

	ret = Inject( dwProcessId, RemoteLoadLibrary, dwFuncSize, &tp, sizeof(tp), 10*1000, FALSE);

	return ret;
}

DWORD WINAPI RemoteLoadLibrary(IN LPVOID lpParameter)
{
	DWORD dwRet = DEF_ThreadError_Unknown;

	PThreadParam tp = (PThreadParam)lpParameter;
	HANDLE hDll = NULL;

	//
	//Get [Module Base Address] of [ntdll.dll]
	//
	HINSTANCE hNTDLL = NULL;
	hNTDLL = (HINSTANCE)shellcode_getntdllmodbase();
	if( !hNTDLL )
	{
		return DEF_ThreadError_GetKERNEL32Mod;
	}

	//
	//Get [Proc Address] of [ntdll!RtlInitUnicodeString]
	//
	typedef VOID (NTAPI * __pfnRtlInitUnicodeString)
	(
		PUNICODE_STRING DestinationString,
		PCWSTR SourceString
		);

	__pfnRtlInitUnicodeString pfnRtlInitUnicodeString = NULL;
	pfnRtlInitUnicodeString = (__pfnRtlInitUnicodeString) (DWORD) ( (DWORD)hNTDLL + tp->dwRVA_pfnRtlInitUnicodeString );

	//
	//Get [Proc Address] of [ntdll!LdrLoadDll]
	//
	typedef NTSTATUS (NTAPI * __pfnLdrLoadDll)
	(
		IN PWCHAR               PathToFile OPTIONAL,
		IN ULONG                Flags OPTIONAL,
		IN PUNICODE_STRING      ModuleFileName,
		OUT PHANDLE             ModuleHandle );

	__pfnLdrLoadDll pfnLdrLoadDll = NULL;
	pfnLdrLoadDll = (__pfnLdrLoadDll) (DWORD) ( (DWORD)hNTDLL + tp->dwRVA_pfnLdrLoadDll );


	//
	//Init the UNICODE_STRING
	//
	UNICODE_STRING usDllFileName;
	pfnRtlInitUnicodeString(&usDllFileName,tp->szDllFileName);

	//
	//Call [ntdll!LdrLoadDll] to load [DestDll]
	//
	NTSTATUS nStatus;
	nStatus = pfnLdrLoadDll(NULL,NULL,&usDllFileName,&hDll);
	if( !hDll || nStatus != ((NTSTATUS)0x00000000L) )
	{
		return DEF_ThreadError_CallLoadLibraryA;
	}

	//
	//Get [Proc Address] of [DestDll!Export]
	//
	typedef BOOL (*PObjectStart)(LPVOID lp);
	PObjectStart pfnObjectStart = NULL;
	pfnObjectStart = (PObjectStart) (DWORD) ( (DWORD)hDll + tp->dwRVA_pfnDestDllExport );

	//
	//Call [DestDllExport]
	//
	dwRet = pfnObjectStart(&tp->lp);

	return dwRet;
}

DWORD Inject
(
	IN DWORD PID,
	IN void* pfnRemoteFunc,
	IN DWORD dwFuncSize,
	IN void* pRemoteParam,
	IN DWORD dwParamSize,
	IN DWORD dwMilliseconds,
	IN BOOL bFreeRemoteThread
)
{
    if( PID < 1 || dwFuncSize < 1 || pfnRemoteFunc == NULL )
	{
		return -1;
	}

    HANDLE hProc = NULL;
    void* pfnRmtFunc = NULL;
    void* pRmtParam = NULL;
    HANDLE hRemoteThread = NULL;
    DWORD dwThreadId = NULL;
    DWORD dwExitCode = NULL;
    BOOL bFlag;

    hProc = OpenProcess( PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION|PROCESS_VM_WRITE,FALSE,PID);		
    if( !hProc )
    {
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif
		goto InjectCleanUp;
    }

    pfnRmtFunc = VirtualAllocEx( hProc, NULL, dwFuncSize, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if( !pfnRmtFunc )
    {
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif
		goto InjectCleanUp;
    }

    pRmtParam = VirtualAllocEx( hProc, NULL, dwParamSize, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if( !pRmtParam )
    {
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif
		goto InjectCleanUp;
    }

    bFlag = WriteProcessMemory( hProc, pfnRmtFunc, (PVOID)pfnRemoteFunc, dwFuncSize, 0);
    if( bFlag == FALSE )
    {
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif
		goto InjectCleanUp;
    }

    bFlag = WriteProcessMemory( hProc, pRmtParam, (PVOID)pRemoteParam, dwParamSize, 0);
    if( bFlag == FALSE )
    {
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif
		goto InjectCleanUp;
    }

    hRemoteThread = CreateRemoteThread(
		hProc,
		NULL,
		NULL,
		(LPTHREAD_START_ROUTINE)pfnRmtFunc,
		(LPVOID)pRmtParam,
		CREATE_SUSPENDED,
		&dwThreadId);

    if( !hRemoteThread )
    {
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif
		goto InjectCleanUp;
    }
	else
	{
		PULONG SuspendCount = 0;
		NtResumeThread(hRemoteThread,SuspendCount);

//#ifdef Dbg
//		WCHAR szDebugString[256] = {0};
//		wsprintf(szDebugString,L"CreateRemoteThread SuspendCount=[%d]",SuspendCount);
//		DebugLog(DbgInfo,szDebugString);
//#endif
	}

    WaitForSingleObject( hRemoteThread, dwMilliseconds ); 
    GetExitCodeThread( hRemoteThread, &dwExitCode );

#ifdef Dbg
	WCHAR szThreadExitCode[256] = {0};
	wsprintf(szThreadExitCode,L"szThreadExitCode =[%d]",dwExitCode);
	DebugLog(DbgInfo,szThreadExitCode);
#endif

    CloseHandle( hProc );
    if( dwMilliseconds != 0 &&
		bFreeRemoteThread == TRUE
		)
    {
		VirtualFree( pfnRmtFunc, 0, MEM_RELEASE);
    }
    CloseHandle( hRemoteThread );

    return dwExitCode;

InjectCleanUp:
    if( hProc )
		CloseHandle( hProc );
    if( pfnRmtFunc )
		VirtualFree( pfnRmtFunc, 0, MEM_RELEASE);
    if( hRemoteThread )
		CloseHandle( hRemoteThread );

    return 0;
}

void * __stdcall shellcode_getkernel32modbase()
{
	//
	//Get [Module Base Address] of [kernel32.dll]
	//
	//(1) FS寄存器 -> TEB结构
	//(2) TEB+0x30 -> PEB结构
	//(3) PEB+0x0c -> PEB_LDR_DATA
	//(4) PEB_LDR_DATA+0x1c -> Ntdll.dll
	//(5) Ntdll.dll+0x08 -> Kernel32.dll
	//
	//typedef struct _PEB_LDR_DATA {
	//	ULONG Length; // +0x00
	//	BOOLEAN Initialized; // +0x04
	//	PVOID SsHandle; // +0x08
	//	LIST_ENTRY InLoadOrderModuleList; // +0x0c
	//	LIST_ENTRY InMemoryOrderModuleList; // +0x14
	//	LIST_ENTRY InInitializationOrderModuleList;// +0x1c
	//} PEB_LDR_DATA,*PPEB_LDR_DATA; // +0x24
	//
	//typedef struct _LDR_MODULE {
	//	LIST_ENTRY          InLoadOrderModuleList;   +0x00
	//	LIST_ENTRY          InMemoryOrderModuleList; +0x08 
	//	LIST_ENTRY          InInitializationOrderModuleList; +0x10
	//	void*               BaseAddress; +0x18
	//	void*               EntryPoint;   +0x1c
	//	ULONG               SizeOfImage;
	//	UNICODE_STRING      FullDllName;
	//	UNICODE_STRING      BaseDllName;
	//	ULONG               Flags;
	//	SHORT               LoadCount;
	//	SHORT               TlsIndex;
	//	HANDLE              SectionHandle;
	//	ULONG               CheckSum;
	//	ULONG               TimeDateStamp;
	//} LDR_MODULE, *PLDR_MODULE;
	//
	//typedef struct _LIST_ENTRY {
	//	struct _LIST_ENTRY *Flink;
	//	struct _LIST_ENTRY *Blink;
	//} LIST_ENTRY, *PLIST_ENTRY, *RESTRICTED_POINTER PRLIST_ENTRY;

	//
	//Example 1:
	//
	//__asm
	//{
	//	push esi
	//	mov eax, fs:0x30		//打开FS寄存器
	//	mov eax, [eax + 0x0c]	//得到PEB结构地址
	//	mov esi, [eax + 0x1c]	//得到PEB_LDR_DATA结构地址
	//	lodsd					//InInitializationOrderModuleList
	//	mov eax, [eax + 0x08]	//得到BaseAddress，即kernel32.dll基址
	//	pop esi
	//}
	//

	//
	//Example 2:
	//
	//__asm
	//{
	//	push esi
	//	mov eax,fs:[30h]
	//	mov eax,[eax+0Ch]
	//	mov eax,[eax+0Ch]
	//	mov eax,[eax]		//ntdll.dll
	//	mov eax,[eax]		//kernel32.dll
	//	mov eax,[eax+18h]
	//	pop esi
	//}

	__asm
	{
		push esi
		mov eax, fs:0x30		//打开FS寄存器
		mov eax, [eax + 0x0c]	//得到PEB结构地址
		mov esi, [eax + 0x1c]	//得到PEB_LDR_DATA结构地址
		lodsd					//InInitializationOrderModuleList
		mov eax, [eax + 0x08]	//得到BaseAddress，即kernel32.dll基址
		pop esi
	}
}

void * __stdcall shellcode_getntdllmodbase()
{
	__asm
	{
		push esi
		mov eax,fs:[30h]
		mov eax,[eax+0Ch]
		mov eax,[eax+0Ch]
		mov eax,[eax]		//ntdll.dll
		//mov eax,[eax]		//kernel32.dll
		mov eax,[eax+18h]
		pop esi
	}
}