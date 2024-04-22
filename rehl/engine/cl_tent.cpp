#include "precompiled.h"

cvar_t tracerSpeed = { "tracerSpeed","6000" };
cvar_t tracerOffset = { "tracerOffset","30" };
cvar_t tracerLength = { "tracerLength","0.8" };
cvar_t tracerRed = { "tracerRed","0.8" };
cvar_t tracerGreen = { "tracerGreen","0.8" };
cvar_t tracerBlue = { "tracerBlue","0.4" };
cvar_t tracerAlpha = { "tracerAlpha","0.5" };
cvar_t ex_interp = { "ex_interp", "0.1", FCVAR_ARCHIVE };

sfx_t * cl_sfx_r_exp1;
sfx_t * cl_sfx_r_exp2;
sfx_t * cl_sfx_r_exp3;
sfx_t * cl_sfx_pl_shell1;
sfx_t * cl_sfx_pl_shell2;
sfx_t * cl_sfx_pl_shell3;
sfx_t * cl_sfx_sshell1;
sfx_t * cl_sfx_sshell2;
sfx_t * cl_sfx_sshell3;
sfx_t * cl_sfx_wood1;
sfx_t * cl_sfx_wood2;
sfx_t * cl_sfx_wood3;
sfx_t * cl_sfx_metal1;
sfx_t * cl_sfx_metal2;
sfx_t * cl_sfx_metal3;
sfx_t * cl_sfx_glass1;
sfx_t * cl_sfx_glass2;
sfx_t * cl_sfx_glass3;
sfx_t * cl_sfx_concrete1;
sfx_t * cl_sfx_concrete2;
sfx_t * cl_sfx_concrete3;
sfx_t * cl_sfx_flesh1;
sfx_t * cl_sfx_flesh2;
sfx_t * cl_sfx_flesh3;
sfx_t * cl_sfx_flesh4;
sfx_t * cl_sfx_flesh5;
sfx_t * cl_sfx_flesh6;
sfx_t * cl_sfx_geiger1;
sfx_t * cl_sfx_geiger2;
sfx_t * cl_sfx_geiger3;
sfx_t * cl_sfx_geiger4;
sfx_t * cl_sfx_geiger5;
sfx_t * cl_sfx_geiger6;

model_t* cl_sprite_dot;
model_t* cl_sprite_lightning;
model_t* cl_sprite_white;
model_t* cl_sprite_glow;
model_t* cl_sprite_muzzleflash[3];
model_t* cl_sprite_ricochet;
model_t* cl_sprite_shell;

TEMPENTITY gTempEnts[MAX_TEMP_ENTITIES];
TEMPENTITY* gpTempEntFree;
TEMPENTITY* gpTempEntActive;

sfx_t *cl_sfx_ric1, *cl_sfx_ric2, *cl_sfx_ric3, *cl_sfx_ric4, *cl_sfx_ric5;

void CL_TempEntInit()
{
	Q_memset(gTempEnts, 0, sizeof(gTempEnts));

	// Fix up pointers
	for (int i = 0; i < ARRAYSIZE(gTempEnts) - 1; ++i)
	{
		gTempEnts[i].next = &gTempEnts[i + 1];
	}

	gTempEnts[ARRAYSIZE(gTempEnts) - 1].next = nullptr;

	gpTempEntFree = gTempEnts;
	gpTempEntActive = nullptr;
}

void CL_TempEntPrepare(TEMPENTITY* pTemp, model_t* model)
{
	Q_memset(&pTemp->entity, 0, sizeof(pTemp->entity));
	pTemp->flags = 0;
	pTemp->entity.curstate.colormap = 0;
	pTemp->die = g_pcl.time + 0.75;
	pTemp->entity.model = model;
	pTemp->fadeSpeed = 0.5;
	pTemp->hitSound = 0;
	pTemp->clientIndex = -1;
	pTemp->bounceFactor = 1.0;
	pTemp->hitcallback = 0;
	pTemp->callback = 0;
}

TEMPENTITY* CL_TempEntAlloc(vec_t* org, model_t* model)
{
	cl_entity_t* dest;
	TEMPENTITY* pTVar1;
	TEMPENTITY* pTVar2;

	pTVar2 = gpTempEntFree;
	if (gpTempEntFree == NULL)
	{
		Con_DPrintf("Overflow %d temporary ents!\n", MAX_TEMP_ENTITIES);
	}
	else if (model == NULL)
	{
		pTVar2 = NULL;
		Con_DPrintf("efx.CL_TempEntAlloc: No model\n");
	}
	else
	{
		dest = &gpTempEntFree->entity;
		gpTempEntFree = gpTempEntFree->next;
		Q_memset(dest, 0, 3000);
		pTVar2->flags = FTENT_NONE;
		(pTVar2->entity).curstate.colormap = 0;
		pTVar2->die = g_pcl.time + 0.75;
		(pTVar2->entity).model = model;
		(pTVar2->entity).curstate.rendermode = 0;
		(pTVar2->entity).curstate.renderfx = 0;
		pTVar2->fadeSpeed = 0.5;
		pTVar2->hitSound = 0;
		pTVar2->clientIndex = -1;
		pTVar2->bounceFactor = 1.0;
		pTVar2->hitcallback = NULL;
		pTVar2->callback = NULL;
		pTVar2->priority = 0;
		(pTVar2->entity).origin[0] = *org;
		(pTVar2->entity).origin[1] = org[1];
		(pTVar2->entity).origin[2] = org[2];
		pTVar1 = gpTempEntActive;
		gpTempEntActive = pTVar2;
		pTVar2->next = pTVar1;
	}
	return pTVar2;
}

TEMPENTITY* CL_TempEntAllocNoModel(vec_t* org)
{
	cl_entity_t* dest;
	TEMPENTITY* pTVar1;
	TEMPENTITY* pTVar2;

	pTVar1 = gpTempEntFree;
	if (gpTempEntFree == NULL)
	{
		Con_DPrintf("Overflow %d temporary ents!\n", MAX_TEMP_ENTITIES);
	}
	else
	{
		dest = &gpTempEntFree->entity;
		gpTempEntFree = gpTempEntFree->next;
		Q_memset(dest, 0, 3000);
		pTVar1->flags = 0;
		(pTVar1->entity).curstate.colormap = 0;
		pTVar1->die = g_pcl.time + 0.75;
		(pTVar1->entity).model = NULL;
		(pTVar1->entity).curstate.rendermode = 0;
		pTVar1->flags = FTENT_NOMODEL;
		(pTVar1->entity).curstate.renderfx = 0;
		pTVar1->fadeSpeed = 0.5;
		pTVar1->hitSound = 0;
		pTVar1->clientIndex = -1;
		pTVar1->bounceFactor = 1.0;
		pTVar1->hitcallback = NULL;
		pTVar1->callback = NULL;
		pTVar1->priority = 0;
		(pTVar1->entity).origin[0] = *org;
		(pTVar1->entity).origin[1] = org[1];
		(pTVar1->entity).origin[2] = org[2];
		pTVar2 = gpTempEntActive;
		gpTempEntActive = pTVar1;
		pTVar1->next = pTVar2;
	}
	return pTVar1;
}

TEMPENTITY* CL_TempEntAllocHigh(vec_t* org, model_t* model)
{
	tempent_s** pptVar1;
	TEMPENTITY* pTVar2;
	tempent_s* ptVar3;
	TEMPENTITY* pTVar4;

	pTVar2 = gpTempEntActive;
	pTVar4 = gpTempEntFree;
	if (model == NULL)
	{
		pTVar4 = NULL;
		Con_DPrintf("temporary ent model invalid\n");
	}
	else if (gpTempEntFree == NULL)
	{
		ptVar3 = gpTempEntActive;
		if (gpTempEntActive != NULL)
		{
			do
			{
				if (ptVar3->priority == 0)
					goto INIT_ENTITY;
				ptVar3 = ptVar3->next;
			} while (ptVar3 != NULL);
		}
		Con_DPrintf("Couldn't alloc a high priority TENT!\n");
	}
	else
	{
		gpTempEntActive = gpTempEntFree;
		pptVar1 = &gpTempEntFree->next;
		gpTempEntFree = gpTempEntFree->next;
		*pptVar1 = pTVar2;
		ptVar3 = pTVar4;
	INIT_ENTITY:
		Q_memset(&ptVar3->entity, 0, 3000);
		(ptVar3->entity).curstate.colormap = 0;
		ptVar3->flags = FTENT_NONE;
		ptVar3->die = g_pcl.time + 0.75;
		(ptVar3->entity).curstate.rendermode = 0;
		(ptVar3->entity).model = model;
		(ptVar3->entity).curstate.renderfx = 0;
		ptVar3->fadeSpeed = 0.5;
		ptVar3->hitSound = 0;
		ptVar3->clientIndex = -1;
		ptVar3->bounceFactor = 1.0;
		ptVar3->hitcallback = NULL;
		ptVar3->callback = NULL;
		ptVar3->priority = 1;
		(ptVar3->entity).origin[0] = *org;
		(ptVar3->entity).origin[1] = org[1];
		(ptVar3->entity).origin[2] = org[2];
		pTVar4 = ptVar3;
	}
	return pTVar4;
}

TEMPENTITY* CL_AllocCustomTempEntity(float* origin, model_t* model, int high, void(*callback)(TEMPENTITY*, float, float))
{
	TEMPENTITY* tempent; // eax

	if (high)
		tempent = efx.CL_TempEntAllocHigh(origin, model);
	else
		tempent = efx.CL_TempEntAlloc(origin, model);

	if (tempent)
	{
		tempent->flags |= FTENT_CLIENTCUSTOM;
		tempent->callback = callback;
		tempent->die = g_pcl.time;
	}

	return tempent;
}

void CL_InitTEnts() // TODO: implement - ScriptedSnark
{
	Cvar_RegisterVariable(&tracerSpeed);
	Cvar_RegisterVariable(&tracerOffset);
	Cvar_RegisterVariable(&tracerLength);
	Cvar_RegisterVariable(&tracerRed);
	Cvar_RegisterVariable(&tracerGreen);
	Cvar_RegisterVariable(&tracerBlue);
	Cvar_RegisterVariable(&tracerAlpha);

	cl_sfx_ric1 = S_PrecacheSound("weapons/ric1.wav");
	cl_sfx_ric2 = S_PrecacheSound("weapons/ric2.wav");
	cl_sfx_ric3 = S_PrecacheSound("weapons/ric3.wav");
	cl_sfx_ric4 = S_PrecacheSound("weapons/ric4.wav");
	cl_sfx_ric5 = S_PrecacheSound("weapons/ric5.wav");
	cl_sfx_r_exp1 = S_PrecacheSound("weapons/explode3.wav");
	cl_sfx_r_exp2 = S_PrecacheSound("weapons/explode4.wav");
	cl_sfx_r_exp3 = S_PrecacheSound("weapons/explode5.wav");
	cl_sfx_pl_shell1 = S_PrecacheSound("player/pl_shell1.wav");
	cl_sfx_pl_shell2 = S_PrecacheSound("player/pl_shell2.wav");
	cl_sfx_pl_shell3 = S_PrecacheSound("player/pl_shell3.wav");
	cl_sfx_sshell1 = S_PrecacheSound("weapons/sshell1.wav");
	cl_sfx_sshell2 = S_PrecacheSound("weapons/sshell2.wav");
	cl_sfx_sshell3 = S_PrecacheSound("weapons/sshell3.wav");
	cl_sfx_wood1 = S_PrecacheSound("debris/wood1.wav");
	cl_sfx_wood2 = S_PrecacheSound("debris/wood2.wav");
	cl_sfx_wood3 = S_PrecacheSound("debris/wood3.wav");
	cl_sfx_metal1 = S_PrecacheSound("debris/metal1.wav");
	cl_sfx_metal2 = S_PrecacheSound("debris/metal2.wav");
	cl_sfx_metal3 = S_PrecacheSound("debris/metal3.wav");
	cl_sfx_glass1 = S_PrecacheSound("debris/glass1.wav");
	cl_sfx_glass2 = S_PrecacheSound("debris/glass2.wav");
	cl_sfx_glass3 = S_PrecacheSound("debris/glass3.wav");
	cl_sfx_concrete1 = S_PrecacheSound("debris/concrete1.wav");
	cl_sfx_concrete2 = S_PrecacheSound("debris/concrete2.wav");
	cl_sfx_concrete3 = S_PrecacheSound("debris/concrete3.wav");
	cl_sfx_flesh1 = S_PrecacheSound("debris/flesh1.wav");
	cl_sfx_flesh2 = S_PrecacheSound("debris/flesh2.wav");
	cl_sfx_flesh3 = S_PrecacheSound("debris/flesh3.wav");
	cl_sfx_flesh4 = S_PrecacheSound("debris/flesh5.wav");
	cl_sfx_flesh5 = S_PrecacheSound("debris/flesh6.wav");
	cl_sfx_flesh6 = S_PrecacheSound("debris/flesh7.wav");
	cl_sfx_geiger1 = S_PrecacheSound("player/geiger1.wav");
	cl_sfx_geiger2 = S_PrecacheSound("player/geiger2.wav");
	cl_sfx_geiger3 = S_PrecacheSound("player/geiger3.wav");
	cl_sfx_geiger4 = S_PrecacheSound("player/geiger4.wav");
	cl_sfx_geiger5 = S_PrecacheSound("player/geiger5.wav");
	cl_sfx_geiger6 = S_PrecacheSound("player/geiger6.wav");

	cl_sprite_dot = Mod_ForName("sprites/dot.spr", true, false);
	Mod_MarkClient(cl_sprite_dot);

	cl_sprite_lightning = Mod_ForName("sprites/lgtning.spr", true, false);
	Mod_MarkClient(cl_sprite_lightning);

	cl_sprite_white = Mod_ForName("sprites/white.spr", true, false);
	Mod_MarkClient(cl_sprite_white);

	cl_sprite_glow = Mod_ForName("sprites/animglow01.spr", true, false);
	Mod_MarkClient(cl_sprite_glow);

	cl_sprite_muzzleflash[0] = Mod_ForName("sprites/muzzleflash1.spr", true, false);
	Mod_MarkClient(cl_sprite_muzzleflash[0]);

	cl_sprite_muzzleflash[1] = Mod_ForName("sprites/muzzleflash2.spr", true, false);
	Mod_MarkClient(cl_sprite_muzzleflash[1]);

	cl_sprite_muzzleflash[2] = Mod_ForName("sprites/muzzleflash3.spr", true, false);
	Mod_MarkClient(cl_sprite_muzzleflash[2]);

	cl_sprite_ricochet = Mod_ForName("sprites/richo1.spr", true, false);
	Mod_MarkClient(cl_sprite_ricochet);

	cl_sprite_shell = Mod_ForName("sprites/shellchrome.spr", true, false);
	Mod_MarkClient(cl_sprite_shell);

	Q_memset(gTempEnts, 0, 1534000);
	for (int i = 1; i < 500; ++i)
	{
		gTempEnts[i-1].next = &gTempEnts[i];
	}

	gTempEnts[499].next = nullptr;
	gpTempEntFree = gTempEnts;
	gpTempEntActive = nullptr;
}

void CL_InitExtrap()
{
	Cvar_RegisterVariable(&ex_interp);
}

void R_BloodSprite(vec_t* org, int colorindex, int modelIndex, int modelIndex2, float size)
{
	NOT_IMPLEMENTED;
}

void R_BreakModel(float* pos, float* size, float* dir, float random, float life, int count, int modelIndex, char flags)
{
	NOT_IMPLEMENTED;
}

void R_Bubbles(vec_t* mins, vec_t* maxs, float height, int modelIndex, int count, float speed)
{
	NOT_IMPLEMENTED;
}

void R_BubbleTrail(vec_t* start, vec_t* end, float height, int modelIndex, int count, float speed)
{
	NOT_IMPLEMENTED;
}

void R_Explosion(float* pos, int model, float scale, float framerate, int flags)
{
	NOT_IMPLEMENTED;
}

void R_FizzEffect(cl_entity_t* pent, int modelIndex, int density)
{
	NOT_IMPLEMENTED;
}

void R_FireField(vec_t* org, int radius, int modelIndex, int count, int flags, float life)
{
	NOT_IMPLEMENTED;
}

void R_FunnelSprite(float* org, int modelIndex, int reverse)
{
	NOT_IMPLEMENTED;
}

void R_MultiGunshot(vec_t* org, vec_t* dir, vec_t* noise, int count, int decalCount, int* decalIndices)
{
	NOT_IMPLEMENTED;
}

void R_MuzzleFlash(float* pos1, int type)
{
	NOT_IMPLEMENTED;
}

void R_ParticleBox(float* mins, float* maxs, byte r, byte g, byte b, float life)
{
	NOT_IMPLEMENTED;
}

void R_ParticleLine(float* start, float* end, byte r, byte g, byte b, float life)
{
	NOT_IMPLEMENTED;
}

void R_PlayerSprites(int client, int modelIndex, int count, int size)
{
	NOT_IMPLEMENTED;
}

void R_Projectile(vec_t* origin, vec_t* velocity, int modelIndex, int life, int owner, void(*hitcallback)(TEMPENTITY*, pmtrace_t*))
{
	NOT_IMPLEMENTED;
}

void R_RicochetSound(vec_t* pos)
{
	switch (RandomLong(0, 4))
	{
	case 0:
		S_StartDynamicSound(-1, 0, cl_sfx_ric1, pos, 1.0, 1.0, 0, 100);
		break;
	case 1:
		S_StartDynamicSound(-1, 0, cl_sfx_ric2, pos, 1.0, 1.0, 0, 100);
		break;
	case 2:
		S_StartDynamicSound(-1, 0, cl_sfx_ric3, pos, 1.0, 1.0, 0, 100);
		break;
	case 3:
		S_StartDynamicSound(-1, 0, cl_sfx_ric4, pos, 1.0, 1.0, 0, 100);
		break;
	case 4:
		S_StartDynamicSound(-1, 0, cl_sfx_ric5, pos, 1.0, 1.0, 0, 100);
		break;
	default:
		return;
	}
}

void R_RicochetSprite(float* pos, model_t* pmodel, float duration, float scale)
{
	NOT_IMPLEMENTED;
}

void R_RocketFlare(float* pos)
{
	NOT_IMPLEMENTED;
}

void R_SparkEffect(float* pos, int count, int velocityMin, int velocityMax)
{
	NOT_IMPLEMENTED;
}

void R_SparkShower(float* pos)
{
	TEMPENTITY* tmpent = efx.CL_TempEntAllocNoModel(pos);

	if (tmpent)
	{
		tmpent->entity.baseline.origin[0] = RandomFloat(-300.0, 300.0);
		tmpent->entity.baseline.origin[1] = RandomFloat(-300.0, 300.0);
		tmpent->flags |= FTENT_SPARKSHOWER; // If it doesn't work correctly - use "0x20028u" instead "FTENT_SPARKSHOWER" - ScriptedSnark
		tmpent->entity.baseline.angles[0] = 0.0;
		tmpent->entity.baseline.angles[1] = 0.0;
		tmpent->entity.baseline.angles[2] = 0.0;
		tmpent->entity.baseline.origin[2] = RandomFloat(-200.0, 200.0);
		tmpent->die = g_pcl.time + 0.5;
		tmpent->entity.curstate.framerate = RandomFloat(0.5, 1.5);
		tmpent->entity.curstate.scale = g_pcl.time;
	}
}

void R_Spray(vec_t* pos, vec_t* dir, int modelIndex, int count, int speed, int spread, int rendermode)
{
	NOT_IMPLEMENTED;
}

void R_Sprite_Explode(TEMPENTITY* pTemp, float scale, int flags)
{
	NOT_IMPLEMENTED;
}

void R_Sprite_Smoke(TEMPENTITY* pTemp, float scale)
{
	NOT_IMPLEMENTED;
}

void R_Sprite_Spray(vec_t* pos, vec_t* dir, int modelIndex, int count, int speed, int iRand)
{
	NOT_IMPLEMENTED;
}

void R_Sprite_Trail(int type, vec_t* start, vec_t* end,
	int modelIndex, int count, float life, float size,
	float amplitude, int renderamt, float speed)
{
	NOT_IMPLEMENTED;
}

void R_Sprite_WallPuff(TEMPENTITY* pTemp, float scale)
{
	if (pTemp)
	{
		pTemp->entity.curstate.renderamt = 255;
		pTemp->entity.curstate.renderfx = 0;
		pTemp->entity.curstate.rendermode = 4;
		pTemp->entity.curstate.rendercolor.r = 0;
		pTemp->entity.curstate.rendercolor.g = 0;
		pTemp->entity.curstate.scale = scale;
		pTemp->entity.curstate.rendercolor.b = 0;
		pTemp->die = g_pcl.time + 0.01;
		pTemp->entity.curstate.frame = 0.0;
		pTemp->entity.angles[2] = (float)RandomLong(0, 359);
	}
}

void R_TracerEffect(vec_t* start, vec_t* end)
{
	NOT_IMPLEMENTED;
}

void R_TempSphereModel(float* pos, float speed, float life, int count, int modelIndex)
{
	NOT_IMPLEMENTED;
}

TEMPENTITY* R_TempModel(float* pos, float* dir, float* angles, float life, int modelIndex, int soundtype)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

TEMPENTITY* R_DefaultSprite(float* pos, int spriteIndex, float framerate)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

TEMPENTITY* R_TempSprite(float* pos, float* dir, float scale, int modelIndex, int rendermode, int renderfx, float a, float life, int flags)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

mspriteframe_t* R_GetSpriteFrame(msprite_t* pSprite, int frame)
{
	if (!pSprite)
	{
		Con_Printf("Sprite:  no pSprite!!!\n");
		return NULL;
	}

	if (!pSprite->numframes)
	{
		Con_Printf("Sprite:  pSprite has no frames!!!\n");
		return NULL;
	}

	if (frame >= pSprite->numframes || frame < 0)
		Con_DPrintf("Sprite: no such frame %d\n", frame);

	if (pSprite->frames[frame].type == SPR_SINGLE)
		return pSprite->frames[frame].frameptr;

	return NULL;
}

void R_AttachTentToPlayer2(int client, model_s* pModel, float zoffset, float life)
{
	model_s* pmVar1;
	TEMPENTITY* pTVar2;
	int iVar3;
	vec3_t position;

	if ((client > -1) && (client <= g_pcl.maxclients))
	{
		// TODO: impl - xWhitey
		if (true /*cl_entit.curstate.messagenum == cl.parsecount*/)
		{ // There's prob should be CL_GetEntityByIndex, but its definition doesn't exist here
		  // position[0] = cl_entities[client].origin[0];             } the same as             ^^^^^^^^^^^^
		  // position[2] = zoffset + cl_entities[client].origin[2];   }
		  // position[1] = cl_entities[client].origin[1];             }
			pTVar2 = (*efx.CL_TempEntAllocHigh)(position, pModel);
			if (pTVar2 != NULL)
			{
				(pTVar2->entity).curstate.rendermode = 0;
				pTVar2->tentOffset[2] = zoffset;
				(pTVar2->entity).curstate.renderfx = 0xe;
				(pTVar2->entity).curstate.renderamt = 0xff;
				(pTVar2->entity).baseline.renderamt = 0xff;
				(pTVar2->entity).curstate.framerate = 1.0;
				pTVar2->clientIndex = (short)client;
				pTVar2->tentOffset[0] = 0.0;
				pTVar2->tentOffset[1] = 0.0;
				pTVar2->die = life + (float)g_pcl.time;
				pmVar1 = (pTVar2->entity).model;
				pTVar2->flags = pTVar2->flags | 0xa000;
				if (pmVar1->type == mod_sprite)
				{
					// iVar3 = ModelFrameCount(pModel); TODO: impl - xWhitey
					pTVar2->flags = pTVar2->flags | 0x10100; // unknown flag?? - xWhitey
					(pTVar2->entity).curstate.framerate = 10.0;
					// pTVar2->frameMax = (float)iVar3;
				}
				else
				{
					pTVar2->frameMax = 0.0;
				}
				(pTVar2->entity).curstate.frame = 0.0;
				return;
			}
			Con_Printf("No temp ent.\n");
		}
		return;
	}
	Con_Printf("Bad client in R_AttachTentToPlayer()!\n");
	return;
}

void R_AttachTentToPlayer(int client, int modelIndex, float zoffset, float life)
{
	NOT_IMPLEMENTED;
	/*
	model_t* pModel;

	pModel = CL_GetModelByIndex(modelIndex); // TODO: impl - xWhitey
	if (pModel != (model_t*)0x0)
	{
		R_AttachTentToPlayer2(client, pModel, zoffset, life); // TODO: impl - xWhitey
		return;
	}
	Con_Printf("No model %d!\n");
	*/
	return;
}

void R_KillAttachedTents(int client)
{
	tempent_s* ptVar1;

	if ((client > -1) && (client <= g_pcl.maxclients))
	{
		ptVar1 = gpTempEntActive;
		if (gpTempEntActive != NULL)
		{
			do
			{
				while (((ptVar1->flags & FTENT_PLYRATTACHMENT) == 0 || (ptVar1->clientIndex != client)))
				{
					ptVar1 = ptVar1->next;
					if (ptVar1 == NULL)
					{
						return;
					}
				}
				ptVar1->die = g_pcl.time;
				ptVar1 = ptVar1->next;
			} while (ptVar1 != NULL);
		}
		return;
	}
	Con_Printf("Bad client in R_KillAttachedTents()!\n");
	return;
}


/* WARNING: Removing unreachable block (ram,0x001a88de) */
/* WARNING: Removing unreachable block (ram,0x001a877e) */
/* WARNING: Removing unreachable block (ram,0x001a7ed2) */

void CL_ParseTEnt(void)
{
	NOT_IMPLEMENTED;
	return;
}

