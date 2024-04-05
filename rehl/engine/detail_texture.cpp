#include "precompiled.h"
#include "detail_texture.h"
#include "qgl.h"
#include "gl_vidnt.h"


bool detTexSupported = false;

cvar_t r_detailtextures = { "r_detailtextures", "0", FCVAR_ARCHIVE };
cvar_t r_detailtexturessupported = { "r_detailtexturessupported", "1", FCVAR_SPONLY };

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