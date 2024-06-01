#include "hooker.h"
#include "precompiled.h"
#include <Windows.h>
#include "sys_dll2.h"

void SetupHooks()
{
	DirectHook(0xAA790, Sys_FloatTime); // static vars
	DirectHook(0x96EB0, SV_ResetModInfo);
	DirectHook(0xAC000, Sys_InitFloatTime);
	DirectHook(0xAC1D0, Sys_InitMemory);
	DirectHook(0x4DA10, GL_SetMode);	// static vars
	DirectHook(0x27570, Cbuf_Execute); // TODO
	DirectHook(0xC8970, Memory_Init);
	DirectHook(0x2E960, Cvar_RegisterVariable);
	DirectHook(0x2E530, Cvar_DirectSet);
	DirectHook(0x2E960, Cvar_RegisterVariable);
	DirectHook(0xC27F0, V_Init);
	DirectHook(0xCDC0, Chase_Init);
	DirectHook(0x2B450, COM_Init);
	DirectHook(0x5C2A0, Host_ClearSaveDirectory);
}

uintptr_t AddBase(uintptr_t offset)
{
	static HMODULE theBase = GetModuleHandleA("GameHW.dll");

	if (!theBase)
	{
		theBase = LoadLibraryA("GameHW.dll");
		if (theBase == 0)
		{
			MessageBoxA(NULL, "No GameHW.dll", "ReHL", MB_OK);
			exit(0);
		}
	}

	uintptr_t thePtr = (uintptr_t)offset + (uintptr_t)theBase;
	if (!thePtr)
		MessageBoxA(NULL, "Wrong address calculate", "ReHL Warning", MB_OK);

	return (uintptr_t)offset + (uintptr_t)theBase;
}

template<typename T>
inline void DirectHook(uintptr_t offset, T directedFunction)
{
	uintptr_t theAddr = AddBase(offset);
	DWORD protect;
	if (!VirtualProtect((LPVOID)theAddr, 6, PAGE_EXECUTE_READWRITE, &protect))
		MessageBoxA(NULL, "Failed get writing access to a memory", "ReHL", MB_OK);

	const BYTE mov_eax = 0xB8;
	const WORD jmp_eax = 0xE0FF;

	SIZE_T written = 1;
	bool failed;
	if (
		!(written = 1, WriteProcessMemory(GetCurrentProcess(), (LPVOID)theAddr, &mov_eax, written, &written)) ||
		!(written = 4, WriteProcessMemory(GetCurrentProcess(), (LPVOID)((BYTE*)theAddr + 1), (LPCVOID)&directedFunction, written, &written)) ||
		!(written = 2, WriteProcessMemory(GetCurrentProcess(), (LPVOID)((BYTE*)theAddr + 5), &jmp_eax, written, &written))
		)
		MessageBoxA(NULL, "Failed to patch call jump", "ReHL", MB_OK);

	VirtualProtect((LPVOID)theAddr, 6, protect, &protect);
}
