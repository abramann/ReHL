#pragma once

#include "pm_defs.h"
#include "cl_entity.h"
#include "client.h"

const int CL_UPDATE_MASK = 63;

const int CL_UPDATE_BACKUP = 64;

EXTERN_VAR(int, cl_numvisedicts);
EXTERN_VAR(int, cl_numbeamentities);

extern double parsecounttime;

qboolean CL_IsPlayerIndex(int index);

void CL_ResetFrameStats(frame_t* frame);

void CL_Particle(vec_t* origin, int color, float life, int zpos, int zvel);

void CL_SetSolidPlayers(int playernum);

void CL_SetUpPlayerPrediction(bool dopred, bool bIncludeLocalClient);

void CL_AddStateToEntlist(physent_t* pe, entity_state_t* state);
void CL_ParsePacketEntities(qboolean delta, int* playerbits);
void CL_ParseClientdata(void);

qboolean CL_EntityTeleported(cl_entity_t* ent);

void CL_EmitEntities(void);
