#include "precompiled.h"

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
	NOT_IMPLEMENTED;
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
	NOT_IMPLEMENTED;
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
