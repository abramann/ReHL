#pragma once

#include "wad.h"

extern int clearnotify;
extern float scr_centertime_off;
extern float scr_con_current;
extern float scr_fov_value;

extern bool scr_copytop;

extern int glx;
extern int gly;
extern int glwidth;
extern int glheight;

extern cvar_t scr_viewsize;

void SCR_Init();

void SCR_DrawConsole();

void SCR_CenterPrint(const char* str);

void Draw_CenterPic(qpic_t* pPic);

void SCR_DrawLoading();

void SCR_UpdateScreen();

void SCR_BeginLoadingPlaque(bool reconnect);

void SCR_EndLoadingPlaque();

void SCR_CalcRefdef();

void Sbar_Draw();

int ScreenTransform(vec_t *point, vec_t *screen);
