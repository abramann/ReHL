#pragma once

#include "cvar.h"

#ifdef SHARED_GAME_DATA
extern cvar_t& chase_active;
extern cvar_t& chase_active;
extern refdef_t& r_refdef;
#else
extern cvar_t chase_active;
extern cvar_t chase_active;
extern refdef_t r_refdef;
#endif
void Chase_Init();

void Chase_Reset();

void Chase_Update();

void TraceLine(vec_t* start, vec_t* end, vec_t* impact);

