#include "precompiled.h"


event_api_t eventapi =
{
	EVENT_API_VERSION,
	&EV_PlaySound,
	&EV_StopSound,
	&EV_FindModelIndex,
	&EV_IsLocal,
	&EV_LocalPlayerDucking,
	&EV_LocalPlayerViewheight,
	&EV_LocalPlayerBounds,
	&EV_IndexFromTrace,
	&EV_GetPhysent,
	&EV_SetUpPlayerPrediction,
	&CL_PushPMStates,
	&CL_PopPMStates,
	&CL_SetSolidPlayers,
	&EV_SetTraceHull,
	&EV_PlayerTrace,
	&EV_WeaponAnimation,
	&EV_PrecacheEvent,
	&EV_PlaybackEvent,
	&PM_CL_TraceTexture,
	&S_StopSound,
	&EV_KillEvents };

void EV_PlaySound(int ent, float* origin, int channel, const char* sample, float volume, float attenuation, int fFlags, int pitch)
{
	NOT_IMPLEMENTED;
}

void EV_StopSound(int ent, int channel, const char* sample)
{
	EV_PlaySound(ent, vec3_origin, channel, sample, 0.0, 0.0, 32, 100);
}

int EV_FindModelIndex(const char* pmodel)
{
	for (int i = 1; i < g_pcl.model_precache_count; ++i)
	{
		if (g_pcl.model_precache[i] && !Q_stricmp(pmodel, g_pcl.model_precache[i]->name))
		{
			return i;
		}
	}

	return 0;
}

int EV_IsLocal(int playernum)
{
	return g_pcl.playernum == playernum;
}

int EV_LocalPlayerDucking()
{
	NOT_IMPLEMENTED;

	// TODO: define constant - Solokiller
	return g_pcl.usehull == 1;
}

void EV_LocalPlayerViewheight(float* viewheight)
{
	viewheight[0] = g_pcl.viewheight[0];
	viewheight[1] = g_pcl.viewheight[1];
	viewheight[2] = g_pcl.viewheight[2];
}

void EV_LocalPlayerBounds(int hull, float* mins, float* maxs)
{
	if (mins)
	{
		mins[0] = player_mins[hull][0];
		mins[1] = player_mins[hull][1];
		mins[2] = player_mins[hull][2];
	}

	if (maxs)
	{
		maxs[0] = player_maxs[hull][0];
		maxs[1] = player_maxs[hull][1];
		maxs[2] = player_maxs[hull][2];
	}
}

int EV_IndexFromTrace(pmtrace_t* pTrace)
{
	if (pTrace)
		return pmove->physents[pTrace->ent].info;

	return 0;
}

physent_t* EV_GetPhysent(int idx)
{
	if (idx >= 0 && idx < pmove->numphysent)
		return &pmove->physents[idx];

	return nullptr;
}

void EV_SetUpPlayerPrediction(int dopred, int bIncludeLocalClient)
{
	NOT_IMPLEMENTED;
	//CL_SetUpPlayerPrediction(dopred != 0, bIncludeLocalClient != 0);
}

void EV_SetTraceHull(int hull)
{
	pmove->usehull = hull;
}

void EV_PlayerTrace(float* start, float* end, int traceFlags, int ignore_pe, pmtrace_t* tr)
{
	*tr = PM_PlayerTrace(start, end, traceFlags, ignore_pe);
}

void EV_WeaponAnimation(int sequence, int body)
{
	g_pcl.weaponstarttime = 0;
	g_pcl.weaponsequence = sequence;
	g_pcl.viewent.curstate.body = body;
}

unsigned short EV_PrecacheEvent(int type, const char* psz)
{
	for (int i = 1; g_pcl.event_precache[i].filename; ++i)
	{
		if (!Q_stricmp(g_pcl.event_precache[i].filename, psz))
			return i;
	}

	return 0;
}

void EV_PlaybackEvent(int flags, const edict_t* pInvoker, unsigned short eventindex, float delay, float* origin, float* angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2)
{
	NOT_IMPLEMENTED;

	/*
	if (!(flags & FEV_SERVER))
	{
		event_args_t event;
		Q_memset(&event, 0, sizeof(event));

		event.entindex = flags;

		event.origin[0] = origin[0];
		event.origin[1] = origin[1];
		event.origin[2] = origin[2];

		event.angles[0] = angles[0];
		event.angles[1] = angles[1];
		event.angles[2] = angles[2];

		event.fparam1 = fparam1;
		event.fparam2 = fparam2;
		event.iparam1 = iparam1;
		event.iparam2 = iparam2;
		event.bparam1 = bparam1;
		event.bparam2 = bparam2;

		CL_QueueEvent(flags, eventindex, delay, &event);
	}
	*/
}

void EV_KillEvents(int entnum, const char* eventname)
{
	NOT_IMPLEMENTED;
	
	/*
	for (int i = 0; i < ARRAYSIZE(g_pcl.events.ei); ++i)
	{
		auto& event = g_pcl.events.ei[i];

		if (event.index != 0 &&
			event.args.entindex == entnum &&
			!Q_stricmp(eventname, g_pcl.event_precache[event.index].filename))
		{
			CL_ResetEvent(&event);
		}
	}
	*/
}
