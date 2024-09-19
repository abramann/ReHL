#pragma once

#include "decal.h"

struct decalcache_t
{
	int decalIndex;
	float decalVert[4][7];
};


void R_DecalInit();

int R_DecalIndex(decal_t *pdecal);

int R_DecalCacheIndex(int index);

decalcache_t * R_DecalCacheSlot(int decalIndex);

void R_DecalCacheClear(decal_t *pdecal);

void R_DecalUnlink(decal_t *pdecal);

decal_t  R_DecalAlloc(decal_t *pdecal);

void R_DecalRemoveWithFlag(int flags);

int R_DecalUnProject(decal_t *pdecal, vec_t *position);

decal_t * R_DecalIntersect(msurface_t *psurf, int *pcount, float x, float y);

void R_DecalCreate(msurface_t *psurface, int textureIndex, float scale, float x, float y);

void R_DecalNode(mnode_t *node, float flScale);

void R_DecalShoot_(texture_t *ptexture, int index, int entity, int modelIndex, vec_t *position, int flags, float flScale);

void R_DecalShootScaled(int textureIndex, int entity, int modelIndex, vec_t *position, int flags, float flScale);

void R_DecalShoot(int textureIndex, int entity, int modelIndex, vec_t *position, int flags);

void R_DecalVertsLight(float *v, msurface_t *psurf, int vertCount);

float* R_DecalVertsClip(float *poutVerts, decal_t *pdecal, msurface_t *psurf, texture_t *ptexture, int *pvertCount);

float* R_DecalVertsNoclip(decal_t *pdecal, msurface_t *psurf, texture_t *ptexture, qboolean bMultitexture);

void R_DecalPoly(float *v, texture_t *ptexture, msurface_t *psurf, int vertCount);

void R_DecalMPoly(float *v, texture_t *ptexture, msurface_t *psurf, int vertCount);

void R_DecalRemoveNonPermanent(int textureIndex);

void R_FireCustomDecal(int textureIndex, int entity, int modelIndex, vec_t* position, int flags, float scale);

void R_DecalRemoveAll(int textureIndex);
