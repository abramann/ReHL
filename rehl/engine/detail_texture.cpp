#include "precompiled.h"
#include "detail_texture.h"
#include "qgl.h"
#include "gl_vidnt.h"



#ifdef SHARED_GAME_DATA
bool * sp_detTexSupported = ADDRESS_OF_DATA(bool *, 0x33057);
bool & detTexSupported = *sp_detTexSupported;

cvar_t * sp_r_detailtextures = ADDRESS_OF_DATA(cvar_t *, 0x32FD1);
cvar_t & r_detailtextures = *sp_r_detailtextures;

cvar_t * sp_r_detailtexturessupported = ADDRESS_OF_DATA(cvar_t *, 0x32FDB);
cvar_t & r_detailtexturessupported = *sp_r_detailtexturessupported;
#else
bool detTexSupported = false;

cvar_t r_detailtextures = { "r_detailtextures", "0", FCVAR_ARCHIVE };
cvar_t r_detailtexturessupported = { "r_detailtexturessupported", "1", FCVAR_SPONLY };
#endif

void DT_Initialize()
{
	Cvar_RegisterVariable(&r_detailtextures);
	Cvar_RegisterVariable(&r_detailtexturessupported);

	qglSelectTextureSGIS(TEXTURE2_SGIS);
	qglEnable(GL_TEXTURE_2D);
	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	qglTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
	qglTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 2.0);
	qglDisable(GL_TEXTURE_2D);
	qglSelectTextureSGIS(TEXTURE0_SGIS);

	detTexSupported = true;
}

void DT_ClearRenderState()
{
	qglSelectTextureSGIS(TEXTURE2_SGIS);
	qglDisable(GL_TEXTURE_2D);
	qglSelectTextureSGIS(TEXTURE1_SGIS);
}

void DT_LoadDetailMapFile(char * levelName)
{
	return Call_Function<void, char*>(0x334B0, levelName);
}
