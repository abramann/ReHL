#pragma once

#include "cvar.h"

extern cvar_t chase_active;

extern refdef_t r_refdef;

void Chase_Init();

void Chase_Reset();

void Chase_Update();

void TraceLine(vec_t* start, vec_t* end, vec_t* impact);

extern cvar_t chase_active;