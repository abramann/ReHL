#pragma once

#include "model.h"


void R_ResetStudio();

int R_StudioCheckBBox();

void R_StudioDynamicLight(cl_entity_t *ent, alight_t *plight);

void R_StudioEntityLight(alight_t *plight);

void R_StudioSetupLighting(alight_t *plighting);

void R_StudioDrawPoints();

void R_StudioDrawHulls();

void R_StudioAbsBB();

void R_StudioDrawBones();

void R_StudioSetupSkin(studiohdr_t *ptexturehdr, int index);

void R_ResetStudio();