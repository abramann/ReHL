#include "hooker.h"
#include "precompiled.h"
#include <Windows.h>
#include "sys_dll2.h"

extern playermove_t ** sp_pmove;

void SetupHooks()
{
	DirectHook(0x70750, ran1);

	// One time call functions hook removed after implement caller function.
	
	DirectHook(0x27570, Cbuf_Execute); // TODO Cbuf_ExecuteCommandsFromBuffer
	
	//Sys_InitGame
	DirectHook(0xAA790, Sys_FloatTime);
	DirectHook(0x96EB0, SV_ResetModInfo);
	//DirectHook(0xAC000, Sys_InitFloatTime);
	//DirectHook(0xAC1D0, Sys_InitMemory);
	//DirectHook(0x4DA10, GL_SetMode);

	// Host_Init
	DirectHook(0xC8970, Memory_Init);
	DirectHook(0x2E960, Cvar_RegisterVariable);
	DirectHook(0x2E530, Cvar_DirectSet);
	DirectHook(0x2E960, Cvar_RegisterVariable);
	DirectHook(0xC27F0, V_Init);
	DirectHook(0xCDC0, Chase_Init);
	DirectHook(0x2B450, COM_Init);
	DirectHook(0x5C2A0, Host_ClearSaveDirectory);
	DirectHook(0x52E60, HPAK_Init);
	DirectHook(0xC56C0, W_LoadWadFile);

	DirectHook(0x61460, Key_Init);
	DirectHook(0x281A0, Cmd_AddCommand);
	DirectHook(0x28090, Cmd_AddCommandWithFlags);
	DirectHook(0x286C0, Cmd_ForwardToServerInternal);

	DirectHook(0x2C790, Con_CheckResize);	// Check implement
	DirectHook(0x2C920, Con_Init);
	//DirectHook(0x2FD70, Decal_Init); // Correct the wrong implement
	DirectHook(0x2FB20, Decal_MergeInDecals);
	DirectHook(0x2F360, Draw_CacheWadInitFromFile);
	DirectHook(0x28A90, Mod_Init);
	//DirectHook(0x69230, NET_Init);	// Correct wrong implement
	DirectHook(0x325E0, DELTA_Init);
#ifdef UNSRESOLVED_ISSUE
	DirectHook(0x9E450, SV_Init); // Implemented but couldn't resolve the CRT issue with _snprintf
#endif
	DirectHook(0xB1230, SystemWrapper_Init);
	DirectHook(0x8EF0, build_number);
	DirectHook(0x57B10, Host_Version);
	DirectHook(0x465E0, R_InitTextures);
	DirectHook(0x53440, HPAK_CheckIntegrity);
	DirectHook(0x51280, HPAK_FlushHostQueue);
	DirectHook(0x512E0, HPAK_AddLump);	// Check implement
	DirectHook(0x4CFF0, GL_Init);	// Game crashed one time during testing after added this function
	DirectHook(0x4CBC0, GL_Config);
	DirectHook(0x3C100, GL_SelectTexture);

	//DirectHook(0x6B2F0, PM_Init); // Under implement
	DirectHook(0x5DFD0, Info_ValueForKey);	// Check implement
	DirectHook(0x134A0, CL_Particle);
	DirectHook(0x58190, SV_GetPlayerHulls);
	DirectHook(0x6B810, PM_HullPointContents);
	DirectHook(0x810C0, R_StudioHull);
	DirectHook(0x40030, Mod_LoadModel);
	//DirectHook(0x2D1D0, CRC32_ProcessBuffer);
	DirectHook(0x29A70, COM_ExplainDisconnection);
	
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
		MessageBoxA(NULL, "Wrong address calculate", "ReHL", MB_OK);

	return thePtr;
}

template<typename T>
inline void DirectHook(uintptr_t offset, T directedFunction)
{
	uintptr_t theAddr = AddBase(offset);
	DWORD protect;
	if (!VirtualProtect((LPVOID)theAddr, 6, PAGE_EXECUTE_READWRITE, &protect))
		MessageBoxA(NULL, "Failed get writing access to memory", "ReHL", MB_OK);

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