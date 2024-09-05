#pragma once

#include "cvar.h"
#include "hooker.h"

EXTERN_VAR(cvar_t, chase_active);
EXTERN_VAR(refdef_t, r_refdef);

void Chase_Init();

void Chase_Reset();

void Chase_Update();

void TraceLine(vec_t* start, vec_t* end, vec_t* impact);

