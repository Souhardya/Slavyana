#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtSystemDebugControl.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"



//
//Global
//
__pfnNtSystemDebugControl pfnNtSystemDebugControl = NULL;



//
//Dispatch_NTDLL_NtSystemDebugControl Functions
//
NTSTATUS
NTAPI
OnNtSystemDebugControl(
	IN SYSDBG_COMMAND       Command,
	IN PVOID                InputBuffer OPTIONAL,
	IN ULONG                InputBufferLength,
	OUT PVOID               OutputBuffer OPTIONAL,
	IN ULONG                OutputBufferLength,
	OUT PULONG              ReturnLength OPTIONAL )
{
	NTSTATUS nRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		nRet = pfnNtSystemDebugControl(
			Command,
			InputBuffer,
			InputBufferLength,
			OutputBuffer,
			OutputBufferLength,
			ReturnLength
			);

		return nRet;
	}

//	//
//	//Check Command
//	//
//	BOOL bBypass = FALSE;
//	WCHAR szControlCodeInfo[128] = {0};
//	switch( Command )
//	{
//		case SysDbgQueryModuleInformation:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgQueryModuleInformation");
//			}break;
//		case SysDbgQueryTraceInformation:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgQueryTraceInformation");
//			}break;
//		case SysDbgSetTracepoint:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgSetTracepoint");
//			}break;
//		case SysDbgSetSpecialCall:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgSetSpecialCall");
//			}break;
//		case SysDbgClearSpecialCalls:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgClearSpecialCalls");
//			}break;
//		case SysDbgQuerySpecialCalls:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgQuerySpecialCalls");
//			}break;
//		case SysDbgBreakPoint:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgBreakPoint");
//			}break;
//		case SysDbgQueryVersion:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgQueryVersion");
//			}break;
//		case SysDbgReadVirtual:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgReadVirtual");
//			}break;
//		case SysDbgWriteVirtual:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgWriteVirtual");
//			}break;
//		case SysDbgReadPhysical:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgReadPhysical");
//			}break;
//		case SysDbgWritePhysical:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgWritePhysical");
//			}break;
//		case SysDbgReadControlSpace:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgReadControlSpace");
//			}break;
//		case SysDbgWriteControlSpace:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgWriteControlSpace");
//			}break;
//		case SysDbgReadIoSpace:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgReadIoSpace");
//			}break;
//		case SysDbgWriteIoSpace:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgWriteIoSpace");
//			}break;
//		case SysDbgReadMsr:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgReadMsr");
//			}break;
//		case SysDbgWriteMsr:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgWriteMsr");
//			}break;
//		case SysDbgReadBusData:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgReadBusData");
//			}break;
//		case SysDbgWriteBusData:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgWriteBusData");
//			}break;
//		case SysDbgCheckLowMemory:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgCheckLowMemory");
//			}break;
//		case SysDbgEnableKernelDebugger:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgEnableKernelDebugger");
//			}break;
//		case SysDbgDisableKernelDebugger:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgDisableKernelDebugger");
//			}break;
//		case SysDbgGetAutoKdEnable:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgGetAutoKdEnable");
//			}break;
//		case SysDbgSetAutoKdEnable:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgSetAutoKdEnable");
//			}break;
//		case SysDbgGetPrintBufferSize:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgGetPrintBufferSize");
//			}break;
//		case SysDbgSetPrintBufferSize:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgSetPrintBufferSize");
//			}break;
//		case SysDbgGetKdUmExceptionEnable:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgGetKdUmExceptionEnable");
//			}break;
//		case SysDbgSetKdUmExceptionEnable:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgSetKdUmExceptionEnable");
//			}break;
//		case SysDbgGetTriageDump:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgGetTriageDump");
//			}break;
//		case SysDbgGetKdBlockEnable:
//			{
//				bBypass = TRUE;
//				lstrcpy(szControlCodeInfo,L"SysDbgGetKdBlockEnable");
//			}break;
//		case SysDbgSetKdBlockEnable:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"SysDbgSetKdBlockEnable");
//			}
//			break;
//		default:
//			{
//				bBypass = FALSE;
//				lstrcpy(szControlCodeInfo,L"UNKNOWN SysDbgCommand");
//			}break;
//	}
//
//#ifdef Dbg
//	WCHAR szDebugString[256] = {0};
//	wsprintf(
//		szDebugString,
//		L"OnNtSystemDebugControl ControlCode=[%s]",
//		szControlCodeInfo
//		);
//	DebugLog(DbgInfo,szDebugString);
//#endif
//
//	if( bBypass == TRUE )
//	{
//		nRet = pfnNtSystemDebugControl(
//			Command,
//			InputBuffer,
//			InputBufferLength,
//			OutputBuffer,
//			OutputBufferLength,
//			ReturnLength
//			);
//	}
//	else
//	{
//#ifdef Dbg
//		DebugLog(DbgInfo,L"STATUS_ACCESS_DENIED");
//#endif
//		nRet = STATUS_ACCESS_DENIED;
//	}
//
//	return nRet;

	//
	//Refuse directly
	//
	return STATUS_ACCESS_DENIED;
}

NTSTATUS
NTAPI
NtSystemDebugControl(
	IN SYSDBG_COMMAND       Command,
	IN PVOID                InputBuffer OPTIONAL,
	IN ULONG                InputBufferLength,
	OUT PVOID               OutputBuffer OPTIONAL,
	IN ULONG                OutputBufferLength,
	OUT PULONG              ReturnLength OPTIONAL )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtSystemDebugControl addFun = (__pfnNtSystemDebugControl)GetProcAddress(hDll,"NtSystemDebugControl");
	if(addFun)
	{
		ret = addFun(Command,InputBuffer,InputBufferLength,OutputBuffer,OutputBufferLength,ReturnLength);
	}
	return ret;
}