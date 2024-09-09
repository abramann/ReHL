#pragma once
#include "hooker.h"

EXTERN_VAR(int, currenttexture)
EXTERN_ARRAY(int, cnttextures, [2])
EXTERN_VAR(GLenum, gl_lightmap_format);

void GL_BuildLightmaps();

