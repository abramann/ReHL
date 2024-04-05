/*
		Copyright (C) 1996-1997 Id Software, Inc.

		This program is free software; you can redistribute it and/or
		modify it under the terms of the GNU General Public License
		as published by the Free Software Foundation; either version 2
		of the License, or (at your option) any later version.

		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

		See the GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with this program; if not, write to the Free Software
		Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
#include "precompiled.h"
#include "cl_servercache.h"


qboolean g_bIsCStrike;
qboolean g_bIsCZero;

int num_servers;
int cl_playerindex;
server_cache_t cached_servers[16];
netadr_t g_GameServerAddress;
netadr_t g_rconaddress;
char g_lastrconcommand[1024];
client_static_t cls;
client_state_t cl;

cl_entity_t* cl_entities = nullptr;

// TODO, allocate dynamically
efrag_t cl_efrags[MAX_EFRAGS] = {};
dlight_t cl_dlights[MAX_DLIGHTS] = {};
dlight_t cl_elights[MAX_ELIGHTS] = {};
lightstyle_t cl_lightstyle[MAX_LIGHTSTYLES] = {};

// TODO: implement API and add here - Solokiller
playermove_t g_clmove;

extern cvar_t sys_timescale;
int g_bRedirectedToProxy = 0;
int g_iCurrentTiming = 0;
startup_timing_t g_StartupTimings[32];

cvar_t cl_weaponlistfix = { "cl_weaponlistfix", "0" };
cvar_t cl_fixtimerate = { "cl_fixtimerate", "7.5" };
cvar_t cl_showmessages = { "cl_showmessages", "0" };
cvar_t cl_connect_test = { "cl_connect_test", "0" };
cvar_t cl_nointerp = { "cl_nointerp", "0" };
cvar_t cl_needinstanced = { "cl_needinstanced", "0" };
cvar_t cl_showfps = { "cl_showfps", "0", FCVAR_ARCHIVE };
cvar_t cl_cmdbackup = { "cl_cmdbackup", "2", FCVAR_ARCHIVE };
cvar_t cl_showevents = { "cl_showevents", "0" };
cvar_t cl_clockreset = { "cl_clockreset", "0.1" };
cvar_t cl_himodels = { "cl_himodels", "0", FCVAR_ARCHIVE };
cvar_t cl_gaitestimation = { "cl_gaitestimation", "1" };
cvar_t cl_timeout = { "cl_timeout", "60", FCVAR_ARCHIVE };
cvar_t cl_shownet = { "cl_shownet", "0" };
cvar_t cl_showsizes = { "cl_showsizes", "0", FCVAR_SPONLY };
cvar_t cl_solid_players = { "cl_solid_players", "1" };
cvar_t cl_nodelta = { "cl_nodelta", "0" };
cvar_t cl_idealpitchscale = { "cl_idealpitchscale", "0.8", FCVAR_ARCHIVE };
cvar_t rcon_address = { "rcon_address", "", FCVAR_PRIVILEGED };
cvar_t rcon_port = { "rcon_port", "0", FCVAR_PRIVILEGED };
cvar_t cl_resend = { "cl_resend", "6.0" };
cvar_t cl_slisttimeout = { "cl_slist", "10.0" };
cvar_t cl_allow_download = { "cl_allowdownload", "1", FCVAR_ARCHIVE };
cvar_t cl_allow_upload = { "cl_allowupload", "1", FCVAR_ARCHIVE };
cvar_t cl_cmdrate = { "cl_cmdrate", "60", FCVAR_ARCHIVE };
cvar_t cl_download_ingame = { "cl_download_ingame", "1", FCVAR_ARCHIVE };
cvar_t cl_logofile = { "cl_logofile", "lambda", FCVAR_ARCHIVE };
cvar_t cl_logocolor = { "cl_logocolor", "orange", FCVAR_ARCHIVE };
cvar_t password = { "password", "", FCVAR_USERINFO };
cvar_t cl_name = { "name", "unnamed", FCVAR_ARCHIVE | FCVAR_USERINFO | FCVAR_PRINTABLEONLY | FCVAR_NOEXTRAWHITEPACE };
cvar_t team = { "team", "", FCVAR_ARCHIVE | FCVAR_USERINFO };
cvar_t skin = { "skin", "", FCVAR_ARCHIVE | FCVAR_USERINFO };
cvar_t cl_model = { "model", "", FCVAR_ARCHIVE | FCVAR_USERINFO };
cvar_t topcolor = { "topcolor", "0", FCVAR_ARCHIVE | FCVAR_USERINFO };
cvar_t bottomcolor = { "bottomcolor", "0", FCVAR_ARCHIVE | FCVAR_USERINFO };
cvar_t rate = { "rate", "30000", FCVAR_ARCHIVE | FCVAR_USERINFO | FCVAR_FILTERABLE };
cvar_t cl_updaterate = { "cl_updaterate", "60", FCVAR_ARCHIVE | FCVAR_USERINFO };
cvar_t cl_lw = { "cl_lw", "1", FCVAR_ARCHIVE | FCVAR_USERINFO };
cvar_t cl_lc = { "cl_lc", "1", FCVAR_ARCHIVE | FCVAR_USERINFO };
cvar_t cl_dlmax = { "cl_dlmax", "512", FCVAR_ARCHIVE | FCVAR_USERINFO };
cvar_t fs_lazy_precache = { "fs_lazy_precache", "0" };
cvar_t fs_precache_timings = { "fs_precache_timings", "0" };
cvar_t fs_perf_warnings = { "fs_perf_warnings", "0" };
cvar_t fs_startup_timings = { "fs_startup_timings", "0" };
cvar_t cl_mousegrab = { "cl_mousegrab", "1", FCVAR_ARCHIVE };
cvar_t m_rawinput = { "m_rawinput", "0", FCVAR_ARCHIVE };
cvar_t cl_filterstuffcmd = { "cl_filterstuffcmd", "0", FCVAR_ARCHIVE | FCVAR_PRIVILEGED };


void SetupStartupTimings()
{
	g_iCurrentTiming = 0;
	g_StartupTimings[g_iCurrentTiming].name = "Startup";
	g_StartupTimings[g_iCurrentTiming].time = Sys_FloatTime();
}

void AddStartupTiming(const char* name)
{
	++g_iCurrentTiming;
	g_StartupTimings[g_iCurrentTiming].name = name;
	g_StartupTimings[g_iCurrentTiming].time = Sys_FloatTime();
}

void PrintStartupTimings()
{
	Con_Printf("Startup timings (%.2f total)\n", g_StartupTimings[g_iCurrentTiming].time - g_StartupTimings[0].time);
	Con_Printf("    0.00    Startup\n");

	// Print the relative time between each system startup
	for (int i = 1; i < g_iCurrentTiming; ++i)
	{
		Con_Printf("    %.2f    %s\n",
			g_StartupTimings[i].time - g_StartupTimings[i - 1].time,
			g_StartupTimings[i].name);
	}
}

void CL_CheckClientState(void)
{
	if ((cls.state == ca_connected || cls.state == ca_uninitialized) &&
		cls.signon == SIGNONS)
	{
		// first update is the final signon stage
		cls.state = ca_active;

		if (fs_startup_timings.value != 0.0)
			PrintStartupTimings();

		{
			unsigned char* IPPtr = cls.netchan.remote_address.ip;
			unsigned short port = cls.netchan.remote_address.port;

			if (!port)
			{
				IPPtr = net_local_adr.ip;
				port = net_local_adr.port;
			}

			VGuiWrap2_NotifyOfServerConnect(com_gamedir, *((int*)IPPtr), /*Q_*/ ntohs(port));
		}
	}
}

void CL_Connect_f(void) // This function crashes the game due to exception in client.dll (TODO: FIX - ScriptedSnark)
{
	char name[MAX_PATH];
	CareerStateType prevcareer = CAREER_NONE;

	const char* args = nullptr;

	if (Cmd_Argc() < 2)
	{
		Con_Printf("Usage:  connect <server>\n");
		return;
	}

	SetCStrikeFlags();

	if ((g_bIsCStrike || g_bIsCZero) && cmd_source != src_command)
	{
		Con_Printf("Connect only works from the console.\n");
		return;
	}

	args = Cmd_Args();

	if (!args)
		return;

	Q_strncpy(name, args, sizeof(name));
	name[sizeof(name) - 1] = 0;

	/* - TODO: implement demo playback - ScriptedSnark
	if (cls.demoplayback)
	CL_StopPlayback();
	*/

	prevcareer = g_careerState;

	// CL_Disconnect(); - TODO: implement - ScriptedSnark

	if (prevcareer == CAREER_LOADING)
		g_careerState = CAREER_LOADING;

	StartLoadingProgressBar("Connecting", 12);
	SetLoadingProgressBarStatusText("#GameUI_EstablishingConnection");

	int num = Q_atoi(name); // In case it's an index.

	if ((num > 0) && (num <= num_servers) && !Q_strstr(cls.servername, "."))
	{
		Q_strncpy(name, NET_AdrToString(cached_servers[num - 1].adr), sizeof(name));
		name[sizeof(name) - 1] = 0;
	}

	Q_memset(msg_buckets, 0, sizeof(msg_buckets));
	Q_memset(total_data, 0, sizeof(total_data));

	Q_strncpy(cls.servername, name, sizeof(cls.servername) - 1);
	cls.servername[sizeof(cls.servername) - 1] = 0;

	cls.state = ca_connecting;
	cls.connect_time = -99999;

	cls.connect_retry = 0;

	cls.passive = false;
	cls.spectator = false;
	cls.isVAC2Secure = false;

	cls.GameServerSteamID = 0;

	cls.build_num = 0;

	Q_memset(&g_GameServerAddress, 0, sizeof(netadr_t));
	cls.challenge = 0;

	gfExtendedError = false;
	g_LastScreenUpdateTime = 0.0;

	if (Q_strnicmp(cls.servername, "local", 5))
	{
		// allow remote
		NET_Config(true);
	}
}

void CL_Retry_f() // TODO: improve - ScriptedSnark
{
	char szCommand[260]; // [esp+1Ch] [ebp-110h] BYREF

	if (cls.servername[0])
	{
		if (strchr(cls.servername, 10) || strchr(cls.servername, 59))
			Con_Printf("Invalid command separator in server name, refusing retry\n");
		else
		{
			if (cls.passive)
				snprintf(szCommand, sizeof(szCommand), "listen %s\n", cls.servername);
			else
				snprintf(szCommand, sizeof(szCommand), "connect %s\n", cls.servername);

			Cbuf_AddText(szCommand);
			Con_Printf("Commencing connection retry to %s\n", cls.servername);
		}
	}
	else
	{
		Con_Printf("Can't retry, no previous connection\n");
	}
}


int CL_GetFragmentSize(void* state)

{
	NOT_IMPLEMENTED;

	return 0;
}

void CL_ConnectClient(void)
{
	NOT_IMPLEMENTED;
}

void CL_DeallocateDynamicData(void)
{
	NOT_IMPLEMENTED;
}


void CL_Disconnect(void)
{
	NOT_IMPLEMENTED;
}


void CL_Disconnect_f(void)
{
	NOT_IMPLEMENTED;
}


void CL_Listen(char* address)
{
	NOT_IMPLEMENTED;
}

void CL_SendRcon(netadr_t* to, uint32 challenge)
{
	NOT_IMPLEMENTED;

	/*
	qboolean qVar1;
	char local_48c[1160];

	if (g_lastrconcommand[0] == '\0') {
		Con_Printf("Rcon:  Empty rcon string\n");
		return;
	}
	qVar1 = NET_CompareAdr(net_from, g_rconaddress);
	if (qVar1 != false) {
		snprintf(local_48c, 0x480, "rcon %u \"%s\" %s", challenge, rcon_password.string, g_lastrconcommand);
		Netchan_OutOfBandPrint(NS_CLIENT, net_from, "%s", local_48c);
		return;
	}
	Con_Printf("Rcon:  Challenge from spoofed/invalid IP address\n");
	return;
	*/
}


char* CL_GetCDKeyHash(void)
{
	NOT_IMPLEMENTED;
	return nullptr;
}


void CL_SendConnectPacket(void)
{
	NOT_IMPLEMENTED;
}


void CL_PrintCachedServer(int slot)
{
	NOT_IMPLEMENTED;
}


void CL_AddToServerCache(server_cache_t* server)
{
	NOT_IMPLEMENTED;
}


int CL_ClientDllQuery(char* s)
{
	NOT_IMPLEMENTED;

	return 0;
}

void CL_ConnectionlessPacket(void)
{
	NOT_IMPLEMENTED;

	return;
}

extern cvar_t scr_downloading;


qboolean CL_PrecacheResources(void)

{
	resource_s* prVar1;
	model_s* pmVar2;
	byte bVar3;
	model_t* pmVar4;
	char* pcVar5;
	byte* pbVar6;
	int iVar7;
	sfx_t* psVar8;
	resource_s* strSource;
	int iVar9;
	double dVar10;

	SetLoadingProgressBarStatusText("#GameUI_PrecachingResources");
	ContinueLoadingProgressBar("ClientConnect", 7, 0.0);
	if (fs_startup_timings.value != 0.0) {
		iVar9 = g_iCurrentTiming + 1;
		g_iCurrentTiming = iVar9;
		g_StartupTimings[iVar9].name = "begin CL_PrecacheResources()";
		dVar10 = Sys_FloatTime();
		g_StartupTimings[iVar9].time = (float)dVar10;
	}
	if ((cl.resourcesonhand.pNext != (resource_s*)0x0) &&
		(strSource = cl.resourcesonhand.pNext, cl.resourcesonhand.pNext != &cl.resourcesonhand)) {
		do {
			bVar3 = strSource->ucFlags;
			prVar1 = strSource->pNext;
			if ((bVar3 & 0x10) == 0) {
				if (strSource->type < t_world) {
					switch (strSource->type) {
					case t_sound:
						if ((bVar3 & 2) == 0) {
							S_BeginPrecaching();
							iVar9 = strSource->nIndex;
							psVar8 = S_PrecacheSound(strSource->szFileName);
							cl.sound_precache[iVar9] = psVar8;
							S_EndPrecaching();
							bVar3 = strSource->ucFlags;
							if ((cl.sound_precache[strSource->nIndex] == (sfx_s*)0x0) && ((bVar3 & 1) != 0)) {
								COM_ExplainDisconnection
									(true, "Cannot continue without sound %s, disconnecting.", strSource);
								CL_Disconnect();
								return false;
							}
						}
						else {
							cl.sound_precache[strSource->nIndex] = (sfx_s*)0x0;
							bVar3 = strSource->ucFlags;
						}
						break;
					case t_model:
						if ((cl.model_precache_count <= strSource->nIndex) &&
							(cl.model_precache_count = strSource->nIndex + 1, 0x200 < cl.model_precache_count)) {
							cl.model_precache_count = 0x200;
						}
						if (strSource->szFileName[0] == '*') goto LAB_0019e98c;
						if (fs_lazy_precache.value == 0.0) {
							iVar9 = strSource->nIndex;
						LAB_0019ea2d:
							pmVar4 = Mod_ForName(strSource->szFileName, false, true);
							cl.model_precache[iVar9] = pmVar4;
							pmVar2 = cl.model_precache[strSource->nIndex];
						}
						else {
							iVar7 = Q_strnicmp(strSource->szFileName, "maps", 4);
							iVar9 = strSource->nIndex;
							if (iVar7 == 0) goto LAB_0019ea2d;
							pmVar4 = Mod_FindName(true, strSource->szFileName);
							cl.model_precache[iVar9] = pmVar4;
							pmVar2 = cl.model_precache[strSource->nIndex];
						}
						if (((pmVar2 == (model_s*)0x0) && (strSource->ucFlags != '\0')) &&
							(Con_Printf("Model %s not found and not available from server\n", strSource),
								(strSource->ucFlags & 1) != 0)) {
							COM_ExplainDisconnection
								(true, "Cannot continue without model %s, disconnecting.", strSource);
							CL_Disconnect();
							return false;
						}
						iVar9 = Q_stricmp(strSource->szFileName, "models/player.mdl");
						if (iVar9 != 0) goto LAB_0019e98c;
						cl_playerindex = strSource->nIndex;
						bVar3 = strSource->ucFlags;
						break;
					case t_decal:
						if ((bVar3 & 4) == 0) {
							Draw_DecalSetName(strSource->nIndex, strSource->szFileName);
							bVar3 = strSource->ucFlags;
						}
						break;
					case t_eventscript:
						iVar9 = strSource->nIndex;
						pcVar5 = Mem_Strdup(strSource->szFileName);
						cl.event_precache[iVar9].filename = pcVar5;
						iVar9 = strSource->nIndex;
						pbVar6 = COM_LoadFile(strSource->szFileName, 5, (int*)0x0);
						cl.event_precache[iVar9].pszScript = (char*)pbVar6;
						if (cl.event_precache[strSource->nIndex].pszScript != (char*)0x0) goto LAB_0019e98c;
						Con_Printf("Script %s not found and not available from server\n", strSource);
						bVar3 = strSource->ucFlags;
						if ((bVar3 & 1) != 0) {
							COM_ExplainDisconnection
								(true, "Cannot continue without script %s, disconnecting.", strSource);
							CL_Disconnect();
							return false;
						}
					}
				}
				else {
					Con_DPrintf("Unknown resource type\n");
				LAB_0019e98c:
					bVar3 = strSource->ucFlags;
				}
				strSource->ucFlags = bVar3 | 0x10;
			}
		} while ((prVar1 != (resource_s*)0x0) && (strSource = prVar1, prVar1 != &cl.resourcesonhand));
	}
	if (fs_startup_timings.value == 0.0) {
		return true;
	}
	iVar9 = g_iCurrentTiming + 1;
	g_StartupTimings[iVar9].name = "end  CL_PrecacheResources()";
	g_iCurrentTiming = iVar9;
	dVar10 = Sys_FloatTime();
	g_StartupTimings[iVar9].time = (float)dVar10;
	return true;
}


void CL_ProcessFile(qboolean successfully_received, char* filename)
{
	NOT_IMPLEMENTED;
}



void CL_ProcessFile(char* filename, qboolean successfully_received)
{
	NOT_IMPLEMENTED;
}



void CL_ReadPackets(void)
{
	NOT_IMPLEMENTED;
}



void CL_SignonReply(void)
{
	NOT_IMPLEMENTED;
}



void CL_TakeSnapshot_f()
{
	model_s* in;
	int i;
	FileHandle_t file;
	char base[64];
	char filename[64];

	if (cl.num_entities && (in = cl_entities->model) != 0)
		COM_FileBase(in->name, base);
	else
		Q_strcpy(base, "Snapshot");

	for (i = 0; i != 1000; i++)
	{
		Q_snprintf(filename, sizeof(filename), "%s%04d.bmp", base, i);
		file = FS_OpenPathID(filename, "r", "GAMECONFIG");

		if (!file)
		{
			VID_TakeSnapshot(filename);
			return;
		}

		FS_Close(file);
	}

	Con_Printf("Unable to take a screenshot.\n");
}

void CL_ShutDownClientStatic() // probably need to improve if doesn't work
{
	NOT_IMPLEMENTED;
	
	/*
	int client = CL_UPDATE_BACKUP;
	int i;
	packet_entities_t* ent;

	for (i = 0; i < CL_UPDATE_BACKUP; i++)
	{
		ent = &cl.frames[i].packet_entities;
		if (ent->entities)
		{
			Mem_Free(ent->entities);
		}
		ent->entities = NULL;
	}

	Q_memset(cl.frames, 0, sizeof(frame_t) * client);
	*/
}

void CL_Shutdown()
{
	NOT_IMPLEMENTED;

	TextMessageShutdown();
}

void CL_Init()
{
	NOT_IMPLEMENTED;
	
	CL_InitTEnts();
	TextMessageInit();
	Cmd_AddCommand /*WithFlags*/("connect", CL_Connect_f /*, 8*/); // TODO: implement slowhacking protection - ScriptedSnark
	Cmd_AddCommand /*WithFlags*/("retry", CL_Retry_f /*, 8*/);		// TODO: also uncomment when this new func from GoldSrc update will be finished - ScriptedSnark
	Cmd_AddCommand("snapshot", CL_TakeSnapshot_f);
	Cvar_RegisterVariable(&rate);
	Cvar_RegisterVariable(&cl_lw);
	Cvar_RegisterVariable(&cl_showfps);
	Cvar_RegisterVariable(&cl_nointerp);
	Cvar_RegisterVariable(&dev_overview);
	Cvar_RegisterVariable(&cl_mousegrab);
	Cvar_RegisterVariable(&m_rawinput);
}

dlight_t* CL_AllocDlight(int key)
{
	// TODO: implement - Solokiller
	return nullptr;
}

dlight_t* CL_AllocElight(int key) // - TODO: improve - ScriptedSnark
{
	dlight_t* dest; // ebx

	if (key)
	{
		dest = cl_elights;

		while (dest->key != key)
		{
			if (dest++ == &cl_elights[64])
				goto Alloc;
		}
	}
	else
	{

	Alloc:
		dest = cl_elights;
		while (cl.time <= (long double)dest->die)
		{
			if (dest++ == &cl_elights[64])
			{
				Q_memset(cl_elights, 0, 40);
				cl_elights[0].key = key;
				return cl_elights;
			}
		}
	}

	Q_memset(dest, 0, 40);
	dest->key = key;
	return dest;
}

model_t* CL_GetModelByIndex(int index)
{
	NOT_IMPLEMENTED;
	
	/*
	if (index >= MAX_MODELS)
		return nullptr;

	model_t* model = cl.model_precache[index]; // ebx

	if (!model)
		return nullptr;

	if (model->needload == NL_NEEDS_LOADED || model->needload == NL_UNREFERENCED)
	{
		if (fs_precache_timings.value == 0.0)
		{
			Mod_LoadModel(model, false, false);
		}
		else
		{
			double start = Sys_FloatTime();
			Mod_LoadModel(model, false, false);
			double end = Sys_FloatTime() - start;
			Con_DPrintf("fs_precache_timings: loaded model %s in time %.3f sec\n", model->name, end);
		}
	}

	return model;
	*/
	return nullptr;
}

void CL_GetPlayerHulls()
{
	for (int i = 0; i < 4; ++i)
	{
		if (!ClientDLL_GetHullBounds(i, player_mins[i], player_maxs[i]))
			break;
	}
}

bool UserIsConnectedOnLoopback()
{
	return cls.netchan.remote_address.type == NA_LOOPBACK;
}

void CL_HudMessage(const char* pMessage)
{
	DispatchDirectUserMsg("HudText", Q_strlen(pMessage), (void*)pMessage);
}

void SetPal(int i)
{
	// Nothing
}

void GetPos(vec3_t origin, vec3_t angles)
{
	NOT_IMPLEMENTED;
	
	/*
	origin[0] = r_refdef.vieworg[0];
	origin[1] = r_refdef.vieworg[1];
	origin[2] = r_refdef.vieworg[2];

	angles[0] = r_refdef.viewangles[0];
	angles[1] = r_refdef.viewangles[1];
	angles[2] = r_refdef.viewangles[2];

	if (Cmd_Argc() == 2)
	{
		if (Q_atoi(Cmd_Argv(1)) == 2 && cls.state == ca_active)
		{
			origin[0] = cl.frames[cl.parsecountmod].playerstate[cl.playernum].origin[0];
			origin[1] = cl.frames[cl.parsecountmod].playerstate[cl.playernum].origin[1];
			origin[2] = cl.frames[cl.parsecountmod].playerstate[cl.playernum].origin[2];

			angles[0] = cl.frames[cl.parsecountmod].playerstate[cl.playernum].angles[0];
			angles[1] = cl.frames[cl.parsecountmod].playerstate[cl.playernum].angles[1];
			angles[2] = cl.frames[cl.parsecountmod].playerstate[cl.playernum].angles[2];
		}
	}
	*/
}

const char* CL_CleanFileName(const char* filename)
{
	if (filename && *filename && *filename == '!')
		return "customization";

	return filename;
}

void CL_ClearCaches()
{
	for (int i = 1; i < ARRAYSIZE(cl.event_precache) && cl.event_precache[i].pszScript; ++i)
	{
		Mem_Free(const_cast<char*>(cl.event_precache[i].pszScript));
		Mem_Free(const_cast<char*>(cl.event_precache[i].filename));

		Q_memset(&cl.event_precache[i], 0, sizeof(cl.event_precache[i]));
	}
}

void CL_ClearClientState()
{
	NOT_IMPLEMENTED;
	
	/*
	for (int i = 0; i < CL_UPDATE_BACKUP; ++i)
	{
		if (cl.frames[i].packet_entities.entities)
		{
			Mem_Free(cl.frames[i].packet_entities.entities);
		}

		cl.frames[i].packet_entities.entities = nullptr;
		cl.frames[i].packet_entities.num_entities = 0;
	}

	CL_ClearResourceLists();

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		COM_ClearCustomizationList(&cl.players[i].customdata, false);
	}

	CL_ClearCaches();

	Q_memset(&cl, 0, sizeof(cl));

	cl.resourcesneeded.pPrev = &cl.resourcesneeded;
	cl.resourcesneeded.pNext = &cl.resourcesneeded;
	cl.resourcesonhand.pPrev = &cl.resourcesonhand;
	cl.resourcesonhand.pNext = &cl.resourcesonhand;

	CL_CreateResourceList();
	*/
}

void CL_ClearState(bool bQuiet)
{
	if (!Host_IsServerActive())
		Host_ClearMemory(bQuiet);

	CL_ClearClientState();

	// TODO: implement - Solokiller
	SZ_Clear(&cls.netchan.message);

	// clear other arrays
	Q_memset(cl_efrags, 0, sizeof(cl_efrags));
	Q_memset(cl_dlights, 0, sizeof(cl_dlights));
	Q_memset(cl_elights, 0, sizeof(cl_elights));
	Q_memset(cl_lightstyle, 0, sizeof(cl_lightstyle));

	CL_TempEntInit();

	//
	// allocate the efrags and chain together into a free list
	//
	cl.free_efrags = cl_efrags;

	int i;
	for (i = 0; i < MAX_EFRAGS - 1; ++i)
	{
		cl.free_efrags[i].entnext = &cl.free_efrags[i + 1];
	}

	cl.free_efrags[i].entnext = nullptr;
}

void CL_CreateResourceList()
{
	NOT_IMPLEMENTED;
}

void CL_ComputeClientInterpolationAmount(usercmd_t* cmd)
{
	NOT_IMPLEMENTED;
}


void CL_ComputePacketLoss(void)
{
	NOT_IMPLEMENTED;
}


void CL_Move(void)
{
	NOT_IMPLEMENTED;
}


void CL_Parse_DeltaDescription(void)
{
	NOT_IMPLEMENTED;
}


void CL_Parse_Sound(void)
{
	NOT_IMPLEMENTED;
}

