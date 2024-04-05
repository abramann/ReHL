#include "precompiled.h"


#define SPR_MAX_SPRITES 256
unsigned short gSpritePalette[256];

HSPRITE_t ghCrosshair = 0;
wrect_t gCrosshairRc = {};
int gCrosshairR = 0;
int gCrosshairG = 0;
int gCrosshairB = 0;

static msprite_t* gpSprite = nullptr;

static int gSpriteCount = 0;
static SPRITELIST* gSpriteList = nullptr;

msprite_t* SPR_Pointer(SPRITELIST* pList)
{
	return (msprite_t*)pList->pSprite->cache.data;
}

void SetCrosshair(HSPRITE_t hspr, wrect_t rc, int r, int g, int b)
{
	g_engdstAddrs.pfnSetCrosshair(&hspr, &rc, &r, &g, &b);

	ghCrosshair = hspr;
	gCrosshairRc.left = rc.left;
	gCrosshairRc.right = rc.right;
	gCrosshairRc.top = rc.top;
	gCrosshairRc.bottom = rc.bottom;
	gCrosshairR = r;
	gCrosshairG = g;
	gCrosshairB = b;
}

void DrawCrosshair(int x, int y)
{
	NOT_IMPLEMENTED;
}

void SPR_Init()
{
	NOT_IMPLEMENTED;
}

void SPR_Shutdown()
{
	NOT_IMPLEMENTED_IGNORE;
}

void SPR_Shutdown_NoModelFree()
{
	NOT_IMPLEMENTED;
}

HSPRITE_t SPR_Load(const char* pTextureName)
{
	NOT_IMPLEMENTED;
	return 0;
}

int SPR_Frames(HSPRITE_t hSprite)
{
	NOT_IMPLEMENTED;
	return 0;
}

int SPR_Height(HSPRITE_t hSprite, int frame)
{
	NOT_IMPLEMENTED;
	return 0;
}

int SPR_Width(HSPRITE_t hSprite, int frame)
{
	NOT_IMPLEMENTED;
	return 0;
}

void SPR_Set(HSPRITE_t hSprite, int r, int g, int b)
{
	NOT_IMPLEMENTED;
}

void SPR_EnableScissor(int x, int y, int width, int height)
{
	NOT_IMPLEMENTED;
}

void SPR_DisableScissor()
{
	NOT_IMPLEMENTED;
}

void SPR_Draw(int frame, int x, int y, const wrect_t* prc)
{
	NOT_IMPLEMENTED;
}

void SPR_DrawHoles(int frame, int x, int y, const wrect_t* prc)
{
	NOT_IMPLEMENTED;
}

void SPR_DrawAdditive(int frame, int x, int y, const wrect_t* prc)
{
	NOT_IMPLEMENTED;
}

void SPR_DrawGeneric(int frame, int x, int y, const wrect_t* prc, int src, int dest, int width, int height)
{
	NOT_IMPLEMENTED;
}

client_sprite_t* SPR_GetList(char* psz, int* piCount)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

SPRITELIST* SPR_Get(HSPRITE_t hSprite)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

model_t* SPR_GetModelPointer(HSPRITE_t hSprite)
{
	auto pList = SPR_Get(hSprite);

	if (pList)
		return pList->pSprite;

	return nullptr;
}

void SetFilterMode(int mode)
{
	NOT_IMPLEMENTED;
}

void SetFilterColor(float r, float g, float b)
{
	NOT_IMPLEMENTED;
}

void SetFilterBrightness(float brightness)
{
	NOT_IMPLEMENTED;
}
