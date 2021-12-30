#ifdef WIN32
#define RING3
#endif

#ifdef RING3
#include <windows.h>
#else
#include <windef.h>
#endif

#include "./InlineHook.h"
#include "./LDasm.h"

#ifdef RING3
#define __malloc(_s)	VirtualAlloc(NULL, _s, MEM_COMMIT, PAGE_EXECUTE_READWRITE)
#define __free(_p)		VirtualFree(_p, 0, MEM_RELEASE)
#define JMP_SIZE		5
#else
#define __malloc(_s)	ExAllocatePool(NonPagedPool, _s)
#define __free(_p)		ExFreePool(_p)
#define JMP_SIZE		7
#endif

#ifdef RING3

BOOL
WriteReadOnlyMemory(
	LPBYTE	lpDest,
	LPBYTE	lpSource,
	ULONG	Length
	)
{
	BOOL bRet;
	DWORD dwOldProtect;
	bRet = FALSE;

	if (!VirtualProtect(lpDest, Length, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{
		return bRet;
	}

	memcpy(lpDest, lpSource, Length);

	bRet = VirtualProtect(lpDest, Length, dwOldProtect, &dwOldProtect);

	return	bRet;
}

#else

NTSTATUS
WriteReadOnlyMemory(
	LPBYTE	lpDest,
	LPBYTE	lpSource,
	ULONG	Length
	)
{
	NTSTATUS status;
	KSPIN_LOCK spinLock;
	KIRQL oldIrql;
	PMDL pMdlMemory;
	LPBYTE lpWritableAddress;

	status = STATUS_UNSUCCESSFUL;

	pMdlMemory = IoAllocateMdl(lpDest, Length, FALSE, FALSE, NULL);

	if (NULL == pMdlMemory)
		return status;

	MmBuildMdlForNonPagedPool(pMdlMemory);
    MmProbeAndLockPages(pMdlMemory, KernelMode, IoWriteAccess);
	lpWritableAddress = MmMapLockedPages(pMdlMemory, KernelMode);
    if (NULL != lpWritableAddress)
	{
		oldIrql	= 0;
		KeInitializeSpinLock(&spinLock);
		KeAcquireSpinLock(&spinLock, &oldIrql);

		memcpy(lpWritableAddress, lpSource, Length);

		KeReleaseSpinLock(&spinLock, oldIrql);
		MmUnmapLockedPages(lpWritableAddress, pMdlMemory);

		status = STATUS_SUCCESS;
	}

	MmUnlockPages(pMdlMemory);
    IoFreeMdl(pMdlMemory);

	return status;
}

#endif

BOOL 
GetPatchSize(
	IN	void *Proc,			
	IN	DWORD dwNeedSize,	
	OUT LPDWORD lpPatchSize
	)
{
	DWORD Length;
	PUCHAR pOpcode;
	DWORD PatchSize = 0;

	if (!Proc || !lpPatchSize)
	{
		return FALSE;
	}

	do
	{
		Length = SizeOfCode(Proc, &pOpcode);
		if ((Length == 1) && (*pOpcode == 0xC3))
			break;
		if ((Length == 3) && (*pOpcode == 0xC2))
			break;
		Proc = (PVOID)((DWORD)Proc + Length);

		PatchSize += Length;
		if (PatchSize >= dwNeedSize)
		{
			break;
		}

	}while(Length);

	*lpPatchSize = PatchSize;

	return TRUE;
}

BOOL
InlineHook(
	IN	void *OrgProc,		
	IN	void *NewProc,		
	OUT	void **RealProc		
	)
{
	DWORD dwPatchSize;   
	//DWORD dwOldProtect;
	LPVOID lpHookFunc;    
	DWORD dwBytesNeed;    
	LPBYTE lpPatchBuffer; // jmp 

	if (!OrgProc || !NewProc || !RealProc)
	{
		return FALSE;
	}
	// patch
	if (!GetPatchSize(OrgProc, JMP_SIZE, &dwPatchSize))
	{
		return FALSE;
	}

	/*
	0x00000800					0x00000800		sizeof(DWORD)	// dwPatchSize
	JMP	/ FAR 0xAABBCCDD		E9 DDCCBBAA		JMP_SIZE
	...							...				dwPatchSize		// Backup instruction
	JMP	/ FAR 0xAABBCCDD		E9 DDCCBBAA		JMP_SIZE
	*/

	dwBytesNeed = sizeof(DWORD) + JMP_SIZE + dwPatchSize + JMP_SIZE;

	lpHookFunc = __malloc(dwBytesNeed);

	
	*(DWORD *)lpHookFunc = dwPatchSize;

	
	lpHookFunc = (LPVOID)((DWORD)lpHookFunc + sizeof(DWORD));

	
	memcpy((BYTE *)lpHookFunc + JMP_SIZE, OrgProc, dwPatchSize);

	lpPatchBuffer = (LPBYTE)__malloc(dwPatchSize);

	//NOP
	memset(lpPatchBuffer, 0x90, dwPatchSize);

#ifdef RING3
	//jmpHook
	*(BYTE *)lpHookFunc = 0xE9;
	*(DWORD*)((DWORD)lpHookFunc + 1) = (DWORD)NewProc - (DWORD)lpHookFunc - JMP_SIZE;

	
	*(BYTE *)((DWORD)lpHookFunc + 5 + dwPatchSize) = 0xE9;
	*(DWORD*)((DWORD)lpHookFunc + 5 + dwPatchSize + 1) = ((DWORD)OrgProc + dwPatchSize) - ((DWORD)lpHookFunc + JMP_SIZE + dwPatchSize) - JMP_SIZE;


	//jmp 
	*(BYTE *)lpPatchBuffer = 0xE9;
	//OrgProc
	*(DWORD*)(lpPatchBuffer + 1) = (DWORD)lpHookFunc - (DWORD)OrgProc - JMP_SIZE;

#else

	//jmpHook
	*(BYTE *)lpHookFunc = 0xEA;
	*(DWORD*)((DWORD)lpHookFunc + 1) = (DWORD)NewProc;
	*(WORD*)((DWORD)lpHookFunc + 5) = 0x08;

	
	*(BYTE *)((DWORD)lpHookFunc + JMP_SIZE + dwPatchSize) = 0xEA;
	*(DWORD*)((DWORD)lpHookFunc + JMP_SIZE + dwPatchSize + 1) = ((DWORD)OrgProc + dwPatchSize);
	*(WORD*)((DWORD)lpHookFunc + JMP_SIZE + dwPatchSize + 5) = 0x08;

	//jmp far
	*(BYTE *)lpPatchBuffer = 0xEA;
	
	
	*(DWORD*)(lpPatchBuffer + 1) = (DWORD)lpHookFunc;
	*(WORD*)(lpPatchBuffer + 5) = 0x08;
#endif

	WriteReadOnlyMemory((LPBYTE)OrgProc, lpPatchBuffer, dwPatchSize);

	__free(lpPatchBuffer);

	*RealProc = (void *)((DWORD)lpHookFunc + JMP_SIZE);

	return TRUE;
}

void UnInlineHook(
	void *OrgProc,  /* Hook */
	void *RealProc  
	)
{
	DWORD dwPatchSize;
	//DWORD dwOldProtect;
	LPBYTE lpBuffer;

	
	lpBuffer = (LPBYTE)((DWORD)RealProc - (sizeof(DWORD) + JMP_SIZE));
	
	dwPatchSize = *(DWORD *)lpBuffer;

	WriteReadOnlyMemory((LPBYTE)OrgProc, (LPBYTE)RealProc, dwPatchSize);

	
	__free(lpBuffer);

	return;
}
