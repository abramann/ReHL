#pragma once
#include "cl_entity.h"

typedef struct glpoly_s
{
	glpoly_s* next;
	glpoly_s* chain;
	int numverts;
	int flags;
	float verts[4][7];
} glpoly_t;

extern int r_pixbytes;

EXTERN_VAR(qboolean, gl_texsort);
EXTERN_VAR(cl_entity_t , r_worldentity);
EXTERN_VAR(int , c_brush_polys);
EXTERN_VAR(int , c_alias_polys);
EXTERN_VAR(qboolean , mirror);
EXTERN_VAR(int, r_framecount);

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

void R_DrawViewModel();

void R_RenderScene();

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

void R_BlendLightmaps();

void DrawTextureChains();

void R_DrawDecals(qboolean bMultitexture);

void AllowFog(int allowed);

float CalcFov(float* fov_x, float width, float height);
