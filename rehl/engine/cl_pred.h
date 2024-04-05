#pragma once

#include "entity_state.h"
#include "usercmd.h"

extern int pushed;
extern int oldphysent;
extern int oldvisent;
extern int g_lastupdate_sequence;

void CL_PushPMStates();

void CL_PopPMStates();

void CL_RunUsercmd(local_state_t* from, local_state_t* to, usercmd_t* u, bool runfuncs, double* pfElapsed, unsigned int random_seed);

void CL_CheckPredictionError(void);
void CL_SetLastUpdate(void);