#pragma once
#include "vmodes.h"
#include <SDL.h>

#ifdef SHARED_GAME_DATA

extern SDL_Window**& pmainwindow;
extern int& window_center_x;
extern int& window_center_y;

extern const char*& gl_extensions;
extern qboolean& g_bSupportsNPOTTextures;

extern int& TEXTURE0_SGIS;
extern int& TEXTURE1_SGIS;
extern int& TEXTURE2_SGIS;

extern int& gl_mtexable;

extern const char*& gl_vendor;
extern const char*& gl_renderer;
extern const char*& gl_version;

extern const char*& gl_renderer;
#else
extern SDL_Window** pmainwindow;
extern int window_center_x;
extern int window_center_y;
extern const char* gl_extensions;
extern qboolean g_bSupportsNPOTTextures;

extern int TEXTURE0_SGIS;
extern int TEXTURE1_SGIS;
extern int TEXTURE2_SGIS;

extern int gl_mtexable;

extern const char* gl_vendor;
extern const char* gl_renderer;
extern const char* gl_version;

extern const char* gl_renderer;
#endif

// Multitexture
#define QGL_TEXTURE0_SGIS 0x835E
#define QGL_TEXTURE1_SGIS 0x835F
#define QGL_TEXTURE2_SGIS 0x8360

float GetXMouseAspectRatioAdjustment();

float GetYMouseAspectRatioAdjustment();

void GL_BeginRendering(int* x, int* y, int* width, int* height);

void GL_EndRendering();

//bool GL_SetMode(SDL_Window* mainwindow, HDC* pmaindc, HGLRC* pbaseRC, int fD3D, const char* pszDriver, const char* pszCmdLine);

bool VID_Init(unsigned short *palette);

void VID_UpdateWindowVars(rect_t* prc, int x, int y);

void VID_TakeSnapshotRect(const char* pFilename, int x, int y, int w, int h);

void VID_TakeSnapshot(const char* pFilename);

void VID_WriteBuffer(const char* pFilename);

void VID_SetPalette(unsigned char *palette);

void VID_ShiftPalette(unsigned char *palette);

// Move these to gl_vidnt.cpp
void CheckTextureExtensions();
void CheckMultiTextureExtensions();
void CheckATINPatchExtensions();

extern qboolean scr_skipupdate;

extern qboolean scr_skiponeupdate;


