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
#include "net_api.h"
#include "Sequence.h"
#include "DemoPlayerWrapper.h"

cvar_t cl_gamegauge = { "cl_gg", "0" };

static int g_i;

unsigned int g_lastground = UINT_MAX;

int num_servers;
int cl_playerindex;
server_cache_t cached_servers[16];
netadr_t g_GameServerAddress;
netadr_t g_rconaddress;
char g_lastrconcommand[1024];
//client_static_t cls;
//client_state_t cl;

cl_entity_t* cl_entities = nullptr;

// TODO, allocate dynamically
efrag_t cl_efrags[MAX_EFRAGS] = {};
dlight_t cl_dlights[MAX_DLIGHTS] = {};
dlight_t cl_elights[MAX_ELIGHTS] = {};
VARRAY(lightstyle_t, cl_lightstyle, [MAX_LIGHTSTYLES], 0x17812, {});

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
cvar_t fs_startup_timings = { "fs_startup_timings", "0" };

VVAR(cvar_t, fs_perf_warnings, 0x19F40, { "fs_perf_warnings" COMMA "0" }); 
VVAR(cvar_t, cl_mousegrab, 0x1A036, { "cl_mousegrab", "1", FCVAR_ARCHIVE });
VAR(playermove_t, g_clmove, 0xB240);

cvar_t m_rawinput = { "m_rawinput", "0", FCVAR_ARCHIVE };
cvar_t cl_filterstuffcmd = { "cl_filterstuffcmd", "0", FCVAR_ARCHIVE | FCVAR_PRIVILEGED };


void CL_GameDir_f(void);
void CL_FullServerinfo_f(void);
void CL_Record_f(void);
void CL_PlayDemo_f(void);
void CL_ViewDemo_f(void);
void CL_TimeDemo_f(void);
void CL_GameGauge_f(void);
void CL_ListDemo_f(void);
void CL_AppendDemo_f(void);
void CL_RemoveDemo_f(void);
void CL_SwapDemo_f(void);
void CL_SetDemoInfo_f(void);
void CL_Listen_f(void);
void CL_Commentator_f(void);
void CL_WavePlayLen_f(void);
void CL_StartMovie_f(void);
void CL_EndMovie_f(void);
void CL_PrintEntities_f(void);
void CL_Rcon_f(void);
void CL_DumpMessageLoad_f(void);
void CL_PingServers_f(void);
void CL_Slist_f(void);
void CL_ListCachedServers_f(void);
void CL_BeginUpload_f(void);
void CL_HTTPCancel_f(void);
void CL_SpecPos_f(void);
qboolean CL_ReadDemoMessage(void);
int CL_ReadDemoMessage_OLD();
void CL_ParseServerMessage(qboolean normal_message);
void CL_PredictMove(qboolean repredicting);
void CL_GetTargetTime(float *target);
void CL_SetIdealPitch();

vec3_t lastsimorg_10447;

extern cvar_t cl_smoothtime; // = { "cl_smoothtime", "0.4", FCVAR_ARCHIVE };
cvar_t cl_nosmooth = { "cl_nosmooth", "0" };
extern cvar_t cl_showerror; // = { "cl_showerror", "0" };

void CL_InitPrediction();

void CL_InitPrediction();

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
	if ((g_pcls.state == ca_connected || g_pcls.state == ca_uninitialized) &&
		g_pcls.signon == SIGNONS)
	{
		// first update is the final signon stage
		g_pcls.state = ca_active;

		if (fs_startup_timings.value != 0.0)
			PrintStartupTimings();

		{
			unsigned char* IPPtr = g_pcls.netchan.remote_address.ip;
			unsigned short port = g_pcls.netchan.remote_address.port;

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
	if (g_pcls.demoplayback)
	CL_StopPlayback();
	*/

	prevcareer = g_careerState;

	// CL_Disconnect(); - TODO: implement - ScriptedSnark

	if (prevcareer == CAREER_LOADING)
		g_careerState = CAREER_LOADING;

	StartLoadingProgressBar("Connecting", 12);
	SetLoadingProgressBarStatusText("#GameUI_EstablishingConnection");

	int num = Q_atoi(name); // In case it's an index.

	if ((num > 0) && (num <= num_servers) && !Q_strstr(g_pcls.servername, "."))
	{
		Q_strncpy(name, NET_AdrToString(cached_servers[num - 1].adr), sizeof(name));
		name[sizeof(name) - 1] = 0;
	}

	Q_memset(msg_buckets, 0, sizeof(msg_buckets));
	Q_memset(total_data, 0, sizeof(total_data));

	Q_strncpy(g_pcls.servername, name, sizeof(g_pcls.servername) - 1);
	g_pcls.servername[sizeof(g_pcls.servername) - 1] = 0;

	g_pcls.state = ca_connecting;
	g_pcls.connect_time = -99999;

	g_pcls.connect_retry = 0;

	g_pcls.passive = false;
	g_pcls.spectator = false;
	g_pcls.isVAC2Secure = false;

	g_pcls.GameServerSteamID = 0;

	g_pcls.build_num = 0;

	Q_memset(&g_GameServerAddress, 0, sizeof(netadr_t));
	g_pcls.challenge = 0;

	gfExtendedError = false;
	g_LastScreenUpdateTime = 0.0;

	if (Q_strnicmp(g_pcls.servername, "local", 5))
	{
		// allow remote
		NET_Config(true);
	}
}

void CL_Retry_f() // TODO: improve - ScriptedSnark
{
	char szCommand[260]; // [esp+1Ch] [ebp-110h] BYREF

	if (g_pcls.servername[0])
	{
		if (strchr(g_pcls.servername, 10) || strchr(g_pcls.servername, 59))
			Con_Printf("Invalid command separator in server name, refusing retry\n");
		else
		{
			if (g_pcls.passive)
				snprintf(szCommand, sizeof(szCommand), "listen %s\n", g_pcls.servername);
			else
				snprintf(szCommand, sizeof(szCommand), "connect %s\n", g_pcls.servername);

			Cbuf_AddText(szCommand);
			Con_Printf("Commencing connection retry to %s\n", g_pcls.servername);
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
	return Call_Function<void>(0x17850);
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
	NOT_IMPLEMENTED;

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
	if ((g_pcl.resourcesonhand.pNext != (resource_s*)0x0) &&
		(strSource = g_pcl.resourcesonhand.pNext, g_pcl.resourcesonhand.pNext != &g_pcl.resourcesonhand)) {
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
							g_pcl.sound_precache[iVar9] = psVar8;
							S_EndPrecaching();
							bVar3 = strSource->ucFlags;
							if ((g_pcl.sound_precache[strSource->nIndex] == (sfx_s*)0x0) && ((bVar3 & 1) != 0)) {
								COM_ExplainDisconnection
									(true, "Cannot continue without sound %s, disconnecting.", strSource);
								CL_Disconnect();
								return false;
							}
						}
						else {
							g_pcl.sound_precache[strSource->nIndex] = (sfx_s*)0x0;
							bVar3 = strSource->ucFlags;
						}
						break;
					case t_model:
						if ((g_pcl.model_precache_count <= strSource->nIndex) &&
							(g_pcl.model_precache_count = strSource->nIndex + 1, 0x200 < g_pcl.model_precache_count)) {
							g_pcl.model_precache_count = 0x200;
						}
						if (strSource->szFileName[0] == '*') goto LAB_0019e98c;
						if (fs_lazy_precache.value == 0.0) {
							iVar9 = strSource->nIndex;
						LAB_0019ea2d:
							pmVar4 = Mod_ForName(strSource->szFileName, false, true);
							g_pcl.model_precache[iVar9] = pmVar4;
							pmVar2 = g_pcl.model_precache[strSource->nIndex];
						}
						else {
							iVar7 = Q_strnicmp(strSource->szFileName, "maps", 4);
							iVar9 = strSource->nIndex;
							if (iVar7 == 0) goto LAB_0019ea2d;
							pmVar4 = Mod_FindName(true, strSource->szFileName);
							g_pcl.model_precache[iVar9] = pmVar4;
							pmVar2 = g_pcl.model_precache[strSource->nIndex];
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
						g_pcl.event_precache[iVar9].filename = pcVar5;
						iVar9 = strSource->nIndex;
						pbVar6 = COM_LoadFile(strSource->szFileName, 5, (int*)0x0);
						g_pcl.event_precache[iVar9].pszScript = (char*)pbVar6;
						if (g_pcl.event_precache[strSource->nIndex].pszScript != (char*)0x0) goto LAB_0019e98c;
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
		} while ((prVar1 != (resource_s*)0x0) && (strSource = prVar1, prVar1 != &g_pcl.resourcesonhand));
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

void CL_RedoPrediction(void)
{
	if (g_pcls.netchan.incoming_sequence == g_lastupdate_sequence)
		return;
	
	CL_PredictMove(true);
	return CL_CheckPredictionError();
}

void CL_GetTargetTime(float *target)
{
	*target = realtime;
}

void CL_PredictMove(qboolean repredicting)
{
	cmd_t *to_cmd = nullptr, *fromcmd = nullptr;
	local_state_t	*from = nullptr, *to = nullptr;
	frame_t *frame = NULL;
	float f;
	unsigned int i, stoppoint;

	if (g_pcls.state != ca_active || g_pcls.spectator)
		return;

	pmove = &g_clmove;

	CL_SetUpPlayerPrediction(false, false);

	if (m1.intermission || !m1.validsequence ||
		g_pcls.netchan.outgoing_sequence + 1 - g_pcls.netchan.incoming_acknowledged >= CL_UPDATE_MASK)
		return;

	VectorCopy(m1.viewangles, m1.simangles);

	if (g_pcls.demoplayback)
		return;

	int parsecountmod = m1.parsecountmod;
	frame = &m1.frames[parsecountmod];
	from = &m1.predicted_frames[parsecountmod];

	entity_state_t* players_state = &frame->playerstate[m1.playernum];

	from->playerstate = frame->playerstate[m1.playernum];

	from->client = frame->clientdata;

	CHECK_REQUIRED;
	auto a = sizeof(frame->weapondata);
	//Q_memcpy(m1.predicted_frames[parsecountmod].weapondata, frame->weapondata, 5632);
	memcpy(from->weapondata, frame->weapondata, sizeof(from->weapondata));

	if (frame->invalid)
		return;

	double pfElapsed = frame->time;

	fromcmd = &m1.commands[g_pcls.netchan.incoming_acknowledged & CL_UPDATE_MASK];

	m1.onground = -1;

	CL_PushPMStates();

	CL_SetSolidPlayers(m1.playernum);

	int random_seed;

	stoppoint = (repredicting) ? 0 : 1;
	frame_t* update_frame = nullptr;

	g_i = 1;

	for (i = 1; i < CL_UPDATE_MASK && g_pcls.netchan.incoming_acknowledged + i < g_pcls.netchan.outgoing_sequence + stoppoint; i++)
	{
		unsigned int current_command;
		unsigned int current_command_mod;
		qboolean		runfuncs;

		current_command = g_pcls.netchan.incoming_acknowledged + i;
		current_command_mod = current_command & CL_UPDATE_MASK;
		m1.resourcesonhand;
		to = &m1.predicted_frames[(m1.parsecountmod + i) & CL_UPDATE_MASK];
		to_cmd = &m1.commands[current_command_mod];
		runfuncs = (!repredicting && !to_cmd->processedfuncs);

		CL_RunUsercmd(from, to, &to_cmd->cmd, runfuncs, &pfElapsed, current_command);

		CHECK_REQUIRED;

		VectorCopy(to->playerstate.origin, m1.predicted_origins[current_command_mod]);

		to_cmd->processedfuncs = true;

		if (to_cmd->senttime >= realtime)
			break;

		from = to;

		fromcmd = to_cmd;

		g_i = i;
	}

	CL_PopPMStates();

	if (CL_UPDATE_MASK <= i)
		return;

	if (!to)
	{
		to = from;
		to_cmd = fromcmd;
	}

	if (to_cmd->senttime != fromcmd->senttime)
		f = (realtime - fromcmd->senttime) / (to_cmd->senttime - fromcmd->senttime);
	else
		f = 0.0;

	if (fabs(to->playerstate.origin[0] - from->playerstate.origin[0]) > 128.0
		|| fabs(to->playerstate.origin[1] - from->playerstate.origin[1]) > 128.0
		|| fabs(to->playerstate.origin[2] - from->playerstate.origin[2]) > 128.0)
	{
		VectorCopy(to->client.velocity, m1.simvel);
		VectorCopy(to->playerstate.origin, m1.simorg);
		VectorCopy(to->client.punchangle, m1.punchangle);
		VectorCopy(to->client.view_ofs, m1.viewheight);
	}
	else
	{

		//VectorLerp(from->playerstate.origin, f, to->playerstate.origin, cl.simorg);
		//VectorLerp(from->client.velocity, f, to->client.velocity, cl.simvel);
		//VectorLerp(from->client.punchangle, f, to->client.punchangle, cl.punchangle);

		m1.simorg[0] = from->playerstate.origin[0] + (to->playerstate.origin[0] - from->playerstate.origin[0]) * f;
		m1.simorg[1] = from->playerstate.origin[1] + (to->playerstate.origin[1] - from->playerstate.origin[1]) * f;
		m1.simorg[2] = from->playerstate.origin[2] + (to->playerstate.origin[2] - from->playerstate.origin[2]) * f;

		m1.simvel[0] = from->client.velocity[0] + (to->client.velocity[0] - from->client.velocity[0]) * f;
		m1.simvel[1] = from->client.velocity[1] + (to->client.velocity[1] - from->client.velocity[1]) * f;
		m1.simvel[2] = from->client.velocity[2] + (to->client.velocity[2] - from->client.velocity[2]) * f;

		m1.punchangle[0] = from->client.punchangle[0] + (to->client.punchangle[0] - from->client.punchangle[0]) * f;
		m1.punchangle[1] = from->client.punchangle[1] + (to->client.punchangle[1] - from->client.punchangle[1]) * f;
		m1.punchangle[2] = from->client.punchangle[2] + (to->client.punchangle[2] - from->client.punchangle[2]) * f;

		if (from->playerstate.usehull == to->playerstate.usehull)
		{
			for (int i = 0; i < 3; ++i)
			{
				m1.viewheight[i] = from->client.view_ofs[i] + (to->client.view_ofs[i] - i) * f;
			}
		}
		else
		{
			VectorCopy(to->client.view_ofs, m1.viewheight);
		}
	}

	if (to->client.flags & 0x200 != 0)
	{
		m1.onground = g_lastground;
		m1.waterlevel = to->client.waterlevel;
		m1.usehull = to->playerstate.usehull;
		m1.moving = false;
		m1.stats[2] = to->client.viewmodel;

		if (g_lastground > 0 && g_lastground < m1.num_entities)
		{
			cl_entity_t* ent = &cl_entities[g_lastground];

			vec3_t delta;
			delta[0] = ent->curstate.origin[0] - ent->prevstate.origin[0];
			delta[1] = ent->curstate.origin[1] - ent->prevstate.origin[1];
			delta[2] = 0.0;

			if (Length(delta) > (long double)(float)0.0)
			{
				m1.moving = 1;
			}

			if (m1.moving)
			{
				cl_correction_time = 0.0;
				VectorCopy(m1.simorg, lastsimorg_10447);

				CL_SetIdealPitch();
				return;
			}
		}
	}
	else
	{
		m1.onground = -1;
		m1.waterlevel = to->client.waterlevel;
		m1.usehull = to->playerstate.usehull;
		m1.stats[2] = to->client.viewmodel;
		m1.moving = false;
	}

	if (cl_correction_time > 0.0 && cl_nosmooth.value == 0.0 && cl_smoothtime.value != 0.0)
	{
		if (!repredicting)
			cl_correction_time = cl_correction_time - host_frametime;
		if (cl_smoothtime.value <= 0.0)
			Cvar_DirectSet(&cl_smoothtime, "0.1");

		float new_correction_time = 0.0;
		if (cl_correction_time >= 0.0)
			new_correction_time = cl_correction_time;

		if (cl_smoothtime.value <= new_correction_time)
			new_correction_time = cl_smoothtime.value;

		cl_correction_time = new_correction_time;

		// Compute backward interpolation fraction along full correction
		float frac = 1.0f - cl_correction_time / cl_smoothtime.value;

		vec3_t delta;
		VectorSubtract(m1.simorg, lastsimorg_10447, delta);

		VectorScale(delta, frac, delta);

		VectorAdd(lastsimorg_10447, delta, m1.simorg);
	}

	VectorCopy(m1.simorg, lastsimorg_10447);

	CL_SetIdealPitch();
}

void CL_ReadPackets(void)
{
	if (g_pcls.state == ca_dedicated)
		return;

	m1.oldtime = m1.time;
	m1.time += host_frametime;

	qboolean demoplayback = g_pcls.demoplayback;
	cactive_t state = g_pcls.state;

	int i = 0;
	while (demoplayback)
	{
		if (++i > 250 && !CL_ReadDemoMessage())
			break;

		if (*net_message.data == -1)
		{
			CL_ConnectionlessPacket();
			demoplayback = g_pcls.demoplayback;
			continue;
		}

		if (g_pcls.passive)
		{
			state = g_pcls.state;
			demoplayback = g_pcls.demoplayback;
			if (g_pcls.state == ca_connecting)
			{
				g_pcls.state = ca_connected;
			}
		}
		else
		{
			if (!NET_CompareAdr(net_from, g_pcls.netchan.remote_address))
				continue;

			state = g_pcls.state;
			demoplayback = g_pcls.demoplayback;

			if (state - 1 <= 1)
				continue;
		}

		if (g_pcls.demoplayback)
		{
			MSG_BeginReading();
			CL_ParseServerMessage(true);
			demoplayback = g_pcls.demoplayback;
			continue;
		}
		if (net_message.cursize > 7)
		{
			if (Netchan_Process(&g_pcls.netchan))
				CL_ParseServerMessage(true);

			demoplayback = g_pcls.demoplayback;
			continue;
		}

		const char* addrFrom = NET_AdrToString(net_from);
		Con_Printf("CL_ReadPackets:  undersized packet %i bytes from %s\n", net_message.cursize, addrFrom);
	}
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

	if (g_pcl.num_entities && (in = cl_entities->model) != 0)
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
		ent = &g_pcl.frames[i].packet_entities;
		if (ent->entities)
		{
			Mem_Free(ent->entities);
		}
		ent->entities = NULL;
	}

	Q_memset(g_pcl.frames, 0, sizeof(frame_t) * client);
	*/
}

void CL_Shutdown()
{
	NOT_IMPLEMENTED;

	TextMessageShutdown();
}

void CL_Init()
{
	 const char * name = Steam_GetCommunityName();
	if (!name)
		name = "unknown";

	Info_SetValueForKey(g_pcls.userinfo, "name", name, 256);
	Info_SetValueForKey(g_pcls.userinfo, "topcolor", "0", 256);
	Info_SetValueForKey(g_pcls.userinfo, "bottomcolor", "0", 256);
	Info_SetValueForKey(g_pcls.userinfo, "rate", "2500", 256);
	Info_SetValueForKey(g_pcls.userinfo, "cl_updaterate", "20", 256);
	Info_SetValueForKey(g_pcls.userinfo, "cl_lw", "1", 256);
	Info_SetValueForKey(g_pcls.userinfo, "cl_lc", "1", 256);

	Net_APIInit();
	CL_InitTEnts();
	CL_InitExtrap();
	TextMessageInit();
	Sequence_Init();

	Cvar_RegisterVariable(&cl_weaponlistfix);
	Cvar_RegisterVariable(&cl_fixtimerate);
	Cvar_RegisterVariable(&cl_showmessages);
	Cvar_RegisterVariable(&cl_name);
	Cvar_RegisterVariable(&password);
	Cvar_RegisterVariable(&team);
	Cvar_RegisterVariable(&cl_model);
	Cvar_RegisterVariable(&skin);
	Cvar_RegisterVariable(&topcolor);
	Cvar_RegisterVariable(&bottomcolor);
	Cvar_RegisterVariable(&rate);
	Cvar_RegisterVariable(&cl_updaterate);
	Cvar_RegisterVariable(&cl_lw);
	Cvar_RegisterVariable(&cl_lc);
	Cvar_RegisterVariable(&cl_dlmax);
	Cvar_RegisterVariable(&fs_startup_timings);
	Cvar_RegisterVariable(&fs_lazy_precache);
	Cvar_RegisterVariable(&fs_precache_timings);
	Cvar_RegisterVariable(&fs_perf_warnings);
	Cvar_RegisterVariable(&cl_clockreset);
	Cvar_RegisterVariable(&cl_showevents);
	Cvar_RegisterVariable(&cl_himodels);
	Cvar_RegisterVariable(&cl_gaitestimation);
	Cvar_RegisterVariable(&cl_idealpitchscale);
	Cvar_RegisterVariable(&cl_resend);
	Cvar_RegisterVariable(&cl_timeout);
	Cvar_RegisterVariable(&cl_cmdbackup);
	Cvar_RegisterVariable(&cl_shownet);
	Cvar_RegisterVariable(&rcon_address);
	Cvar_RegisterVariable(&rcon_port);
	Cvar_RegisterVariable(&cl_solid_players);
	Cvar_RegisterVariable(&cl_slisttimeout);
	Cvar_RegisterVariable(&cl_download_ingame);
	Cvar_RegisterVariable(&cl_allow_download);
	Cvar_RegisterVariable(&cl_allow_upload);
	Cvar_RegisterVariable(&cl_gamegauge);
	Cvar_RegisterVariable(&cl_cmdrate);
	Cvar_RegisterVariable(&cl_showfps);
	Cvar_RegisterVariable(&cl_needinstanced);
	Cvar_RegisterVariable(&dev_overview);
	Cvar_RegisterVariable(&cl_logofile);
	Cvar_RegisterVariable(&cl_logocolor);
	Cvar_RegisterVariable(&cl_mousegrab);
	Cvar_RegisterVariable(&m_rawinput);
	Cvar_RegisterVariable(&cl_filterstuffcmd);
	if (COM_CheckParm("-nomousegrab"))
		Cvar_Set("cl_mousegrab", "0");

	ClientDLL_HudInit();
	Cmd_AddCommand((char *)"gamedir", CL_GameDir_f);
	Cmd_AddCommandWithFlags((char *)"connect", CL_Connect_f, 8);
	Cmd_AddCommand("fullserverinfo", CL_FullServerinfo_f);
	Cmd_AddCommandWithFlags("retry", CL_Retry_f, 8);
	Cmd_AddCommand("disconnect", CL_Disconnect_f);
	Cmd_AddCommand("record", CL_Record_f);
	Cmd_AddCommand("stop", CL_Stop_f);
	Cmd_AddCommand("playdemo", CL_PlayDemo_f);
	Cmd_AddCommand("viewdemo", CL_ViewDemo_f);
	Cmd_AddCommand("timedemo", CL_TimeDemo_f);
	Cmd_AddCommand((char *)"gg", CL_GameGauge_f);
	Cmd_AddCommand("listdemo", CL_ListDemo_f);
	Cmd_AddCommand("appenddemo", CL_AppendDemo_f);
	Cmd_AddCommandWithFlags("removedemo", CL_RemoveDemo_f, 16);
	Cmd_AddCommand("swapdemo", CL_SwapDemo_f);

	Cmd_AddCommand("setdemoinfo", CL_SetDemoInfo_f);
	Cmd_AddCommand("listen", CL_Listen_f);
	Cmd_AddCommand("commentator", CL_Commentator_f);
	Cmd_AddCommand("waveplaylen", CL_WavePlayLen_f);
	Cmd_AddCommand("snapshot", CL_TakeSnapshot_f);
	Cmd_AddCommand("startmovie", CL_StartMovie_f);
	Cmd_AddCommand("endmovie", CL_EndMovie_f);
	Cmd_AddCommand("entities", CL_PrintEntities_f);
	Cmd_AddCommandWithFlags("rcon", CL_Rcon_f, 8);
	Cmd_AddCommand("cl_messages", CL_DumpMessageLoad_f);
	Cmd_AddCommand("pingservers", CL_PingServers_f);
	Cmd_AddCommand("slist", CL_Slist_f);
	Cmd_AddCommand("list", CL_ListCachedServers_f);
	Cmd_AddCommand("upload", CL_BeginUpload_f);

	CL_InitPrediction();
	Cmd_AddCommand("httpstop", CL_HTTPCancel_f);
	Cmd_AddCommand("spec_pos", CL_SpecPos_f);
	g_pcls.datagram.data = g_pcls.datagram_buf;
	g_pcls.datagram.cursize = 0;
	g_pcls.datagram.buffername = "g_pcls.datagram";
	g_pcls.isVAC2Secure = false;
	g_pcls.GameServerSteamID = 0;
	g_pcls.datagram.maxsize = 4000;
	g_pcls.build_num = 0;
	g_pcls.datagram.flags = 0;

	Q_memset(&m1, 0, 1767016);
	m1.resourcesneeded.pNext = &m1.resourcesneeded;
	m1.resourcesonhand.pPrev = &m1.resourcesonhand;
	m1.resourcesneeded.pPrev = &m1.resourcesneeded;
	m1.resourcesonhand.pNext = &m1.resourcesonhand;

	DELTA_RegisterDescription("clientdata_t");
	DELTA_RegisterDescription("entity_state_t");
	DELTA_RegisterDescription("entity_state_player_t");
	DELTA_RegisterDescription("custom_entity_state_t");
	DELTA_RegisterDescription("usercmd_t");
	DELTA_RegisterDescription("weapon_data_t");
	DELTA_RegisterDescription("event_t");
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
		while (g_pcl.time <= (long double)dest->die)
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
	return Call_Function<model_t*, int>(0x1A3E0, index);

	NOT_IMPLEMENTED;
	
	/*
	if (index >= MAX_MODELS)
		return nullptr;

	model_t* model = g_pcl.model_precache[index]; // ebx

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
	return g_pcls.netchan.remote_address.type == NA_LOOPBACK;
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
		if (Q_atoi(Cmd_Argv(1)) == 2 && g_pcls.state == ca_active)
		{
			origin[0] = g_pcl.frames[g_pcl.parsecountmod].playerstate[g_pcl.playernum].origin[0];
			origin[1] = g_pcl.frames[g_pcl.parsecountmod].playerstate[g_pcl.playernum].origin[1];
			origin[2] = g_pcl.frames[g_pcl.parsecountmod].playerstate[g_pcl.playernum].origin[2];

			angles[0] = g_pcl.frames[g_pcl.parsecountmod].playerstate[g_pcl.playernum].angles[0];
			angles[1] = g_pcl.frames[g_pcl.parsecountmod].playerstate[g_pcl.playernum].angles[1];
			angles[2] = g_pcl.frames[g_pcl.parsecountmod].playerstate[g_pcl.playernum].angles[2];
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
	for (int i = 1; i < ARRAYSIZE(g_pcl.event_precache) && g_pcl.event_precache[i].pszScript; ++i)
	{
		Mem_Free(const_cast<char*>(g_pcl.event_precache[i].pszScript));
		Mem_Free(const_cast<char*>(g_pcl.event_precache[i].filename));

		Q_memset(&g_pcl.event_precache[i], 0, sizeof(g_pcl.event_precache[i]));
	}
}

void CL_ClearClientState()
{
	NOT_IMPLEMENTED;
	
	/*
	for (int i = 0; i < CL_UPDATE_BACKUP; ++i)
	{
		if (g_pcl.frames[i].packet_entities.entities)
		{
			Mem_Free(g_pcl.frames[i].packet_entities.entities);
		}

		g_pcl.frames[i].packet_entities.entities = nullptr;
		g_pcl.frames[i].packet_entities.num_entities = 0;
	}

	CL_ClearResourceLists();

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		COM_ClearCustomizationList(&g_pcl.players[i].customdata, false);
	}

	CL_ClearCaches();

	Q_memset(&cl, 0, sizeof(cl));

	g_pcl.resourcesneeded.pPrev = &g_pcl.resourcesneeded;
	g_pcl.resourcesneeded.pNext = &g_pcl.resourcesneeded;
	g_pcl.resourcesonhand.pPrev = &g_pcl.resourcesonhand;
	g_pcl.resourcesonhand.pNext = &g_pcl.resourcesonhand;

	CL_CreateResourceList();
	*/
}

void CL_ClearState(bool bQuiet)
{
	if (!Host_IsServerActive())
		Host_ClearMemory(bQuiet);

	CL_ClearClientState();

	// TODO: implement - Solokiller
	SZ_Clear(&g_pcls.netchan.message);

	// clear other arrays
	Q_memset(cl_efrags, 0, sizeof(cl_efrags));
	Q_memset(cl_dlights, 0, sizeof(cl_dlights));
	Q_memset(cl_elights, 0, sizeof(cl_elights));
	Q_memset(cl_lightstyle, 0, sizeof(cl_lightstyle));

	CL_TempEntInit();

	//
	// allocate the efrags and chain together into a free list
	//
	g_pcl.free_efrags = cl_efrags;

	int i;
	for (i = 0; i < MAX_EFRAGS - 1; ++i)
	{
		g_pcl.free_efrags[i].entnext = &g_pcl.free_efrags[i + 1];
	}

	g_pcl.free_efrags[i].entnext = nullptr;
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
	if (g_pcls.state <= ca_connecting)
		return;
	
	NOT_IMPLEMENTED;
	
	unsigned char data[2048];

	if (g_pcls.state != ca_connected)
	{
		CL_ComputePacketLoss();

		int outgoing_sequence = g_pcls.netchan.outgoing_sequence;
		
		sizebuf_t buf;
		buf.buffername = "Client Move";
		buf.maxsize = 2048;
		buf.cursize = 0;
		buf.flags = 0;
		buf.data = data;

		float cmdbackup = cl_cmdbackup.value;

		cmd_t* moveCommand;
		moveCommand = &m1.commands[outgoing_sequence & CL_UPDATE_MASK];
		moveCommand->senttime = realtime;
		moveCommand->receivedtime = -1.0;
		moveCommand->processedfuncs = false;
		moveCommand->heldback = false;
		moveCommand->sendsize = 0;

		auto x = sizeof(cmd_t::cmd);
		Q_memset(moveCommand, 0, 52);

		bool active = g_pcls.signon == 2;

		CL_SetSolidEntities();

		CL_PushPMStates();

		CL_SetSolidPlayers(m1.playernum);

		double dest = host_frametime;

		ClientDLL_CreateMove(dest, &moveCommand->cmd, active);
		CL_PopPMStates();
		CL_ComputeClientInterpolationAmount(&moveCommand->cmd);
		moveCommand->cmd.lightlevel = m1.light_level;
		int count = 100;
	}
	
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

void CL_GameDir_f(void)
{
	NOT_IMPLEMENTED;
}

void CL_FullServerinfo_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_Record_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_PlayDemo_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_ViewDemo_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_TimeDemo_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_GameGauge_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_ListDemo_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_AppendDemo_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_RemoveDemo_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_SwapDemo_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_SetDemoInfo_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_Listen_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_Commentator_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_WavePlayLen_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_StartMovie_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_EndMovie_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_PrintEntities_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_Rcon_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_DumpMessageLoad_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_PingServers_f(void)
{
	NOT_IMPLEMENTED;
}

void CL_Slist_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_ListCachedServers_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_BeginUpload_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_HTTPCancel_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_SpecPos_f(void)
{
	NOT_IMPLEMENTED;

}

void CL_InitPrediction()
{
	Cvar_RegisterVariable(&cl_smoothtime);
	Cvar_RegisterVariable(&cl_nosmooth);
	Cvar_RegisterVariable(&cl_showerror);
}

qboolean CL_ReadDemoMessage()
{
	if (!DemoPlayer_IsActive())
		return CL_ReadDemoMessage_OLD();

	int cursize = DemoPlayer_ReadDemoMessage(net_message.data, net_message.maxsize);
	net_message.cursize = cursize;
	if (cursize)
		return false;

	DemoPlayer_ReadNetchanState(
		&g_pcls.netchan.incoming_sequence,
		&g_pcls.netchan.incoming_acknowledged,
		&g_pcls.netchan.incoming_reliable_acknowledged,
		&g_pcls.netchan.incoming_reliable_sequence,
		&g_pcls.netchan.outgoing_sequence,
		&g_pcls.netchan.reliable_sequence,
		&g_pcls.netchan.last_reliable_sequence);
	return true;
}

int CL_ReadDemoMessage_OLD()
{
	NOT_IMPLEMENTED;
	return 0;
}

void CL_ParseServerMessage(qboolean normal_message)
{
	NOT_IMPLEMENTED;
}

void CL_SetIdealPitch()
{
	NOT_IMPLEMENTED;
}

void CL_CheckForResend(void)
{
	if (g_pcls.state == ca_disconnected && Host_IsServerActive())
	{
		g_pcls.state = ca_connecting;

		gfExtendedError = false;

		Q_strncpy(g_pcls.servername, "localhost", 259);
		g_pcls.servername[259] = 0;

		g_pcls.authprotocol = 3;

		CL_SendConnectPacket();
		return;
	}

	if (g_pcls.state == ca_connecting)
	{
		if (g_pcls.passive)
		{
			if (realtime - g_pcls.connect_time <= 30.0)
				return;

			if (g_pcls.challenge)
			{
				Con_Printf("Multicast failed. Falling back to unicast...\n");
				g_pcls.passive = false;
				NET_LeaveGroup(g_pcls.netchan.sock, g_pcls.connect_stream);
				CL_SendConnectPacket();
			}
			else
			{
				COM_ExplainDisconnection(true, "Joining multicast group failed after %.0f seconds.", 30.0);

				g_pcls.connect_time = -99999;

				g_pcls.connect_retry = 0;
				g_pcls.state = ca_disconnected;
				Q_memset(&g_GameServerAddress, 0, 20);
				g_pcls.passive = false;
				netadr_t srca;
				memcpy(&srca, &g_pcls.connect_stream, sizeof(netadr_t));
				NET_LeaveGroup(g_pcls.netchan.sock, srca);
				return;
			}
		}

		float src = cl_resend.value;
		if (src < 1.5)
		{
			src = 1.5;
		}
		else if (src > 20)
		{
			src = 20.0;
		}

		Cvar_SetValue("cl_resend", src);

		char szServerName[260];
		netadr_t adr;

		if (src <= realtime - g_pcls.connect_time)
		{
			Q_strncpy(szServerName, g_pcls.servername, 260);
			szServerName[259] = 0;

			if (!Q_stricmp(g_pcls.servername, "local"))
				snprintf(szServerName, 260, "%s", "localhost");

			if (NET_StringToAdr(szServerName, &adr))
			{
				if (!adr.port)
				{
					adr.port = BigShort(Q_atoi("27015"));
				}
				if (g_pcls.connect_retry > 3)
				{
					COM_ExplainDisconnection(true, "#GameUI_CouldNotContactGameServer");
					g_pcls.connect_time = -99999;
					g_pcls.connect_retry = 0;
					g_pcls.state = ca_disconnected;
					Q_memset(&g_GameServerAddress, 0, 20);
					CL_Disconnect();
				}
				else
				{
					g_pcls.connect_time = realtime;
					if (Q_stricmp(szServerName, "localhost"))
					{
						if (g_pcls.connect_retry)
						{
							if (g_pcls.connect_retry == 2)
							{
								SetLoadingProgressBarStatusText("#GameUI_RetryingConnectionToServer2");
							}
							else if (g_pcls.connect_retry == 3)
							{
								SetLoadingProgressBarStatusText("#GameUI_RetryingConnectionToServer3");
							}
							else
							{
								SetLoadingProgressBarStatusText("#GameUI_RetryingConnectionToServer");
							}

							Con_Printf("Retrying %s...\n", szServerName);
						}
						else
						{
							Con_Printf("Connecting to %s...\n", szServerName);
						}
					}
					++g_pcls.connect_retry;
					char data[2048];
					snprintf(data, 2048, "%c%c%c%cgetchallenge steam\n", 255, 255, 255, 255);
					g_GameServerAddress = adr;
					NET_SendPacket(NS_CLIENT, Q_strlen(data), data, adr);
				}
			}
			else
			{
				Con_Printf("Bad server address\n");
				g_pcls.state = ca_disconnected;
				Q_memset(&g_GameServerAddress, 0, 20);
			}
		}
	}
}

void CL_GGSpeeds(float flCurTime)
{
	NOT_IMPLEMENTED;
}
