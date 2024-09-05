#pragma once

#include "cvar.h"


EXTERN_VAR(int, con_linewidth);
EXTERN_VAR(int, con_num_times);
EXTERN_VAR(void *, con_notifypos);
EXTERN_VAR(float *, con_times);
EXTERN_VAR(char *, con_text);
EXTERN_VAR(cvar_t, con_shifttoggleconsole);
EXTERN_VAR(cvar_t, con_color);
EXTERN_VAR(cvar_t, con_fastmode);
EXTERN_VAR(cvar_t, con_notifytime);
EXTERN_VAR(cvar_t, con_color);
EXTERN_VAR(cvar_t, con_mono);

const int CON_MAX_NOTIFY_STRING = 80;

void Con_CheckResize();

void EXT_FUNC Con_NPrintf(int idx, const char *fmt, ...);

void Con_Shutdown(void);

void Con_ClearNotify();

void Con_ToggleConsole_f();

void Con_SetTimes_f();

void Con_HideConsole_f();

void Con_MessageMode_f();

void Con_MessageMode2_f();

void Con_Clear_f();

void Con_DrawNotify();

int Con_IsVisible();

extern bool con_forcedup;

