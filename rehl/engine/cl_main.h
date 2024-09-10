#pragma once

#include "model.h"
#include "cl_tent.h"
#include "quakedef.h"
#include "cvar.h"
#include "ref_params.h"

struct lightstyle_t
{
	int length;
	char map[MAX_STYLESTRING];
};

struct startup_timing_t
{
	const char* name;
	float time;
};

enum
{
	NL_PRESENT = 0,
	NL_NEEDS_LOADED,
	NL_UNREFERENCED // this model can be freed after sequence precaching is done
};

const int MAX_STARTUP_TIMINGS = 32;

EXTERN_VAR(qboolean, g_bCS_CZ_Flags_Initialized);	
EXTERN_VAR(qboolean, g_bIsCStrike);
EXTERN_VAR(qboolean, g_bIsCZero);
EXTERN_VAR(qboolean, g_bIsCZeroRitual);
EXTERN_VAR(qboolean, g_bIsTerrorStrike);
EXTERN_VAR(qboolean, g_bIsTFC);
EXTERN_VAR(qboolean, g_bIsHL1);
EXTERN_VAR(cvar_t, fs_perf_warnings);
extern cl_entity_t* cl_entities;

extern efrag_t cl_efrags[MAX_EFRAGS];
extern dlight_t cl_dlights[MAX_DLIGHTS];
extern dlight_t cl_elights[MAX_ELIGHTS];
EXTERN_ARRAY(lightstyle_t, cl_lightstyle, [MAX_LIGHTSTYLES]);

extern int g_iCurrentTiming;
extern startup_timing_t g_StartupTimings[32];

extern float g_LastScreenUpdateTime;

extern cvar_t cl_lw;
extern cvar_t cl_nointerp;
extern cvar_t show_fps;
extern cvar_t ex_interp;
extern cvar_t cl_name;

extern client_state_t m1;


extern dlight_t cl_dlights[MAX_DLIGHTS];

void SetupStartupTimings();
void AddStartupTiming(const char* name);
void PrintStartupTimings();

dlight_t* CL_AllocDlight(int key);

dlight_t* CL_AllocElight(int key);

model_t* CL_GetModelByIndex(int index);

void CL_GetPlayerHulls();

bool UserIsConnectedOnLoopback();

void SetPal(int i);

void GetPos(vec3_t origin, vec3_t angles);

const char* CL_CleanFileName(const char* filename);

void CL_ClearCaches();

void CL_ClearClientState();

void CL_ClearState(bool bQuiet);

void CL_CreateResourceList();
void CL_CheckClientState(void);
void CL_Disconnect(void);
void CL_Disconnect_f(void);
void CL_ProcessFile(char* filename, qboolean successfully_received);
void CL_ProcessFile(qboolean successfully_received, char* filename);
void CL_SignonReply(void);
qboolean CL_PrecacheResources(void);
void CL_ReadPackets(void);
void CL_Move(void);
void CL_CheckForResend(void);
void CL_GGSpeeds(float flCurTime);