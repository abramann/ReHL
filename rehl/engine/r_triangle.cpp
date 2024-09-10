#include "precompiled.h"

VVAR(qboolean, gD3DMode, 0x4DA37, false);
VVAR(GLfloat, flFogDensity, 0x88E85, 0);
ARRAY(float, flFinalFogColor, [4], 0x88EA6);
VVAR(GLfloat, flFogEnd, 0x88EC9, 0);
VVAR(GLfloat, flFogStart, 0x88EB7, 0);


triangleapi_t tri =
{
	TRI_API_VERSION,
	&tri_GL_RenderMode,
	&tri_GL_Begin,
	&tri_GL_End,
	&tri_GL_Color4f,
	&tri_GL_Color4ub,
	&tri_GL_TexCoord2f,
	&tri_GL_Vertex3fv,
	&tri_GL_Vertex3f,
	&tri_GL_Brightness,
	&tri_GL_CullFace,
	&R_TriangleSpriteTexture,
	&tri_ScreenTransform,
	&R_RenderFog,
	&tri_WorldTransform,
	&tri_GetMatrix,
	&tri_BoxinPVS,
	&tri_LightAtPoint,
	&tri_GL_Color4fRendermode,
	&R_FogParams
};

void tri_GL_RenderMode(int mode)
{
	switch (mode)
	{
	case kRenderNormal:
		qglDisable(GL_BLEND);
		qglDepthMask(GL_TRUE);
		qglShadeModel(GL_FLAT);
	case kRenderTransColor:
	case kRenderTransTexture:
		qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		qglEnable(GL_BLEND);
		qglShadeModel(GL_SMOOTH);
		break;
	case kRenderTransAlpha:
		qglEnable(GL_BLEND);
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448.0);
		qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		qglShadeModel(GL_SMOOTH);
		qglDepthMask(GL_FALSE);
		break;
	case kRenderTransAdd:
		qglBlendFunc(1, 1);
		qglEnable(GL_BLEND);
		qglDepthMask(GL_FALSE);
		qglShadeModel(GL_SMOOTH);
		break;
	default:
		break;
	}
	gRenderMode = mode;
}

void tri_GL_Begin(int primitiveCode)
{
	NOT_IMPLEMENTED;
}

void tri_GL_End()
{
	NOT_IMPLEMENTED;
}

void tri_GL_Color4f(float x, float y, float z, float w)
{
	NOT_IMPLEMENTED;
}

void tri_GL_Color4ub(byte r, byte g, byte b, byte a)
{
	NOT_IMPLEMENTED;
}

void tri_GL_TexCoord2f(float u, float v)
{
	NOT_IMPLEMENTED;
}

void tri_GL_Vertex3fv(float* worldPnt)
{
	NOT_IMPLEMENTED;
}

void tri_GL_Vertex3f(float x, float y, float z)
{
	NOT_IMPLEMENTED;
}

void tri_GL_Brightness(float x)
{
	NOT_IMPLEMENTED;
}

void tri_GL_CullFace(TRICULLSTYLE style)
{
	NOT_IMPLEMENTED;
}

int R_TriangleSpriteTexture(model_t* pSpriteModel, int frame)
{
	NOT_IMPLEMENTED;
	return 0;
}

int tri_ScreenTransform(float* world, float* screen)
{
	NOT_IMPLEMENTED;
	return 0;
}

void R_RenderFog(float* flFogColor, float flStart, float flEnd, int bOn)
{
	NOT_IMPLEMENTED;
}

void R_RenderFinalFog()
{
	if (gD3DMode != 1)
	{
		glEnable(GL_FOG);
		qglFogi(GL_FOG_MODE, GL_EXP2);
		qglFogf(GL_FOG_DENSITY, flFogDensity);
		qglHint(GL_FOG_HINT, GL_NICEST);
		qglFogfv(GL_FOG_COLOR, flFinalFogColor);
		qglFogf(GL_FOG_START, flFogStart);
		qglFogf(GL_FOG_END, flFogEnd);
	}
}

void tri_WorldTransform(float* screen, float* world)
{
	NOT_IMPLEMENTED;
}

void tri_GetMatrix(const int pname, float* matrix)
{
	NOT_IMPLEMENTED;
}

int tri_BoxinPVS(float* mins, float* maxs)
{
	NOT_IMPLEMENTED;
	return 0;
}

void tri_LightAtPoint(float* vPos, float* value)
{
	NOT_IMPLEMENTED;
}

void tri_GL_Color4fRendermode(float x, float y, float z, float w, int rendermode)
{
	NOT_IMPLEMENTED;
}

void R_FogParams(float flDensity, int iFogSkybox)
{
	NOT_IMPLEMENTED;
}
