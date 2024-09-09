#include "precompiled.h"

#include "quakedef.h"
#include "client.h"
#include "cl_spectator.h"
#include "pm_movevars.h"
#include "pmove.h"

overviewInfo_t gDevOverview = {};
local_state_t spectatorState = {};

VVAR(cvar_t, dev_overview, 0x1A018, { "dev_overview", "0" });

int CL_IsSpectateOnly()
{
	g_engdstAddrs.IsSpectateOnly();

	return g_pcls.spectator;
}

qboolean CL_IsDevOverviewMode()
{
	if (dev_overview.value == 0)
		return FALSE;
	if (sv_cheats.value == 0)
		return g_pcls.spectator;
	return true;
}

void CL_CalculateDevOverviewParameters()
{
	NOT_IMPLEMENTED;
}

void CL_InitSpectator()
{
	NOT_IMPLEMENTED;
}

bool CL_AddEntityToPhysList(int entIndex)
{
	NOT_IMPLEMENTED;
	return false;
}

void CL_MoveSpectatorCamera()
{
	if (g_pcls.state != ca_active && !g_pcls.spectator)
		return;

	double time = g_pcl.time;

	CL_SetUpPlayerPrediction(false, true);
	CL_SetSolidPlayers(g_pcl.playernum);
	CL_RunUsercmd(&spectatorState, &spectatorState, &g_pcl.cmd, true, &time, 100.0 * time);

	VectorCopy(spectatorState.client.velocity, g_pcl.simvel);
	VectorCopy(spectatorState.playerstate.origin, g_pcl.simorg);
	VectorCopy(spectatorState.client.punchangle, g_pcl.punchangle);
	VectorCopy(spectatorState.client.velocity, g_pcl.simvel);
	VectorCopy(spectatorState.client.view_ofs, g_pcl.viewheight);
}

void CL_SetDevOverView(refdef_t* refdef)
{
	NOT_IMPLEMENTED;
}
