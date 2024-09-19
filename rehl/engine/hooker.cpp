#include "hooker.h"
#include "precompiled.h"
#include <Windows.h>
#include "sys_dll2.h"

extern playermove_t ** sp_pmove;

void SetupHooks()
{
	// One time call functions hook removed after implement caller function.
	
	DirectHook(0x70750, ran1);
	DirectHook(0x27570, Cbuf_Execute); // TODO Cbuf_ExecuteCommandsFromBuffer
	
	//Sys_InitGame
	DirectHook(0xAA790, Sys_FloatTime);
	DirectHook(0x96EB0, SV_ResetModInfo);

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
	//DirectHook(0x2FD70, Decal_Init); // Correct wrong implement
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
	
	DirectHook(0x6B2F0, PM_Init);

	// PM_Init functions
	DirectHook(0x6BF00, _PM_PlayerTrace);	// *
	DirectHook(0x810C0, R_StudioHull);	// *
	DirectHook(0x6B970, PM_PointContents);	// *
	DirectHook(0x6B8B0, PM_LinkContents);	// *
	DirectHook(0x6B810, PM_HullPointContents);	// *
	DirectHook(0x6C680, PM_TraceLine);	// *
	DirectHook(0xC90F8, SV_HullPointContents); // *
	DirectHook(0xC6A90, SV_RecursiveHullCheck);	// *
	DirectHook(0x5DFD0, Info_ValueForKey);	// *
	DirectHook(0x134A0, CL_Particle);	// *
	DirectHook(0x58190, SV_GetPlayerHulls);

	//Host_Init
	DirectHook(0xB180, ClientDLL_Init);
	DirectHook(0x1A830, HookServerMsg); // *
	
	//VGui_Startup
	//DirectHook(0xC1340, VGuiWrap_Startup); // Correct wrong implement around EngineSurfaceWrap
	DirectHook(0x70D0, VGuiWrap2_Startup);
	
	// Host_Init
	DirectHook(0x4E120, VID_Init);
	DirectHook(0x3C340, Draw_Init); // Check from LoadTransBMP
	DirectHook(0x4BDE0, SCR_Init);	// Fix alignment in TargaHeader & Check from Draw_PicFromWad
	DirectHook(0x46B80, R_Init);
	
	// R_Init
	DirectHook(0x3EC60, GL_LoadTexture2);	// *
	//(0x3E320, GL_Upload32);	// * Correct implement
	DirectHook(0x3E630, GL_Upload16);	// *

	//DirectHook(0x46440, R_RenderView);	// *
	//R_RenderView functions
	DirectHook(0x47390, R_BuildLightMap);
	DirectHook(0x49BC0, GL_BuildLightmaps); // Correct implement
	DirectHook(0x1A3E0, CL_GetModelByIndex);

	DirectHook(0x46350, R_RenderScene);
	// R_RenderScene is the home for most rendering functions
	DirectHook(0x89220, tri_GL_RenderMode);
	DirectHook(0x47900, R_DrawSequentialPoly); // *
	DirectHook(0x47F00, R_RenderDynamicLightmaps); // *
	DirectHook(0x47750, R_TextureAnimation); // static
	DirectHook(0x4F192, EmitWaterPolys); // *
	// Mod
	//DirectHook(0x40030, Mod_LoadModel);
	//DirectHook(0x3FD50, Mod_PointInLeaf);
	//DirectHook(0x2D1D0, CRC32_ProcessBuffer);
	DirectHook(0x28AB0, Mod_DecompressVis);	// static

	//DirectHook(0x29A70, COM_ExplainDisconnection);
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
