#pragma once

#include "qgl.h"
#include "vmodes.h"
#include "wad.h"

#define MAX_GLTEXTURES 4800

enum GL_TEXTURETYPE
{
	GLT_SYSTEM,
	GLT_DECAL,
	GLT_HUDSPRITE,
	GLT_STUDIO,
	GLT_WORLD,
	GLT_SPRITE,
};

extern int g_currentpalette;
extern cvar_t gl_round_down;
extern cvar_t gl_picmip;
extern cvar_t gl_palette_tex;
extern cvar_t gl_texturemode;
extern cvar_t gl_ansio;
extern cvar_t gl_max_size;

extern cvarhook_t gl_texturemode_hook;

extern qboolean giScissorTest;
extern GLenum oldtarget;

extern GLint scissor_x;
extern GLint scissor_y;
extern GLsizei scissor_width;
extern GLsizei scissor_height;

void gl_texturemode_hook_callback(cvar_t *cvar);

void Draw_Init();

void Draw_FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a);

int Draw_Character(int x, int y, int num, unsigned int font);

int Draw_MessageCharacterAdd(int x, int y, int num, int rr, int gg, int bb, unsigned int font);

int Draw_String(int x, int y, char* str);

int Draw_StringLen(const char* psz, unsigned int font);

void Draw_SetTextColor(float r, float g, float b);

void Draw_GetDefaultColor();

void Draw_ResetTextColor();

void Draw_FillRGBABlend(int x, int y, int w, int h, int r, int g, int b, int a);

GLuint GL_GenTexture();

void GL_SelectTexture(GLenum target);

void GL_Bind(int texnum);

void GL_Texels_f();

int GL_LoadTexture(char *identifier, GL_TEXTURETYPE textureType, int width, int height, unsigned char *data, int mipmap, int iType, unsigned char *pPal);

int GL_LoadTexture2(char *identifier, GL_TEXTURETYPE textureType, int width, int height, unsigned char *data, qboolean mipmap, int iType, unsigned char *pPal, int filter);

void GL_Upload16(char *data, int width, int height, qboolean mipmap, int iType, unsigned char *pPal, int filter);

void GL_Upload32(unsigned int *data, int width, int height, qboolean mipmap, int iType, int filter);

bool ValidateWRect(const wrect_t* prc);
bool IntersectWRect(const wrect_t* prc1, const wrect_t* prc2, wrect_t* prc);

void AdjustSubRect(mspriteframe_t* pFrame, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom, int* pw, int* ph, const wrect_t* prcSubRect);

void Draw_SpriteFrame(mspriteframe_t* pFrame, unsigned short* pPalette, int x, int y, const wrect_t* prcSubRect);
void Draw_SpriteFrameHoles(mspriteframe_t* pFrame, unsigned short* pPalette, int x, int y, const wrect_t* prcSubRect);
void Draw_SpriteFrameAdditive(mspriteframe_t* pFrame, unsigned short* pPalette, int x, int y, const wrect_t* prcSubRect);
void Draw_SpriteFrameGeneric(mspriteframe_t* pFrame, unsigned short* pPalette, int x, int y, const wrect_t* prcSubRect, int src, int dest, int width, int height);
void Draw_Pic(int x, int y, qpic_t* pic);

void Draw_BeginDisc();

void DT_Initialize();
void DT_ClearRenderState();