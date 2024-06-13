#pragma once

#include "cvar.h"


#ifdef SHARED_GAME_DATA
extern int& con_linewidth;
extern int& con_num_times;
extern void*& con_notifypos;
extern float*& con_times;
extern char*& con_text;
extern cvar_t& con_shifttoggleconsole;
extern cvar_t& con_color;

extern cvar_t& con_fastmode;
extern cvar_t& con_notifytime;
extern cvar_t& con_color;
extern cvar_t& con_mono;
#else
extern int con_linewidth;
extern int con_num_times;
extern void *con_notifypos;
extern float *con_times;
extern char *con_text;
extern cvar_t con_shifttoggleconsole;
extern cvar_t con_color;
extern cvar_t con_fastmode;
extern cvar_t con_notifytime;
extern cvar_t con_color;
extern cvar_t con_mono;
#endif

const int CON_MAX_NOTIFY_STRING = 80;

void Con_CheckResize();

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

