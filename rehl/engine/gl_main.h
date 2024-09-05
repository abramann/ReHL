#pragma once
#include "hooker.h"

EXTERN_VAR(int, currenttexture)
EXTERN_ARRAY(int, cnttextures, [2])

void GL_BuildLightmaps();

