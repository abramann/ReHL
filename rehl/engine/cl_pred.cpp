#include "precompiled.h"


int pushed;
int oldphysent;
int oldvisent;
int cl_correction_time;
cvar_t cl_showerror = { "cl_showerror", "0" };
cvar_t cl_smoothtime = { "cl_smoothtime", "0" };
int g_lastupdate_sequence = INT_MAX;

void CL_PushPMStates()
{
	if (pushed)
	{
		Con_Printf("CL_PushPMStates called with pushed stack\n");
	}
	else
	{
		oldphysent = pmove->numphysent;
		oldvisent = pmove->numvisent;
		pushed = 1;
	}
}

void CL_PopPMStates()
{
	playermove_t* plmove; // edx

	if (!pushed)
	{
		Con_Printf("CL_PopPMStates called without stack\n");
		return;
	}

	if (pushed)
	{
		plmove = pmove;
		pushed--;
		pmove->numphysent = oldphysent;
		plmove->numvisent = oldvisent;
	}
}

void CL_RunUsercmd(local_state_t* from, local_state_t* to, usercmd_t* u, bool runfuncs, double* pfElapsed, unsigned int random_seed)
{
	NOT_IMPLEMENTED;
}

void CL_SetLastUpdate(void)
{
	g_lastupdate_sequence = g_pcls.netchan.incoming_sequence;
	return;
}

/* WARNING: Removing unreachable block (ram,0x001927f2) */

void CL_CheckPredictionError(void)
{
	NOT_IMPLEMENTED;
	/*
	static int	pos = 0;
	int cmd = g_pcl.parsecountmod;
	unsigned int frame = CL_UPDATE_MASK & g_pcls.netchan.incoming_acknowledged;
	float dist;
	vec3_t delta;

	VectorSubtract(g_pcl.frames[cmd].playerstate[cl.playernum].origin, cl.predicted_origins[frame], delta);
	dist = Length(delta);
	if (64.0 < dist) 
	{
		VectorClear(cl.prediction_error);
	}
	else {
		if ((cl_showerror.value != 0.0) && (0.5 < dist)) {
			pos = pos + 1;
			Con_NPrintf((pos & 3U) + 10, "pred. error %.3f units", (double)dist);
		}
		VectorCopy(cl.frames[cmd].playerstate[cl.playernum].origin, cl.predicted_origins[frame]);

		// save for error interpolation
		VectorCopy(delta, cl.prediction_error);
		if ((0.25 < dist) && (cl.maxclients != 1)) {
			cl_correction_time = (double)cl_smoothtime.value;
			return;
		}
	}
	*/
}
