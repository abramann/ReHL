#include "precompiled.h"


void CL_TempEntUpdate();
void CL_LinkPlayers();
void CL_LinkPacketEntities();
void CL_FireEvents();
long double CL_LerpPoint();

VAR(particle_t*, free_particles, 0x134A4);
VAR(particle_t*, active_particles, 0x13513);
VVAR(int, cl_numvisedicts, 0xBB9D, 0);
//VVAR(int, cl_numvisedicts, 0xBBB3, 0);

double g_flLatency;
int last_incoming_sequence = 0;
int cl_numbeamentities;
mleaf_t *r_oldviewleaf;

cl_entity_t* cl_visedicts[512];

qboolean CL_IsPlayerIndex(int index)
{
	if (index > 0)
		return index <= g_pcl.maxclients;

	return false;
}

void CL_ResetFrameStats(frame_t* frame)
{
	frame->clientbytes = 0;
	frame->packetentitybytes = 0;
	frame->tentitybytes = 0;
	frame->playerinfobytes = 0;
	frame->soundbytes = 0;
	frame->usrbytes = 0;
	frame->eventbytes = 0;
	frame->voicebytes = 0;
	frame->msgbytes = 0;
}

void CL_Particle(vec_t* origin, int color, float life, int zpos, int zvel)
{
	NOT_TESTED;
	if (!free_particles)
		return;

	active_particles = free_particles;
	free_particles = free_particles->next;
	active_particles->next = free_particles;
	active_particles->die = life + g_pcl.time;
	active_particles->color = color;
	active_particles->packedColor = 0;
	active_particles->type = pt_static;

	VectorCopy(active_particles->vel, vec3_origin);
	VectorCopy(active_particles->org, vec3_origin);
	active_particles->org[2] += zpos;
}

void CL_SetSolidPlayers(int playernum)
{
	NOT_IMPLEMENTED;
}

void CL_SetUpPlayerPrediction(bool dopred, bool bIncludeLocalClient)
{
	NOT_IMPLEMENTED;
}

void CL_AddStateToEntlist(physent_t* pe, entity_state_t* state)
{
	NOT_IMPLEMENTED;
}

qboolean CL_EntityTeleported(cl_entity_t* ent)
{
	if (ent->curstate.origin[0] - ent->prevstate.origin[0] <= 128.0 && ent->curstate.origin[1] - ent->prevstate.origin[1] <= 128.0)
	{
		return ent->curstate.origin[2] - ent->prevstate.origin[2] > 128.0;
	}

	return true;
}



int CL_ParseDeltaHeader(qboolean* remove, qboolean* custom, int* numbase, qboolean* newbl,
	int* newblindex, qboolean full, int* offset)
{
	NOT_IMPLEMENTED;
	return 0;
}



void CL_FlushEntityPacket(qboolean needbitencodingframe)
{
	NOT_IMPLEMENTED;
}


/* WARNING: Removing unreachable block (ram,0x0018b9c4) */
/* WARNING: Removing unreachable block (ram,0x0018b9b8) */
inline qboolean CL_EntityIgnoreLerp(cl_entity_t* e)
{
	NOT_IMPLEMENTED;
	// Check from implementation

	if (cl_nointerp.value > 0.f)
		return true;

	if (e->model && e->model->type == mod_alias)
		return false;

	return (e->curstate.movetype == MOVETYPE_NONE) ? true : false;
}

/*
==================
CL_EntityCustomLerp

==================
*/
inline qboolean CL_EntityCustomLerp(cl_entity_t* e)
{
	switch (e->curstate.movetype)
	{
	case MOVETYPE_NONE:
	case MOVETYPE_STEP:
	case MOVETYPE_WALK:
	case MOVETYPE_FLY:
		// case MOVETYPE_COMPOUND:
		return false;
	}

	return true;
}

/*
==================
CL_ParametricMove

check for parametrical moved entities
==================
*/
qboolean CL_ParametricMove(cl_entity_t* ent)
{
	NOT_IMPLEMENTED;
	// Check from function

	float	frac, dt, t;
	vec3_t	delta;

	if (ent->curstate.starttime == 0.0f || ent->curstate.impacttime == 0.0f)
		return false;

	VectorSubtract(ent->curstate.endpos, ent->curstate.startpos, delta);
	dt = ent->curstate.impacttime - ent->curstate.starttime;

	if (dt != 0.0f)
	{
		if (ent->lastmove > g_pcl.time)
			t = ent->lastmove;
		else t = g_pcl.time;

		frac = (t - ent->curstate.starttime) / dt;
		if ((frac <= 1.0) && (frac = 0.0, 0.0 <= frac))
			frac = 1.0;

		//frac = bound(0.0f, frac, 1.0f);
		VectorMA(ent->curstate.startpos, frac, delta, ent->curstate.origin);

		ent->lastmove = t;
	}

	VectorNormalize(delta);
	if (Length(delta) > 0.0f)
		VectorAngles(delta, ent->curstate.angles); // re-aim projectile

	return true;
}

void CL_ProcessEntityUpdate(cl_entity_t* ent)
{
	NOT_IMPLEMENTED;
	
	/*
	qboolean	parametric;

	ent->model = CL_GetModelByIndex(ent->curstate.modelindex);
	ent->index = ent->curstate.number;
	COM_NormalizeAngles(ent->curstate.angles);

	parametric = ent->curstate.starttime != 0.0f && ent->curstate.impacttime != 0.0f;

	// allow interpolation on bmodels too
	if (ent->model && ent->model->type == mod_brush)
		ent->curstate.animtime = ent->curstate.msg_time;

	if (CL_EntityCustomLerp(ent) && !parametric)
		ent->curstate.animtime = ent->curstate.msg_time;

	if (!(ent->curstate.animtime * 1000 - ent->prevstate.animtime * 1000) < 2 || CL_EntityIgnoreLerp(ent))
	{
		R_UpdateLatchedVars(ent);
		CL_UpdatePositions(ent);
	}

	// g-cont. it should be done for all the players?
	if (ent->player)
		ent->curstate.angles[PITCH] /= -3.0f;

	VectorCopy(ent->curstate.origin, ent->origin);
	VectorCopy(ent->curstate.angles, ent->angles);

	if ((ent->curstate.starttime != 0.0) && ((ent->curstate).impacttime != 0.0)) {
		CL_ParametricMove(ent);
	}

	VectorCopy(ent->curstate.origin, ent->origin);
	VectorCopy(ent->curstate.angles, ent->angles);

	// initialize attachments for now
	VectorCopy(ent->origin, ent->attachment[0]);
	VectorCopy(ent->origin, ent->attachment[1]);
	VectorCopy(ent->origin, ent->attachment[2]);
	VectorCopy(ent->origin, ent->attachment[3]);
	return;
	*/
}

void CL_ProcessPacket(frame_t* frame)
{
	NOT_IMPLEMENTED;
	
	/*
	entity_state_s* dst;
	int iVar1;
	int uVar2;
	uint uVar3;
	qboolean qVar4;
	uint uVar5;
	cl_entity_t* pcVar6;
	entity_state_s* state;
	int iVar7;
	int local_30;

	if (0 < (frame->packet_entities).num_entities) {
		local_30 = 0;
		iVar7 = 0;
		do {
			state = (entity_state_s*)
				((int)((frame->packet_entities).entities)->origin + local_30 + -0x10);
			state->messagenum = cl.parsecount;
			pcVar6 = cl_entities;
			state->msg_time = (float)cl.mtime[0];
			iVar1 = state->number;
			pcVar6 = pcVar6 + iVar1;
			qVar4 = false;
			if (0 < iVar1) {
				qVar4 = (qboolean)(iVar1 <= cl.maxclients);
			}
			pcVar6->player = qVar4;
			if (state->number + -1 == cl.playernum) {
				ClientDLL_TxferLocalOverrides(state, &frame->clientdata);
			}
			uVar3 = 0;
			do {
				uVar5 = uVar3;
				uVar2 = *(int*)((int)(pcVar6->curstate).origin + (uVar5 - 0x10));
				*(int*)((int)(pcVar6->prevstate).origin + (uVar5 - 0xc)) =
					*(int*)((int)(pcVar6->curstate).origin + (uVar5 - 0xc));
				*(int*)((int)(pcVar6->prevstate).origin + (uVar5 - 0x10)) = uVar2;
				uVar3 = uVar5 + 8;
			} while (uVar5 + 8 < 0x150);
			*(int*)((int)(pcVar6->prevstate).origin + (uVar5 - 8)) =
				*(int*)((int)(pcVar6->curstate).origin + (uVar5 - 8));
			uVar3 = 0;
			do {
				uVar5 = uVar3;
				uVar2 = *(int*)((int)state->origin + (uVar5 - 0x10));
				*(int*)((int)(pcVar6->curstate).origin + (uVar5 - 0xc)) =
					*(int*)((int)state->origin + (uVar5 - 0xc));
				*(int*)((int)(pcVar6->curstate).origin + (uVar5 - 0x10)) = uVar2;
				uVar3 = uVar5 + 8;
			} while (uVar5 + 8 < 0x150);
			*(int*)((int)(pcVar6->curstate).origin + (uVar5 - 8)) =
				*(int*)((int)state->origin + (uVar5 - 8));
			CL_ProcessEntityUpdate(pcVar6);
			if ((1 << ((byte)iVar7 & 7) & (uint)(frame->packet_entities).flags[iVar7 >> 3]) == 0) {
			LAB_0018c2f0:
				qVar4 = pcVar6->player;
			}
			else {
				(pcVar6->latched).prevorigin[0] = (pcVar6->curstate).origin[0];
				(pcVar6->latched).prevorigin[1] = (pcVar6->curstate).origin[1];
				(pcVar6->latched).prevorigin[2] = (pcVar6->curstate).origin[2];
				(pcVar6->latched).prevangles[0] = (pcVar6->curstate).angles[0];
				(pcVar6->latched).prevangles[1] = (pcVar6->curstate).angles[1];
				(pcVar6->latched).prevangles[2] = (pcVar6->curstate).angles[2];
				R_ResetLatched(pcVar6, true);
				CL_ResetPositions(pcVar6);
				if (((pcVar6->curstate).starttime == 0.0) || ((pcVar6->curstate).impacttime == 0.0))
					goto LAB_0018c2f0;
				pcVar6->lastmove = (float)cl.time;
				qVar4 = pcVar6->player;
			}
			if (qVar4 != false) {
				iVar1 = state->number + -1;
				dst = (entity_state_s*)(cl.parsecountmod * 0x42b8 + 0xc7f6fc + iVar1 * 0x154);
				dst->number = state->number;
				dst->messagenum = cl.parsecount;
				dst->msg_time = (float)cl.mtime[0];
				ClientDLL_ProcessPlayerState(dst, state);
				if (cl.playernum == iVar1) {
					CL_CheckPredictionError();
				}
			}
			iVar7 = iVar7 + 1;
			local_30 = local_30 + 0x154;
		} while ((frame->packet_entities).num_entities != iVar7 &&
			iVar7 <= (frame->packet_entities).num_entities);
	}
	return;
	*/
}



void CL_ParsePacketEntities(qboolean delta, int* playerbits)
{
	NOT_IMPLEMENTED;
	/*
	byte* pbVar1;
	entity_state_s* peVar2;
	int uVar3;
	int iVar4;
	delta_t** ppdVar5;
	delta_t** ppdVar6;
	delta_t** ppdVar7;
	int iVar8;
	int iVar9;
	entity_state_s* peVar10;
	uint32 uVar11;
	int iVar12;
	model_s* pmVar13;
	delta_t** ppdVar14;
	uint uVar15;
	packet_entities_t* dest;
	uint32 deadEntity;
	delta_t* pFields;
	int local_c4;
	int local_b8;
	qboolean local_b4;
	packet_entities_t dummy;
	qboolean remove;
	qboolean custom;
	qboolean newbl;
	int newblindex;
	int local_2c[3];
	int numbase;

	newblindex = 0;
	remove = false;
	custom = false;
	newbl = false;
	numbase = 0;
	ppdVar5 = DELTA_LookupRegistration("entity_state_t");
	ppdVar6 = DELTA_LookupRegistration("entity_state_player_t");
	ppdVar7 = DELTA_LookupRegistration("custom_entity_state_t");
	if (cls.signon == 1) {
		cls.signon = 2;
		CL_SignonReply();
	}
	iVar4 = cl.parsecountmod;
	iVar8 = cl.parsecountmod * 0x42b8;
	cl.frames[cl.parsecountmod].invalid = false;
	iVar9 = MSG_ReadShort();
	iVar12 = 1;
	if (0 < iVar9) {
		iVar12 = iVar9;
	}
	peVar10 = cl.frames[iVar4].packet_entities.entities;
	if (peVar10 != (entity_state_s*)0x0) {
		Mem_Free(peVar10);
	}
	peVar10 = (entity_state_s*)Mem_ZeroMalloc(iVar12 * 0x154);
	cl.frames[iVar4].packet_entities.entities = peVar10;
	cl.frames[iVar4].packet_entities.num_entities = iVar9;
	Q_memset(cl.frames[iVar4].packet_entities.flags, 0, 0x20);
	if (delta == false) {
		uVar15 = 0xffffffff;
		if (cls.demowaiting != false) {
			cls.demowaiting = false;
			MSG_WriteChar(&cls.netchan.message, 3);
			MSG_WriteString(&cls.netchan.message, "fullupdate");
		}
	}
	else {
		uVar15 = MSG_ReadByte();
		uVar15 = uVar15 & 0xff;
	}
	if (cl.max_edicts == 0) {
		CL_FlushEntityPacket(true);
		peVar10 = cl.frames[iVar4].packet_entities.entities;
		if (peVar10 != (entity_state_s*)0x0) {
			Mem_Free(peVar10);
		}
		cl.frames[iVar4].packet_entities.entities = (entity_state_s*)0x0;
		cl.frames[iVar4].packet_entities.num_entities = 0;
		return;
	}
	if (uVar15 == 0xffffffff) {
		dest = &dummy;
		Q_memset(dest, 0, 0x28);
		dummy.num_entities = 0;
		local_b4 = true;
	}
	else {
		if (CL_UPDATE_MASK <= (int)(cls.netchan.incoming_sequence - uVar15 & 0xff)) {
			CL_FlushEntityPacket(true);
			peVar10 = cl.frames[iVar4].packet_entities.entities;
			if (peVar10 != (entity_state_s*)0x0) {
				Mem_Free(peVar10);
			}
			cl.frames[iVar4].packet_entities.entities = (entity_state_s*)0x0;
			cl.frames[iVar4].packet_entities.num_entities = 0;
			return;
		}
		local_b4 = false;
		dest = &cl.frames[uVar15 & CL_UPDATE_MASK].packet_entities;
	}
	cl.validsequence = cls.netchan.incoming_sequence;
	MSG_StartBitReading(&net_message);
	local_c4 = 0;
	local_b8 = 0;
LAB_0018cb00:
	do {
		uVar11 = MSG_PeekBits(0x10);
		if (uVar11 == 0) {
			uVar11 = MSG_ReadBits(0x10);
		}
		else {
			uVar11 = CL_ParseDeltaHeader(&remove, &custom, &numbase, &newbl, &newblindex, local_b4, local_2c);
		}
		if (uVar11 == 0) {
			if (dest->num_entities != local_b8 && local_b8 <= dest->num_entities) {
				iVar8 = local_b8 - local_c4;
				iVar12 = local_c4 * 0x154;
				do {
					if (0xff < local_c4) {
						Host_EndGame("CL_ParsePacketEntities: newindex == MAX_PACKET_ENTITIES");
					}
					peVar10 = dest->entities;
					peVar2 = cl.frames[iVar4].packet_entities.entities;
					uVar15 = 0;
					do {
						uVar3 = *(int*)((int)peVar10[iVar8].origin + uVar15 + iVar12 + -0x10);
						*(int*)((int)peVar2->origin + uVar15 + iVar12 + -0xc) =
							*(int*)((int)peVar10[iVar8].origin + uVar15 + iVar12 + -0xc);
						*(int*)((int)peVar2->origin + uVar15 + iVar12 + -0x10) = uVar3;
						uVar15 = uVar15 + 8;
					} while (uVar15 < 0x150);
					local_b8 = local_b8 + 1;
					local_c4 = local_c4 + 1;
					*(int*)((int)peVar2->origin + uVar15 + iVar12 + -0x10) =
						*(int*)((int)peVar10[iVar8].origin + uVar15 + iVar12 + -0x10);
					iVar12 = iVar12 + 0x154;
				} while (dest->num_entities != local_b8 && local_b8 <= dest->num_entities);
			}
			goto LAB_0018cc5f;
		}
		deadEntity = 9999;
		if (local_b8 < dest->num_entities) {
			deadEntity = dest->entities[local_b8].number;
		}
		if ((int)deadEntity < (int)uVar11) {
			if (local_b4 != false) {
				Con_Printf("WARNING: oldcopy invalid on non-delta compressed update\n");
				CL_FlushEntityPacket(false);
				peVar10 = cl.frames[iVar4].packet_entities.entities;
				if (peVar10 != (entity_state_s*)0x0) {
					Mem_Free(peVar10);
				}
				cl.frames[iVar4].packet_entities.entities = (entity_state_s*)0x0;
				cl.frames[iVar4].packet_entities.num_entities = 0;
				goto LAB_0018cb00;
			}
			iVar9 = local_b8 * 0x154;
			iVar12 = local_c4 - local_b8;
			do {
				if (0xff < local_c4) {
					Host_EndGame("CL_ParsePacketEntities: newindex == MAX_PACKET_ENTITIES");
				}
				uVar15 = 0;
				peVar10 = cl.frames[iVar4].packet_entities.entities;
				peVar2 = dest->entities;
				do {
					uVar3 = *(int*)((int)peVar2->origin + uVar15 + iVar9 + -0x10);
					*(int*)((int)peVar10[iVar12].origin + uVar15 + iVar9 + -0xc) =
						*(int*)((int)peVar2->origin + uVar15 + iVar9 + -0xc);
					*(int*)((int)peVar10[iVar12].origin + uVar15 + iVar9 + -0x10) = uVar3;
					uVar15 = uVar15 + 8;
				} while (uVar15 < 0x150);
				deadEntity = 9999;
				local_b8 = local_b8 + 1;
				local_c4 = local_c4 + 1;
				*(int*)((int)peVar10[iVar12].origin + uVar15 + iVar9 + -0x10) =
					*(int*)((int)peVar2->origin + uVar15 + iVar9 + -0x10);
				if (local_b8 < dest->num_entities) {
					deadEntity = *(uint32*)((int)dest->entities[1].origin + iVar9 + -0xc);
				}
				iVar9 = iVar9 + 0x154;
			} while ((int)deadEntity < (int)uVar11);
		}
		if ((int)uVar11 < (int)deadEntity) {
			if (remove == false) {
				if (0xff < local_c4) {
					Host_EndGame("CL_ParsePacketEntities: newindex == MAX_PACKET_ENTITIES");
				}
				if (newbl == false) {
					if (local_2c[0] == 0) {
						peVar10 = &cl_entities[uVar11].baseline;
					}
					else {
						peVar10 = cl.frames[iVar4].packet_entities.entities + (local_c4 - local_2c[0]);
					}
				}
				else {
					peVar10 = cl.instanced_baseline + newblindex;
				}
				CL_EntityNum(uVar11);
				if (custom == false) {
					if ((int)uVar11 < 1) {
						DELTA_ParseDelta((unsigned char*)peVar10,
							(unsigned char*)(cl.frames[iVar4].packet_entities.entities + local_c4),
							*ppdVar5);// , 0x154);
						cl.frames[iVar4].packet_entities.entities[local_c4].entityType = 1;
					}
					else {
						ppdVar14 = ppdVar6;
						if (cl.maxclients < (int)uVar11) {
							ppdVar14 = ppdVar5;
						}
						iVar12 = DELTA_ParseDelta((unsigned char*)peVar10,
							(unsigned char*)(cl.frames[iVar4].packet_entities.entities + local_c4
								), *ppdVar14);// , 0x154);
						cl.frames[iVar4].packet_entities.entities[local_c4].entityType = 1;
						if (((int)uVar11 <= cl.maxclients) && (playerbits != (int*)0x0)) {
							*playerbits = *playerbits + iVar12;
						}
					}
				}
				else {
					DELTA_ParseDelta((unsigned char*)peVar10,
						(unsigned char*)(cl.frames[iVar4].packet_entities.entities + local_c4), *ppdVar7);// ,  0x154);
					cl.frames[iVar4].packet_entities.entities[local_c4].entityType = 2;
				}
				cl.frames[iVar4].packet_entities.entities[local_c4].number = uVar11;
				if (((cl_needinstanced.value != 0.0) && (cl.highentity < (int)uVar11)) && (newbl == false))
				{
					iVar12 = cl.frames[iVar4].packet_entities.entities[local_c4].modelindex;
					pmVar13 = nullptr;
					if (iVar12 != 0) {
						pmVar13 = cl.model_precache[iVar12];
					}
					Con_DPrintf("Non-instanced %i %s without baseline\n", uVar11, pmVar13);
				}
				if ((local_b4 == false) || (cls.passive == false)) {
					pbVar1 = (byte*)(iVar8 + 0xc83964 + (local_c4 >> 3));
					*pbVar1 = *pbVar1 | (byte)(1 << ((byte)local_c4 & 7));
				}
				local_c4 = local_c4 + 1;
			}
			else if (local_b4 != false) {
				Con_Printf("WARNING: remove invalid on non-delta compressed update\n");
				CL_FlushEntityPacket(false);
				peVar10 = cl.frames[iVar4].packet_entities.entities;
				if (peVar10 != (entity_state_s*)0x0) {
					Mem_Free(peVar10);
				}
				cl.frames[iVar4].packet_entities.entities = (entity_state_s*)0x0;
				cl.frames[iVar4].packet_entities.num_entities = 0;
			LAB_0018cc5f:
				MSG_EndBitReading(&net_message);
				if (msg_badread != false) {
					Host_EndGame("msg_badread in packetentities\n");
					return;
				}
				CL_ProcessPacket(cl.frames + iVar4);
				return;
			}
		}
		else if (uVar11 == deadEntity) {
			if (local_b4 != false) {
				cl.validsequence = 0;
				Con_Printf("WARNING: delta on full update");
			}
			if (remove == false) {
				if (custom == false) {
					if ((int)uVar11 < 1) {
						DELTA_ParseDelta((unsigned char*)(dest->entities + local_b8),
							(unsigned char*)(cl.frames[iVar4].packet_entities.entities + local_c4),
							*ppdVar5);// , 0x154);
						cl.frames[iVar4].packet_entities.entities[local_c4].entityType = 1;
					}
					else {
						if (cl.maxclients < (int)uVar11) {
							pFields = *ppdVar5;
							peVar10 = cl.frames[iVar4].packet_entities.entities;
						}
						else {
							pFields = *ppdVar6;
							peVar10 = cl.frames[iVar4].packet_entities.entities;
						}
						iVar12 = DELTA_ParseDelta((unsigned char*)(dest->entities + local_b8),
							(unsigned char*)(peVar10 + local_c4), pFields);// , 0x154);
						cl.frames[iVar4].packet_entities.entities[local_c4].entityType = 1;
						if (((int)uVar11 <= cl.maxclients) && (playerbits != (int*)0x0)) {
							*playerbits = *playerbits + iVar12;
						}
					}
				}
				else {
					DELTA_ParseDelta((unsigned char*)(dest->entities + local_b8),
						(unsigned char*)(cl.frames[iVar4].packet_entities.entities + local_c4), *ppdVar7);// , 0x154);
					cl.frames[iVar4].packet_entities.entities[local_c4].entityType = 2;
				}
				local_b8 = local_b8 + 1;
				cl.frames[iVar4].packet_entities.entities[local_c4].number = uVar11;
				local_c4 = local_c4 + 1;
			}
			else {
				R_KillDeadBeams(deadEntity);
				local_b8 = local_b8 + 1;
			}
		}
	} while (true);
	*/
}

void CL_ParseClientdata(void)
{
	NOT_IMPLEMENTED;
	
	/*
	clientdata_t* to;
	float fVar1;
	float fVar2;
	local_state_t* plVar3;
	int iVar4;
	int iVar5;
	int iVar6;
	int iVar7;
	qboolean qVar8;
	delta_t** ppdVar9;
	delta_t** ppdVar10;
	clientdata_s* pcd;
	int iVar11;
	uint32 uVar12;
	uint32 uVar13;
	uint uVar14;
	weapon_data_s* pwd;
	uint uVar15;
	local_state_t* pps;
	int iVar16;
	weapon_data_s* wd;
	local_state_t* local_26c;
	int command_ack;
	int last_command_ack;
	clientdata_t baseline;
	weapon_data_t wbaseline;

	ppdVar9 = DELTA_LookupRegistration("clientdata_t");
	ppdVar10 = DELTA_LookupRegistration("weapon_data_t");
	Q_memset(&baseline, 0, 0x1dc);
	iVar7 = cls.netchan.incoming_acknowledged;
	iVar11 = cls.netchan.incoming_sequence;
	iVar5 = last_incoming_sequence;
	iVar4 = CL_UPDATE_MASK;
	uVar14 = last_incoming_sequence + 1;
	if ((int)uVar14 < cls.netchan.incoming_sequence) {
		do {
			uVar15 = uVar14 & iVar4;
			if (0.0 <= cl.frames[uVar15].receivedtime) {
				cl.frames[uVar15].receivedtime = 1.6;
				cl.frames[uVar15].latency = 0;
			}
			uVar14 = uVar14 + 1;
		} while (uVar14 != iVar11);
	}
	uVar14 = iVar4 & iVar11;
	iVar16 = uVar14 * 0x42b8;
	cl.parsecount = iVar11;
	cl.parsecountmod = uVar14;
	cl.frames[uVar14].time = cl.mtime[0];
	fVar1 = cl.commands[iVar7 & iVar4].senttime;
	parsecounttime = (double)fVar1;
	fVar2 = (float)realtime;
	cl.commands[iVar7 & iVar4].receivedtime = fVar2;
	cl.frames[uVar14].receivedtime = (double)fVar2;
	cl.frames[uVar14].clientbytes = 0;
	cl.frames[uVar14].tentitybytes = 0;
	cl.frames[uVar14].soundbytes = 0;
	cl.frames[uVar14].eventbytes = 0;
	cl.frames[uVar14].msgbytes = 0;
	iVar6 = last_command_ack;
	cl.frames[uVar14].packetentitybytes = 0;
	cl.frames[uVar14].playerinfobytes = 0;
	cl.frames[uVar14].usrbytes = 0;
	cl.frames[uVar14].voicebytes = 0;
	if (iVar6 != -1) {
		if (cls.spectator == false) {
			uVar15 = iVar5 + (iVar7 - iVar6) & iVar4;
			local_26c = (local_state_t*)(iVar16 + 0xc7f6fc + cl.playernum * 0x154);
			pps = cl.predicted_frames + uVar15;
			pcd = &cl.frames[uVar14].clientdata;
			plVar3 = cl.predicted_frames + uVar15;
			pwd = cl.predicted_frames[uVar15].weapondata;
			wd = cl.frames[uVar14].weapondata;
		}
		else {
			pwd = spectatorState.weapondata;
			plVar3 = &spectatorState;
			pps = &spectatorState;
			pcd = &spectatorState.client;
			local_26c = &spectatorState;
			wd = pwd;
		}
		ClientDLL_TxferPredictionData
			((entity_state_s*)local_26c, (entity_state_s*)pps, pcd, &plVar3->client, wd, pwd);
		fVar2 = (float)realtime;
		fVar1 = (float)parsecounttime;
		iVar11 = cls.netchan.incoming_sequence;
	}
	qVar8 = cls.spectator;
	last_incoming_sequence = iVar11;
	cl.frames[uVar14].latency = (double)(fVar2 - fVar1);
	last_command_ack = iVar7;
	g_flLatency = (double)(fVar2 - fVar1);
	if (qVar8 == false) {
		to = &cl.frames[uVar14].clientdata;
		MSG_StartBitReading(&net_message);
		uVar12 = MSG_ReadBits(1);
		if (uVar12 == 0) {
			DELTA_ParseDelta((unsigned char*)&baseline, (unsigned char*)to, *ppdVar9);// , 0x1dc);
			Q_memset(&wbaseline, 0, 0x58);
			while (uVar12 = MSG_ReadBits(1), uVar12 != 0) {
				uVar12 = MSG_ReadBits(6);
				DELTA_ParseDelta((unsigned char*)&wbaseline, (unsigned char*)(iVar16 + 0xc82360 + uVar12 * 0x58), *ppdVar10);// , 0x58);
			}
		}
		else {
			uVar12 = MSG_ReadBits(8);
			DELTA_ParseDelta((unsigned char*)&cl.frames[CL_UPDATE_MASK & uVar12].clientdata, (unsigned char*)to, *ppdVar9);// , 0x1dc);
			Q_memset(&wbaseline, 0, 0x58);
			while (uVar13 = MSG_ReadBits(1), uVar13 != 0) {
				uVar13 = MSG_ReadBits(6);
				DELTA_ParseDelta((unsigned char*)(uVar13 * 0x58 + 0xc82360 + (CL_UPDATE_MASK & uVar12) * 0x42b8),
					(unsigned char*)(iVar16 + 0xc82360 + uVar13 * 0x58), *ppdVar10);// , 0x58);
			}
		}
		MSG_EndBitReading(&net_message);
		Q_strncpy(cls.physinfo, cl.frames[uVar14].clientdata.physinfo, 0xff);
		cls.physinfo[255] = '\0';
		cl.stats[0] = (int)cl.frames[uVar14].clientdata.health;
		cl.weapons = cl.frames[uVar14].clientdata.weapons;
		cl.maxspeed = cl.frames[uVar14].clientdata.maxspeed;
		cl.pushmsec = cl.frames[uVar14].clientdata.pushmsec;
		return;
	}
	cl.stats[0] = 1;
	return;
	*/
}

void CL_EmitEntities(void)
{
	cl_numvisedicts = 0;
	cl_numbeamentities = 0;

	if (g_pcls.state != ca_active || !m1.validsequence || m1.frames[m1.parsecountmod].invalid)
		return;

	m1.commands[CL_UPDATE_MASK & (g_pcls.netchan.outgoing_sequence - 1)].frame_lerp = CL_LerpPoint();
	if (g_pcls.spectator)
	{
		mleaf_t* leaf = Mod_PointInLeaf(m1.simorg, m1.worldmodel);
		uchar* ppvs = Mod_LeafPVS(leaf, m1.worldmodel);
		PVSMark(m1.worldmodel, ppvs);
	}

	CL_LinkPlayers();
	CL_LinkPacketEntities();

	for (int i = 0; i < cl_numvisedicts; i++)
	{
		cl_entity_t* viseditct = cl_visedicts[i];
		int aiment = viseditct->curstate.aiment;
		if (aiment && viseditct->curstate.movetype == 12)
		{
			cl_entity_t* ent = &cl_entities[aiment];
			VectorCopy(ent->origin, viseditct->origin);
		}
	}

	ClientDLL_CreateEntities();

	CL_TempEntUpdate();
	if (g_pcls.spectator)
	{
		r_oldviewleaf = 0;
		R_MarkLeaves();
	}

	CL_FireEvents();
}

void CL_TempEntUpdate()
{
	NOT_IMPLEMENTED;
}

void CL_LinkPlayers()
{
	NOT_IMPLEMENTED;
}

void CL_LinkPacketEntities()
{
	NOT_IMPLEMENTED;
}

void CL_FireEvents()
{
	NOT_IMPLEMENTED;
}

long double CL_LerpPoint()
{
	NOT_IMPLEMENTED;
	return 0;
}