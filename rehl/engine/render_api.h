#pragma once
#include "cl_entity.h"
#include "model.h"
#include "decal.h"

typedef struct cshift_s
{
	int destcolor[3];
	int percent;
} cshift_t;

struct transObjRef
{
	cl_entity_t* pEnt;
	float distance;
};

EXTERN_VAR(qboolean, gl_texsort);
EXTERN_VAR(cl_entity_t , r_worldentity);
EXTERN_VAR(int , c_brush_polys);
EXTERN_VAR(int , c_alias_polys);
EXTERN_VAR(qboolean , mirror);
EXTERN_VAR(int, r_framecount);
EXTERN_VAR(int, gRenderMode);
EXTERN_VAR(int, mirrortexturenum);
EXTERN_VAR(particle_t*, active_particles);
EXTERN_VAR(particle_t*, free_particles);
EXTERN_VAR(unsigned short*, host_basepal);

EXTERN_VAR(cvar_t, ati_npatch);
EXTERN_VAR(cvar_t, gl_wireframe);
EXTERN_VAR(cvar_t, r_cachestudio);
EXTERN_VAR(cvar_t, r_cullsequencebox);
EXTERN_VAR(cvar_t, r_bmodelinterp);
EXTERN_VAR(cvar_t, r_norefresh);
EXTERN_VAR(cvar_t, r_lightmap);
EXTERN_VAR(cvar_t, r_fullbright);
EXTERN_VAR(cvar_t, r_decals);
EXTERN_VAR(cvar_t, sp_decals);
EXTERN_VAR(cvar_t, mp_decals);
EXTERN_VAR(cvar_t, r_drawentities);
EXTERN_VAR(cvar_t, r_drawviewmodel);
EXTERN_VAR(cvar_t, r_mirroralpha);
EXTERN_VAR(cvar_t, r_wateralpha);
EXTERN_VAR(cvar_t, r_dynamic);
EXTERN_VAR(cvar_t, r_novis);
EXTERN_VAR(cvar_t, r_speeds);
EXTERN_VAR(cvar_t, d_spriteskip);
EXTERN_VAR(cvar_t, r_wadtextures);
EXTERN_VAR(cvar_t, r_mmx);
EXTERN_VAR(cvar_t, r_traceglow);
EXTERN_VAR(cvar_t, r_glowshellfreq);
EXTERN_VAR(cvar_t, gl_clear);
EXTERN_VAR(cvar_t, gl_cull);
EXTERN_VAR(cvar_t, gl_affinemodels);
EXTERN_VAR(cvar_t, gl_dither);
EXTERN_VAR(cvar_t, gl_spriteblend);
EXTERN_VAR(cvar_t, gl_polyoffset);
EXTERN_VAR(cvar_t, gl_lightholes);
EXTERN_VAR(cvar_t, gl_keeptjunctions);
EXTERN_VAR(cvar_t, gl_wateramp);
EXTERN_VAR(cvar_t, gl_overbright);
EXTERN_VAR(cvar_t, gl_zmax);
EXTERN_VAR(cvar_t, gl_alphamin);
EXTERN_VAR(cvar_t, gl_flipmatrix);
EXTERN_VAR(cvar_t, gl_monolights);
EXTERN_VAR(cvar_t, gl_fog);

void R_RenderView();

void R_TimeRefresh_f(void);

void R_Envmap_f(void);

void R_ReadPointFile_f(void);

void R_InitParticles();

void R_InitParticleTexture();

void R_UploadEmptyTex();

void R_Clear();

void R_PreDrawViewModel();

qboolean RecursiveLightPoint(colorVec* out, mnode_t* node, vec_t* start, vec_t* end);

colorVec R_LightVec(vec_t* start, vec_t* end);

colorVec R_LightPoint(vec3_t p0);

void R_DrawViewModel();

void R_RenderScene();

void R_DrawWorld();

void DrawGLPolyScroll(msurface_t* psurface, cl_entity_t* pEntity);

void R_SetStackBase(void);

void R_Init(void);

void R_ForceCVars(qboolean multiplayer);

void R_InitSky(void);

void R_MarkLeaves(void);

void R_InitTextures(void);

void R_PolyBlend();

void R_PushDlights();

void R_SetupFrame();

void R_SetFrustum();

void R_SetupGL();

void R_ClearSkyBox();

void R_RecursiveWorldNode(mnode_t* node);

qboolean R_CullBox(vec_t* mins, vec_t* maxs);

void R_StoreEfrags(efrag_t** ppefrag);

void R_BlendLightmaps();

void R_DrawSequentialPoly(msurface_t* s, int face);

void DrawTextureChains();

void R_RenderBrushPoly(msurface_t* fa);

void R_MirrorChain(msurface_t* s);

void DrawGLSolidPoly(glpoly_t* p);

void DrawGLWaterPoly(glpoly_t* p);

void R_DrawSkyChain(msurface_t* s);

void R_DrawWaterChain(msurface_t* pChain);

void R_DrawSkyBox();

void MakeSkyVec(float s, float t, int axis);

void ClipSkyPolygon(int nump, vec_t* vecs, int stage);

void DrawSkyPolygon(int nump, vec_t* vecs);

void AddTEntity(cl_entity_t* pEnt);

void R_DrawBrushModel(cl_entity_t* e);

void R_SetRenderMode(cl_entity_t* pEntity);

void R_RotateForEntity(vec_t* origin, cl_entity_t* e);

float* R_GetAttachmentPoint(int entity, int attachment);

void R_DrawSpriteModel(cl_entity_t* e);

void R_SpriteColor(colorVec* pColor, cl_entity_t* pEntity, int alpha);

void R_GetSpriteAxes(cl_entity_t* pEntity, int type, vec_t* forward, vec_t* right, vec_t* up);

texture_t* R_TextureAnimation(msurface_t* s);

void EmitWaterPolys(msurface_t* fa, int direction);

void EmitWireFrameWaterPolys(msurface_t* fa, int direction);

void D_SetFadeColor(int r, int g, int b, int fog);

void R_RenderDynamicLightmaps(msurface_t* fa);

void R_DrawDecals(qboolean bMultitexture);

void R_DrawTEntitiesOnList(qboolean clientOnly);

float GlowBlend(cl_entity_t* pEntity);

void R_DrawAliasModel(cl_entity_t* e);

void R_RenderDlights();

void R_MarkLights(dlight_t* light, int bit, mnode_t* node);

void R_FreeDeadParticles(particle_t** ppparticles);

void R_DrawParticles();

void R_BeamDrawList();

void R_DrawBeamEntList(float frametime);

void R_BeamSetup(BEAM* pbeam, vec_t* start, vec_t* end, int modelIndex, float life, float width, float amplitude, float brightness, float speed);

int R_BeamCull(vec_t* start, vec_t* end, int pvsOnly);

void Noise(float* noise, int divs);

void SineNoise(float* noise, int divs);

cl_entity_t* R_GetBeamAttachmentEntity(int index);

void R_BeamDraw(BEAM* pbeam, float frametime);

void R_DrawSegs(vec_t* source, vec_t* delta, float width, float scale, float freq, float speed, int segments, int flags);

void R_DrawTorus(vec_t* source, vec_t* delta, float width, float scale, float freq, float speed, int segments);

void R_DrawDisk(vec_t* source, vec_t* delta, float width, float scale, float freq, float speed, int segments);

void R_DrawCylinder(vec_t* source, vec_t* delta, float width, float scale, float freq, float speed, int segments);

void R_DrawRing(vec_t* source, vec_t* delta, float width, float amplitude, float freq, float speed, int segments);

void R_DrawBeamFollow(BEAM* pbeam);

void R_DrawEntitiesOnList();

void R_Clear();

void R_CheckVariables();

void R_AnimateLight();

void GL_LoadFilterTexture(float r, float g, float b, float brightness);

int SignbitsForPlane(mplane_t* out);

void R_BuildLightMap(msurface_t* psurf, uchar* dest, int stride);

void R_AddDynamicLights(msurface_t* surf);

void R_RenderDlight(dlight_t* light);

void DrawGLWaterPolyLightmap(glpoly_t* p);

void AllowFog(qboolean allowed);

float CalcFov(float fov_x, float width, float height);

float ScrollOffset(msurface_t* psurface, cl_entity_t* pEntity);

void R_ConcatRotations(vec3_t* in1, vec3_t* in2, vec3_t* out);

void RotatePointAroundVector(vec3_t* dst, const vec_t* dir, const vec_t* point, float degrees);
