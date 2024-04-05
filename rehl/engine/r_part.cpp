#include "precompiled.h"
#include <cmodel.h>

particle_t* free_particles;
particle_t* active_particles;

particle_t* R_AllocParticle(void(*callback)(particle_t*, float))
{
	particle_s* particles = free_particles; // eax

	if (!free_particles)
	{
		Con_Printf("Not enough free particles\n");
		return particles;
	}

	if (free_particles)
	{
		free_particles->type = pt_clientcustom;
		free_particles = free_particles->next;
		active_particles = particles;
		particles->next = active_particles;
		particles->die = g_pcl.time;
		particles->color = 0;
		particles->callback = callback;
		particles->deathfunc = 0;
		particles->ramp = 0.0;
		particles->org[0] = vec3_origin[0];
		particles->org[1] = vec3_origin[1];
		particles->org[2] = vec3_origin[2];
		particles->vel[0] = vec3_origin[0];
		particles->vel[1] = vec3_origin[1];
		particles->packedColor = 0;
		particles->vel[2] = vec3_origin[2];
	}

	return particles;
}

void R_BlobExplosion(vec_t* org)
{
	NOT_IMPLEMENTED;
}

void R_Blood(vec_t* org, vec_t* dir, int pcolor, int speed)
{
	NOT_IMPLEMENTED;
}

void R_BloodStream(vec_t* org, vec_t* dir, int pcolor, int speed)
{
	NOT_IMPLEMENTED;
}

void R_BulletImpactParticles(vec_t* pos)
{
	NOT_IMPLEMENTED;
}

void R_EntityParticles(cl_entity_t* ent)
{
	NOT_IMPLEMENTED;
}

void R_FlickerParticles(vec_t* org)
{
	NOT_IMPLEMENTED;
}

void R_Implosion(vec_t* end, float radius, int count, float life)
{
	NOT_IMPLEMENTED;
}

void R_LargeFunnel(vec_t* org, int reverse)
{
	NOT_IMPLEMENTED;
}

void R_LavaSplash(vec_t* org)
{
	NOT_IMPLEMENTED;
}

void R_ParticleBurst(vec_t* pos, int size, int color, float life)
{
	NOT_IMPLEMENTED;
}

void R_ParticleExplosion(vec_t* org)
{
	NOT_IMPLEMENTED;
}

void R_ParticleExplosion2(vec_t* org, int colorStart, int colorLength)
{
	NOT_IMPLEMENTED;
}

void R_RocketTrail(vec_t* start, vec_t* end, int type)
{
	NOT_IMPLEMENTED;
}

void R_RunParticleEffect(vec_t* org, vec_t* dir, int color, int count)
{
	NOT_IMPLEMENTED;
}

void R_ShowLine(vec_t* start, vec_t* end)
{
	NOT_IMPLEMENTED;
}

void R_SparkStreaks(vec_t* pos, int count, int velocityMin, int velocityMax)
{
	NOT_IMPLEMENTED;
}

void R_StreakSplash(vec_t* pos, vec_t* dir, int color, int count,
	float speed, int velocityMin, int velocityMax)
{
	NOT_IMPLEMENTED;
}

void R_UserTracerParticle(vec_t* org, vec_t* vel,
	float life, int colorIndex, float length,
	byte context, void(*deathfunc)(particle_t*))
{
	NOT_IMPLEMENTED;
}

particle_t* R_TracerParticles(vec_t* org, vec_t* vel, float life)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

void R_TeleportSplash(vec_t* org)
{
	NOT_IMPLEMENTED;
}

BEAM* R_BeamCirclePoints(int type, vec_t* start, vec_t* end,
	int modelIndex, float life, float width, float amplitude,
	float brightness, float speed, int startFrame, float framerate,
	float r, float g, float b)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

BEAM* R_BeamEntPoint(int startEnt, vec_t* end,
	int modelIndex, float life, float width, float amplitude,
	float brightness, float speed, int startFrame, float framerate,
	float r, float g, float b)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

BEAM* R_BeamEnts(int startEnt, int endEnt,
	int modelIndex, float life, float width, float amplitude,
	float brightness, float speed, int startFrame, float framerate,
	float r, float g, float b)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

BEAM* R_BeamFollow(int startEnt,
	int modelIndex, float life, float width,
	float r, float g, float b,
	float brightness)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

void R_BeamKill(int deadEntity)
{
	NOT_IMPLEMENTED;
}

BEAM* R_BeamLightning(vec_t* start, vec_t* end,
	int modelIndex, float life, float width, float amplitude,
	float brightness, float speed)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

BEAM* R_BeamPoints(vec_t* start, vec_t* end,
	int modelIndex, float life, float width, float amplitude,
	float brightness, float speed, int startFrame, float framerate,
	float r, float g, float b)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

BEAM* R_BeamRing(int startEnt, int endEnt,
	int modelIndex, float life, float width, float amplitude,
	float brightness, float speed, int startFrame, float framerate,
	float r, float g, float b)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

void R_GetPackedColor(short* packed, short color)
{
	if (packed)
		*packed = 0;
	else
		Con_Printf("R_GetPackedColor called without packed!\n");
}

short R_LookupColor(byte r, byte g, byte b)
{
	NOT_IMPLEMENTED;
	return 0;
}



void R_ParseParticleEffect(void)

{
	int iVar1;
	int iVar2;
	int color;
	vec3_t org;
	vec3_t dir;

	org[0] = MSG_ReadCoord();
	org[1] = MSG_ReadCoord();
	org[2] = MSG_ReadCoord();
	iVar1 = MSG_ReadChar();
	dir[0] = (float)iVar1 * 0.0625;
	iVar1 = MSG_ReadChar();
	dir[1] = (float)iVar1 * 0.0625;
	iVar1 = MSG_ReadChar();
	dir[2] = (float)iVar1 * 0.0625;
	iVar2 = MSG_ReadByte();
	color = MSG_ReadByte();
	iVar1 = 0x400;
	if (iVar2 != 0xff) {
		iVar1 = iVar2;
	}
	R_RunParticleEffect(org, dir, color, iVar1);
	return;
}


void R_KillDeadBeams(int deadEntity)

{
	/*
	BEAM* pBVar1;
	particle_s* ppVar2;
	BEAM* dest;
	BEAM* pBVar3;

	pBVar3 = (BEAM*)0x0;
	pBVar1 = gpActiveBeams;
	if (gpActiveBeams != (BEAM*)0x0) {
	do {
	while ((dest = pBVar1, pBVar1 = dest->next, dest->startEntity != deadEntity ||
	(dest->flags = dest->flags & 0xfffffffc, dest->type == 0x16))) {
	dest->next = pBVar3;
	pBVar3 = dest;
	if (pBVar1 == (BEAM*)0x0) {
	gpActiveBeams = dest;
	return;
	}
	}
	dest->die = (float)cl.time - 0.1;
	for (ppVar2 = dest->particles; ppVar2 != (particle_s*)0x0; ppVar2 = ppVar2->next) {
	ppVar2->die = (float)cl.time - 0.1;
	}
	R_FreeDeadParticles(&dest->particles);
	Q_memset(dest, 0, 0x7c);
	dest->next = R_BeamAlloc::pbeam;
	R_BeamAlloc::pbeam = dest;
	} while (pBVar1 != (BEAM*)0x0);
	}
	gpActiveBeams = pBVar3;*/
	return;
}

