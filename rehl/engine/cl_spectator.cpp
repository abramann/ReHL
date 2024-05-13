#include "precompiled.h"

#include "quakedef.h"
#include "client.h"
#include "cl_spectator.h"
#include "pm_movevars.h"
#include "pmove.h"

overviewInfo_t gDevOverview = {};
local_state_t spectatorState = {};

cvar_t dev_overview = { "dev_overview", "0" };

int CL_IsSpectateOnly()
{
	g_engdstAddrs.IsSpectateOnly();

	return g_pcls.spectator;
}

bool CL_IsDevOverviewMode()
{
	NOT_IMPLEMENTED;
	return false;
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

	double time = m1.time;

	CL_SetUpPlayerPrediction(false, true);
	CL_SetSolidPlayers(m1.playernum);
	CL_RunUsercmd(&spectatorState, &spectatorState, &m1.cmd, true, &time, 100.0 * time);

	VectorCopy(spectatorState.client.velocity, m1.simvel);
	VectorCopy(spectatorState.playerstate.origin, m1.simorg);
	VectorCopy(spectatorState.client.punchangle, m1.punchangle);
	VectorCopy(spectatorState.client.velocity, m1.simvel);
	VectorCopy(spectatorState.client.view_ofs, m1.viewheight);
}

void CL_SetDevOverView(refdef_t* refdef)
{
	NOT_IMPLEMENTED;
}
