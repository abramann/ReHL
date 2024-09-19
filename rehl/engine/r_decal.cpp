#include "precompiled.h"

static decal_t gDecalPool[4096];
static decalcache_t gDecalCache[256];

void R_DecalInit()
{
	NOT_IMPLEMENTED;
}

int R_DecalIndex(decal_t * pdecal)
{
	NOT_IMPLEMENTED; 
	return 0;
}

int R_DecalCacheIndex(int index)
{
	NOT_IMPLEMENTED;
	return 0;
}

decalcache_t * R_DecalCacheSlot(int decalIndex)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

void R_DecalCacheClear(decal_t * pdecal)
{
	NOT_IMPLEMENTED;
}

void R_DecalUnlink(decal_t * pdecal)
{
	NOT_IMPLEMENTED;
}

decal_t R_DecalAlloc(decal_t * pdecal)
{
	NOT_IMPLEMENTED;
	return decal_t();
}

void R_DecalRemoveWithFlag(int flags)
{
	NOT_IMPLEMENTED;
}

int R_DecalUnProject(decal_t * pdecal, vec_t * position)
{
	NOT_IMPLEMENTED;
	return 0;
}

decal_t * R_DecalIntersect(msurface_t * psurf, int * pcount, float x, float y)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

void R_DecalCreate(msurface_t * psurface, int textureIndex, float scale, float x, float y)
{
	NOT_IMPLEMENTED;
}

void R_DecalNode(mnode_t * node, float flScale)
{
	NOT_IMPLEMENTED;
}

void R_DecalShoot_(texture_t * ptexture, int index, int entity, int modelIndex, vec_t * position, int flags, float flScale)
{
	NOT_IMPLEMENTED;
}

void R_DecalShootScaled(int textureIndex, int entity, int modelIndex, vec_t * position, int flags, float flScale)
{
	NOT_IMPLEMENTED;
}

void R_DecalShoot(int textureIndex, int entity, int modelIndex, vec_t * position, int flags)
{
	NOT_IMPLEMENTED;
}

void R_DecalVertsLight(float * v, msurface_t * psurf, int vertCount)
{
	TO_IMPLEMENT;
	return Call_Function<void, float*, msurface_t*, int>(0x4B410, v, psurf, vertCount);
}

float* R_DecalVertsClip(float *poutVerts, decal_t *pdecal, msurface_t *psurf, texture_t *ptexture, int *pvertCount)
{
	TO_IMPLEMENT;
	return Call_Function<float*, float*, decal_t*, msurface_t*, texture_t*, int*>(0x4B510, poutVerts, pdecal, psurf, ptexture, pvertCount);
}
float* R_DecalVertsNoclip(decal_t *pdecal, msurface_t *psurf, texture_t *ptexture, qboolean bMultitexture)
{
	TO_IMPLEMENT;;
	return Call_Function<float*, decal_t*, msurface_t*, texture_t*, qboolean>(0x4B720, pdecal, psurf, ptexture, bMultitexture);
}

void R_DecalPoly(float *v, texture_t *ptexture, msurface_t *psurf, int vertCount)
{
	TO_IMPLEMENT;
	return Call_Function<void, float*, texture_t*, msurface_t*, int>(0x4B780, v, ptexture, psurf, vertCount);
}

void R_DecalMPoly(float *v, texture_t *ptexture, msurface_t *psurf, int vertCount)
{
	TO_IMPLEMENT;
	return Call_Function<void, float*, texture_t*, msurface_t*, int>(0x4B7E0, v, ptexture, psurf, vertCount);
}

void R_DecalRemoveNonPermanent(int textureIndex)
{
	NOT_IMPLEMENTED;
}

void R_FireCustomDecal(int textureIndex, int entity, int modelIndex, vec_t * position, int flags, float scale)
{
	NOT_IMPLEMENTED;
}

void R_DecalRemoveAll(int textureIndex)
{
	NOT_IMPLEMENTED;
}
