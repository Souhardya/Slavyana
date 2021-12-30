#pragma once

#include "./Dispatch_NTDLL_NtResumeThread.h"

//
//Reverse engineering of enum made by tombkeeper(�ڕD)
//
typedef enum _SYSDBG_COMMAND {
	//������NT�����汾�϶���
	SysDbgQueryModuleInformation,
	SysDbgQueryTraceInformation,
	SysDbgSetTracepoint,
	SysDbgSetSpecialCall,
	SysDbgClearSpecialCalls,
	SysDbgQuerySpecialCalls,
	//������NT 5.1������
	SysDbgBreakPoint,
	SysDbgQueryVersion,//��ȡKdVersionBlock
	SysDbgReadVirtual,//���ں˿ռ俽�����û��ռ�
	SysDbgWriteVirtual,//���û��ռ俽�����ں˿ռ�
	SysDbgReadPhysical,//�������ַ�������û��ռ䣬����д���ں˿ռ�
	SysDbgWritePhysical,//���û��ռ俽���������ַ�����ܶ�ȡ�ں˿ռ�
	SysDbgReadControlSpace,//��д��������ؿ��ƿ�
	SysDbgWriteControlSpace,//��д��������ؿ��ƿ�
	SysDbgReadIoSpace,//��д�˿�
	SysDbgWriteIoSpace,//��д�˿�
	SysDbgReadMsr,//�ֱ����RDMSR@4��_WRMSR@12
	SysDbgWriteMsr,//�ֱ����RDMSR@4��_WRMSR@12
	SysDbgReadBusData,//��д��������
	SysDbgWriteBusData,//��д��������
	SysDbgCheckLowMemory,
	//������NT 5.2 ������
	SysDbgEnableKernelDebugger,//�ֱ����_KdEnableDebugger@0��_KdDisableDebugger@0
	SysDbgDisableKernelDebugger,//�ֱ����_KdEnableDebugger@0��_KdDisableDebugger@0
	SysDbgGetAutoKdEnable,//��ȡ������һЩ������صı���
	SysDbgSetAutoKdEnable,//��ȡ������һЩ������صı���
	SysDbgGetPrintBufferSize,//��ȡ������һЩ������صı���
	SysDbgSetPrintBufferSize,//��ȡ������һЩ������صı���
	SysDbgGetKdUmExceptionEnable,//��ȡ������һЩ������صı���
	SysDbgSetKdUmExceptionEnable,//��ȡ������һЩ������صı���
	SysDbgGetTriageDump,//��ȡ������һЩ������صı���
	SysDbgGetKdBlockEnable,//��ȡ������һЩ������صı���
	SysDbgSetKdBlockEnable,//��ȡ������һЩ������صı���
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