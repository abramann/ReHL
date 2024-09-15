#include "precompiled.h"
#include "detail_texture.h"
#include "qgl.h"
#include "gl_vidnt.h"

VVAR(bool, detTexSupported, 0x33057, false);
VVAR(cvar_t, r_detailtextures, 0x32FD1, { "r_detailtextures" COMMA  "0" COMMA  FCVAR_ARCHIVE });
VVAR(cvar_t, r_detailtexturessupported, 0x32FDB, { "r_detailtexturessupported" COMMA  "1" COMMA  FCVAR_SPONLY });


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

int DT_SetRenderState(int diffuseId)
{
	TO_IMPLEMENT;
	return Call_Function<int, int>(0x33CA0, diffuseId);
}

void DT_SetTextureCoordinates(float u, float v)
{
	qglMTexCoord2fSGIS(TEXTURE2_SGIS, u, v);
}
