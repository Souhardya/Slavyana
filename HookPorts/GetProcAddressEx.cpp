#include <windows.h>

#include "./GetProcAddressEx.h"



//
//Define
//
DWORD __stdcall searchexport(IN DWORD modbase, IN DWORD szexportname);

void* __stdcall getkernel32modhandle();



//
//Routines
//
DWORD GetProcAddressEx(IN HMODULE hModule, IN LPCSTR pProcName)
{
	return searchexport( (DWORD) hModule,(DWORD) pProcName );
}

HINSTANCE GetKERNEL32ModHandle(void)
{
	return (HINSTANCE)getkernel32modhandle();
}

__declspec(naked)
DWORD __stdcall searchexport(IN DWORD modbase, IN DWORD szexportname)
{
	__asm
	{
		push ebp
		mov ebp,esp
		push ebx
		push esi
		push edi
		mov al,0
		mov ecx,256
		mov edi,szexportname
		cld
		repnz scasb
		mov edx,256
		sub edx,ecx//edx:strlen
		mov ebx,modbase//ebx: hold the modbase.
		mov esi,[ebx+60]//PEheaderRVA
		add esi,ebx
		add esi,78h//datadirectory,no.1 is exporttable
		mov eax,[esi]//exptable RVA
		add eax,ebx
		mov esi,eax//exptable
		mov ecx,[esi+24]//ecx:num_of_names
		mov eax,[esi+32]
		add eax,ebx//eax:namestable

looper:	cmp ecx,0
		jz error
		dec ecx
		mov esi,[eax+4*ecx]
		add esi,ebx
		inc ecx
		push ecx
		mov ecx,edx
		mov edi,szexportname
		cld
		repz cmpsb
		pop ecx
		jz found
		dec ecx
		jmp looper
found:	dec ecx
		mov esi,[ebx+60]//PEheaderRVA
		add esi,ebx
		add esi,78h//datadirectory,no.1 is exporttable
		mov esi,[esi]//exptable RVA
		add esi,ebx//exptable:esi
		mov esi,[esi+36]
		add esi,ebx//Index table
		mov ax,[esi+2*ecx]//got x
		mov esi,[ebx+60]//PEheaderRVA
		add esi,ebx
		add esi,78h//datadirectory,no.1 is exporttable
		mov esi,[esi]//exptable RVA
		add esi,ebx//exptable:esi
		mov esi,[esi+28]
		add esi,ebx//address table
		xor edi,edi
		movzx edi,ax
		mov eax,[esi+edi*4]
		add eax,ebx
		jmp end
error:
		xor eax,eax
		jmp end
end:
		pop edi
		pop esi
		pop ebx
		pop ebp
		ret 8
	}
}

void* __stdcall getkernel32modhandle()
{
	//
	//Get Module Handle of [kernel32.dll]
	//
	//(1) FS -> TEB
	//(2) TEB+0x30 -> PEB结构
	//(3) PEB+0x0c -> PEB_LDR_DATA
	//(4) PEB_LDR_DATA+0x1c -> Ntdll.dll
	//(5) Ntdll.dll+0x08 -> Kernel32.dll
	//
	//For example:
	//
	//__asm
	//{
	//	push esi
	//	mov eax, fs:0x30		
	//	mov eax, [eax + 0x0c]	
	//	mov esi, [eax + 0x1c]	
	//	lodsd					//InInitializationOrderModuleList
	//	mov eax, [eax + 0x08]	//BaseAddress，kernel32.dll
	//	pop esi
	//}

	__asm
	{
		push esi
		mov eax, fs:0x30		//FS
		mov eax, [eax + 0x0c]	//PEB
		mov esi, [eax + 0x1c]	//PEB_LDR_DATA
		lodsd					//InInitializationOrderModuleList
		mov eax, [eax + 0x08]	//BaseAddress，kernel32.dll
		pop esi
	}
}
