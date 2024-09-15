#pragma once
#include "hooker.h"

struct glRect_t
{
	int l;
	int t;
	int w;
	int h;
};

EXTERN_VAR(int, currenttexture)
EXTERN_VAR(GLint, lightmap_used);
EXTERN_VAR(int, lightmap_bytes);
EXTERN_ARRAY(int, cnttextures, [2])
EXTERN_VAR(GLenum, gl_lightmap_format);
EXTERN_VAR(qboolean, mtexenabled);
EXTERN_ARRAY(int, lightmap_textures, [64]);
EXTERN_ARRAY(qboolean, lightmap_modified, [64]);
EXTERN_ARRAY(uchar, lightmaps, [4194304]);
EXTERN_ARRAY(glRect_t, lightmap_rectchange, [64]);

void GL_BuildLightmaps();

int AllocBlock(int w, int h, int* x, int* y);

void GL_DisableMultitexture();

void GL_EnableMultitexture();

void GL_DisableMultitexture();

void GL_EnableMultitexture();
