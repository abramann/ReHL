#pragma once
#include "vmodes.h"
#include <SDL.h>

// Multitexture
#define QGL_TEXTURE0_SGIS 0x835E
#define QGL_TEXTURE1_SGIS 0x835F
#define QGL_TEXTURE2_SGIS 0x8360

extern int window_center_x;
extern int window_center_y;

extern int TEXTURE0_SGIS;
extern int TEXTURE1_SGIS;
extern int TEXTURE2_SGIS;

extern int gl_mtexable;

extern const char* gl_vendor;
extern const char* gl_renderer;
extern const char* gl_version;
extern const char* gl_extensions;

extern const char* gl_renderer;

float GetXMouseAspectRatioAdjustment();

float GetYMouseAspectRatioAdjustment();

void GL_BeginRendering(int* x, int* y, int* width, int* height);

void GL_EndRendering();

//bool GL_SetMode(SDL_Window* mainwindow, HDC* pmaindc, HGLRC* pbaseRC, const char* pszDriver, const char* pszCmdLine);

bool VID_Init(unsigned short *palette);

void VID_UpdateWindowVars(rect_t* prc, int x, int y);

void VID_TakeSnapshotRect(const char* pFilename, int x, int y, int w, int h);

void VID_TakeSnapshot(const char* pFilename);

void VID_WriteBuffer(const char* pFilename);

void VID_SetPalette(unsigned char *palette);

void VID_ShiftPalette(unsigned char *palette);

extern qboolean scr_skipupdate;

extern qboolean scr_skiponeupdate;

extern bool g_bSupportsNPOTTextures;

