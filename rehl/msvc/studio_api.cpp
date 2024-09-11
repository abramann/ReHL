#include "precompiled.h"

r_studio_interface_t studio;

r_studio_interface_t* pStudioAPI;

float lighttransform[128][3][4];

extern model_t* cl_sprite_shell;

int r_smodels_total;
int r_amodels_drawn;


int R_StudioCheckBBox()
{
	NOT_IMPLEMENTED;
	return 0;
}

void R_StudioDynamicLight(cl_entity_t *ent, alight_t *plight)
{
	NOT_IMPLEMENTED;
}

void R_StudioEntityLight(alight_t *plight)
{
	NOT_IMPLEMENTED;
}

void R_StudioSetupLighting(alight_t *plighting)
{
	NOT_IMPLEMENTED;
}

void R_StudioDrawPoints()
{
	NOT_IMPLEMENTED;

	//R_GLStudioDrawPoints();
}

void R_StudioDrawHulls()
{
	NOT_IMPLEMENTED;
}

void R_StudioAbsBB()
{
	NOT_IMPLEMENTED;
}

void R_StudioDrawBones()
{
	NOT_IMPLEMENTED;
}

void R_StudioSetupSkin(studiohdr_t *ptexturehdr, int index)
{
	NOT_IMPLEMENTED;
}

void R_ResetStudio()
{
	pStudioAPI = &studio;
}

model_s * legacy_Mod_ForName(const char *model, int crash)
{
	return Mod_ForName(model, (qboolean)crash, false);
}

model_t *studioapi_GetModelByIndex(int index)
{
	return CL_GetModelByIndex(index);
}

cl_entity_t *studioapi_GetCurrentEntity()
{
	NOT_IMPLEMENTED;
	return nullptr;
	//return currententity;
}

player_info_t *studioapi_PlayerInfo(int index)
{
	if (index >= 0 && index < g_pcl.maxclients)
		return &g_pcl.players[index];

	return nullptr;
}

entity_state_t *__cdecl studioapi_GetPlayerState(int index)
{
	if (index >= 0 && index < g_pcl.maxclients)
		return &g_pcl.frames[g_pcl.parsecountmod].playerstate[index];

	return nullptr;
}

cl_entity_t *studioapi_GetViewEntity()
{
	return &g_pcl.viewent;
}

void studioapi_GetTimes(int *framecount, double *cl_time, double *cl_oldtime)
{
	NOT_IMPLEMENTED;

	//*framecount = r_framecount;
	*cl_time = g_pcl.time;
	*cl_oldtime = g_pcl.oldtime;
}

cvar_s *studioapi_GetCvar(const char *name)
{
	return Cvar_FindVar(name);
}

void studioapi_GetViewInfo(float *origin, float *upv, float *rightv, float *vpnv)
{
	NOT_IMPLEMENTED;
	/*
	*origin = r_origin[0];
	origin[1] = r_origin[1];
	origin[2] = r_origin[2];
	*upv = vup[0];
	upv[1] = vup[1];
	upv[2] = vup[2];
	*rightv = vright[0];
	rightv[1] = vright[1];
	rightv[2] = vright[2];
	*vpnv = vpn[0];
	vpnv[1] = vpn[1];
	vpnv[2] = vpn[2];
	*/
}

model_t *studioapi_GetChromeSprite()
{
	return cl_sprite_shell;
}

void studioapi_GetModelCounters(int **s, int **a)
{
	*s = &r_smodels_total;
	*a = &r_amodels_drawn;
}

void studioapi_GetAliasScale(float *x, float *y)
{
	*y = 1.0;
	*x = 1.0;
}

float ****studioapi_StudioGetBoneTransform()
{
	return (float ****)bonetransform;
}

float ****studioapi_StudioGetLightTransform()
{
	return (float ****)lighttransform;
}

float ***studioapi_StudioGetAliasTransform()
{
	return nullptr;
}

float ***studioapi_StudioGetRotationMatrix()
{
	return (float ***)rotationmatrix;
}

void studioapi_SetupModel(int bodypart, void **ppbodypart, void **ppsubmodel)
{
	NOT_IMPLEMENTED;
}

void studioapi_StudioSetRemapColors(int top, int bottom)
{
	NOT_IMPLEMENTED;

	//r_topcolor = top;
	//r_bottomcolor = bottom;
}

model_t *studioapi_SetupPlayerModel(int playerindex)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

void R_StudioClientEvents()
{
	NOT_IMPLEMENTED;
}

int studioapi_GetForceFaceFlags()
{
	NOT_IMPLEMENTED;
	return 0;
	//return g_ForcedFaceFlags;
}

void studioapi_SetForceFaceFlags(int flags)
{
	NOT_IMPLEMENTED;

	//g_ForcedFaceFlags = flags;
}

void studioapi_StudioSetHeader(void *header)
{
	pstudiohdr = (studiohdr_t *)header;
}

void studioapi_SetRenderModel(model_s *model)
{
	NOT_IMPLEMENTED;
	//r_model = model;
}

void studioapi_SetupRenderer(int rendermode)
{
	NOT_IMPLEMENTED;
}

void studioapi_RestoreRenderer()
{
	NOT_IMPLEMENTED;
}

void studioapi_SetChromeOrigin()
{
	NOT_IMPLEMENTED;
	//g_ChromeOrigin[0] = r_origin[0];
	//g_ChromeOrigin[1] = r_origin[1];
	//g_ChromeOrigin[2] = r_origin[2];
}

int IsHardware()
{
	NOT_IMPLEMENTED;
	return 0;
	//return 2 - (gD3DMode == 0);
}

void studioapi_SetBackFaceCull(int iCull)
{
	NOT_IMPLEMENTED;

	//g_iBackFaceCull = iCull;
}

void studioapi_GL_StudioDrawShadow()
{
	NOT_IMPLEMENTED;
}

void studioapi_GL_SetRenderMode(int rendermode) 
{
	NOT_IMPLEMENTED;
}

void studioapi_StudioSetRenderamt(int iRenderamt)
{
	NOT_IMPLEMENTED;
}

void studioapi_RenderShadow(int iSprite, float *p1, float *p2, float *p3, float *p4)
{
	NOT_IMPLEMENTED;
}

engine_studio_api_t engine_studio_api =
{
	Mem_Calloc,
	Cache_Check,
	COM_LoadCacheFile,
	legacy_Mod_ForName,
	Mod_Extradata,
	studioapi_GetModelByIndex,
	studioapi_GetCurrentEntity,
	studioapi_PlayerInfo,
	studioapi_GetPlayerState,
	studioapi_GetViewEntity,
	studioapi_GetTimes,
	studioapi_GetCvar,
	studioapi_GetViewInfo,
	studioapi_GetChromeSprite,
	studioapi_GetModelCounters,
	studioapi_GetAliasScale,
	studioapi_StudioGetBoneTransform,
	studioapi_StudioGetLightTransform,
	studioapi_StudioGetAliasTransform,
	studioapi_StudioGetRotationMatrix,
	studioapi_SetupModel,
	R_StudioCheckBBox,
	R_StudioDynamicLight,
	R_StudioEntityLight,
	R_StudioSetupLighting,
	R_StudioDrawPoints,
	R_StudioDrawHulls,
	R_StudioAbsBB,
	R_StudioDrawBones,
	R_StudioSetupSkin,
	studioapi_StudioSetRemapColors,
	studioapi_SetupPlayerModel,
	R_StudioClientEvents,
	studioapi_GetForceFaceFlags,
	studioapi_SetForceFaceFlags,
	studioapi_StudioSetHeader,
	studioapi_SetRenderModel,
	studioapi_SetupRenderer,
	studioapi_RestoreRenderer,
	studioapi_SetChromeOrigin,
	IsHardware,
	studioapi_GL_StudioDrawShadow,
	studioapi_GL_SetRenderMode,
	studioapi_StudioSetRenderamt,
	studioapi_SetBackFaceCull,
	studioapi_RenderShadow
};