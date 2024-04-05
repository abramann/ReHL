#pragma once

#include "cvar.h"


const int CON_MAX_NOTIFY_STRING = 80;

void Con_ToggleConsole_f();

int Con_IsVisible();

extern bool con_forcedup;

extern cvar_t con_shifttoggleconsole;