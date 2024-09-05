#include "precompiled.h"
#include "dlls\vector.h"

#ifdef SHARED_GAME_DATA
cvar_t * sp_chase_back = ADDRESS_OF_DATA(cvar_t *, 0xCDC1);
cvar_t & chase_back = *sp_chase_back; 

cvar_t * sp_chase_up = ADDRESS_OF_DATA(cvar_t *, 0xCDCB);
cvar_t & chase_up = *sp_chase_up;

cvar_t * sp_chase_right = ADDRESS_OF_DATA(cvar_t *, 0xCDD5);
cvar_t & chase_right = *sp_chase_right;

cvar_t * sp_chase_active = ADDRESS_OF_DATA(cvar_t *, 0xCDDF);
cvar_t & chase_active = *sp_chase_active;

refdef_t * sp_r_refdef = ADDRESS_OF_DATA(refdef_t *, 0x4635A);
refdef_t & r_refdef = *sp_r_refdef; 

#else
// TODO: why would this be a SP only cvar? - Solokiller
cvar_t chase_back = { "chase_back", "100", FCVAR_SPONLY };
cvar_t chase_up = { "chase_up", "16", FCVAR_SPONLY };
cvar_t chase_right = { "chase_right", "0", FCVAR_SPONLY };
cvar_t chase_active = { "chase_active", "0", FCVAR_SPONLY };
refdef_t r_refdef = {};
#endif

vec3_t chase_pos = {};
vec3_t chase_angles = {};

vec3_t chase_dest = {};
vec3_t chase_dest_angles = {};

void Chase_Init()
{
	Cvar_RegisterVariable(&chase_back);
	Cvar_RegisterVariable(&chase_up);
	Cvar_RegisterVariable(&chase_right);
	Cvar_RegisterVariable(&chase_active);
}

void Chase_Reset()
{
	// Nothing
}

void TraceLine(vec_t* start, vec_t* end, vec_t* impact)
{
	trace_t trace;

	Q_memset(&trace, 0, sizeof(trace));

	SV_RecursiveHullCheck(g_pcl.worldmodel->hulls, 0, 0.0, 1.0, start, end, &trace);

	impact[0] = trace.endpos[0];
	impact[1] = trace.endpos[1];
	impact[2] = trace.endpos[2];
}

void Chase_Update()
{
	int i;
	float dist;
	vec3_t forward, up, right;
	vec3_t dest, stop;


	// if can't see player, reset
	AngleVectors(g_pcl.viewangles, forward, right, up);

	// calc exact destination
	for (i = 0; i < 3; i++)
		chase_dest[i] = r_refdef.vieworg[i] - forward[i] * chase_back.value - right[i] * chase_right.value;
	chase_dest[2] = r_refdef.vieworg[2] + chase_up.value;

	// find the spot the player is looking at
	// TODO: 4096 limit - Solokiller
	VectorMA(r_refdef.vieworg, 4096, forward, dest);
	TraceLine(r_refdef.vieworg, dest, stop);

	// calculate pitch to look at the same spot from camera
	VectorSubtract(stop, r_refdef.vieworg, stop);
	dist = DotProduct(stop, forward);
	if (dist < 1)
		dist = 1;
	r_refdef.viewangles[PITCH] = -atan(stop[2] / dist) / M_PI * 180;

	// move towards destination
	VectorCopy(chase_dest, r_refdef.vieworg);
}
