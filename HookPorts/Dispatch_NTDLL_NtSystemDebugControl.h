#pragma once

#include "./Dispatch_NTDLL_NtResumeThread.h"

//
//Reverse engineering of enum made by tombkeeper(于旸)
//
typedef enum _SYSDBG_COMMAND {
	//以下是NT各个版本上都有
	SysDbgQueryModuleInformation,
	SysDbgQueryTraceInformation,
	SysDbgSetTracepoint,
	SysDbgSetSpecialCall,
	SysDbgClearSpecialCalls,
	SysDbgQuerySpecialCalls,
	//以下是NT 5.1新增的
	SysDbgBreakPoint,
	SysDbgQueryVersion,//获取KdVersionBlock
	SysDbgReadVirtual,//从内核空间拷贝到用户空间
	SysDbgWriteVirtual,//从用户空间拷贝到内核空间
	SysDbgReadPhysical,//从物理地址拷贝到用户空间，不能写到内核空间
	SysDbgWritePhysical,//从用户空间拷贝到物理地址，不能读取内核空间
	SysDbgReadControlSpace,//读写处理器相关控制块
	SysDbgWriteControlSpace,//读写处理器相关控制块
	SysDbgReadIoSpace,//读写端口
	SysDbgWriteIoSpace,//读写端口
	SysDbgReadMsr,//分别调用RDMSR@4和_WRMSR@12
	SysDbgWriteMsr,//分别调用RDMSR@4和_WRMSR@12
	SysDbgReadBusData,//读写总线数据
	SysDbgWriteBusData,//读写总线数据
	SysDbgCheckLowMemory,
	//以下是NT 5.2 新增的
	SysDbgEnableKernelDebugger,//分别调用_KdEnableDebugger@0和_KdDisableDebugger@0
	SysDbgDisableKernelDebugger,//分别调用_KdEnableDebugger@0和_KdDisableDebugger@0
	SysDbgGetAutoKdEnable,//获取和设置一些调试相关的变量
	SysDbgSetAutoKdEnable,//获取和设置一些调试相关的变量
	SysDbgGetPrintBufferSize,//获取和设置一些调试相关的变量
	SysDbgSetPrintBufferSize,//获取和设置一些调试相关的变量
	SysDbgGetKdUmExceptionEnable,//获取和设置一些调试相关的变量
	SysDbgSetKdUmExceptionEnable,//获取和设置一些调试相关的变量
	SysDbgGetTriageDump,//获取和设置一些调试相关的变量
	SysDbgGetKdBlockEnable,//获取和设置一些调试相关的变量
	SysDbgSetKdBlockEnable,//获取和设置一些调试相关的变量
} SYSDBG_COMMAND, *PSYSDBG_COMMAND;

typedef NTSTATUS (NTAPI * __pfnNtSystemDebugControl)
(
	IN SYSDBG_COMMAND       Command,
	IN PVOID                InputBuffer OPTIONAL,
	IN ULONG                InputBufferLength,
	OUT PVOID               OutputBuffer OPTIONAL,
	IN ULONG                OutputBufferLength,
	OUT PULONG              ReturnLength OPTIONAL );

extern __pfnNtSystemDebugControl pfnNtSystemDebugControl;

NTSTATUS
NTAPI
OnNtSystemDebugControl(
	IN SYSDBG_COMMAND       Command,
	IN PVOID                InputBuffer OPTIONAL,
	IN ULONG                InputBufferLength,
	OUT PVOID               OutputBuffer OPTIONAL,
	IN ULONG                OutputBufferLength,
	OUT PULONG              ReturnLength OPTIONAL );

NTSTATUS
NTAPI
NtSystemDebugControl(
	IN SYSDBG_COMMAND       Command,
	IN PVOID                InputBuffer OPTIONAL,
	IN ULONG                InputBufferLength,
	OUT PVOID               OutputBuffer OPTIONAL,
	IN ULONG                OutputBufferLength,
	OUT PULONG              ReturnLength OPTIONAL );