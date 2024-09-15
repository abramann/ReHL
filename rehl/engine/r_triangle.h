#pragma once

#include "triangleapi.h"
#include "model.h"

#include <glew.h>

EXTERN_VAR(triangleapi_t, tri);

EXTERN_VAR(GLfloat, flFogDensity);
EXTERN_ARRAY(float, flFinalFogColor, [4]);
EXTERN_VAR(GLfloat, flFogEnd);
EXTERN_VAR(GLfloat, flFogStart);

void tri_GL_RenderMode(int mode);

void tri_GL_Begin(int primitiveCode);

void tri_GL_End();

void tri_GL_Color4f(float x, float y, float z, float w);

void tri_GL_Color4ub(byte r, byte g, byte b, byte a);

void tri_GL_TexCoord2f(float u, float v);

void tri_GL_Vertex3fv(float* worldPnt);

void tri_GL_Vertex3f(float x, float y, float z);

void tri_GL_Brightness(float x);

void tri_GL_CullFace(TRICULLSTYLE style);

int R_TriangleSpriteTexture(model_t* pSpriteModel, int frame);

int tri_ScreenTransform(float* world, float* screen);

void R_RenderFog(float* flFogColor, float flStart, float flEnd, int bOn);

void R_RenderFinalFog();

void tri_WorldTransform(float* screen, float* world);

void tri_GetMatrix(const int pname, float* matrix);

int tri_BoxinPVS(float* mins, float* maxs);

void tri_LightAtPoint(float* vPos, float* value);

void tri_GL_Color4fRendermode(float x, float y, float z, float w, int rendermode);

void R_FogParams(float flDensity, int iFogSkybox);