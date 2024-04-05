#include "precompiled.h"
#include "qgl.h"
#include "text_draw.h"
#include "gl_vidnt.h"
#include "gl_draw.h"
#include "gl_main.h"


int g_currentpalette;
cvar_t gl_ansio = { "gl_ansio", "16" };

qpic_t* draw_disc = nullptr;

#define MAX_CACHED_PICS 128

typedef struct gltexture_s
{
	int texnum;
	short servercount;
	short paletteIndex;
	int width;
	int height;
	qboolean mipmap;
	char identifier[64];
} gltexture_t;

static gltexture_t gltextures[MAX_GLTEXTURES];

struct GL_PALETTE
{
	int tag;
	int referenceCount;
	unsigned __int8 colors[768];
};

GL_PALETTE gGLPalette[350];
static int numgltextures = 0;

static int scissor_x = 0, scissor_y = 0, scissor_width = 0, scissor_height = 0;
static qboolean giScissorTest = false;

void EnableScissorTest(int x, int y, int width, int height)
{
	x = clamp<int>(x, 0, vid.width);
	y = clamp<int>(y, 0, vid.height);
	width = clamp<int>(width, 0, vid.width - x);
	height = clamp<int>(height, 0, vid.height - y);

	scissor_x = x;
	scissor_width = width;
	scissor_y = y;
	scissor_height = height;

	giScissorTest = true;
}

void DisableScissorTest(void)
{
	scissor_x = 0;
	scissor_width = 0;
	scissor_y = 0;
	scissor_height = 0;

	giScissorTest = false;
}

void Draw_Init()
{
	m_bDrawInitialized = true;
	VGUI2_Draw_Init();

	// TODO: implement - Solokiller

	Cvar_RegisterVariable(&gl_ansio);

	if (Host_GetVideoLevel() > 0)
	{
		Cvar_DirectSet(&gl_ansio, "4");
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, gl_ansio.value);
	}
	// TODO: implement - Solokiller
}

void Draw_FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a)
{
	g_engdstAddrs.pfnFillRGBA(&x, &y, &w, &h, &r, &g, &b, &a);

	qglDisable(GL_TEXTURE_2D);
	qglEnable(GL_BLEND);

	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	qglBlendFunc(GL_SRC_ALPHA, GL_LINES);
	qglColor4f(r / 255.0, g / 255.0, b / 255.0, a / 255.0);

	qglBegin(GL_RELATIVE_HORIZONTAL_LINE_TO_NV);
	qglVertex2f(x, y);
	qglVertex2f(w + x, y);
	qglVertex2f(w + x, h + y);
	qglVertex2f(x, h + y);
	qglEnd();

	qglColor3f(1.0, 1.0, 1.0);
	qglEnable(GL_TEXTURE_2D);
	qglDisable(GL_BLEND);
}

int Draw_Character(int x, int y, int num, unsigned int font)
{
	return VGUI2_Draw_Character(x, y, num, font);
}

int Draw_MessageCharacterAdd(int x, int y, int num, int rr, int gg, int bb, unsigned int font)
{
	return VGUI2_Draw_CharacterAdd(x, y, num, rr, gg, bb, font);
}

int Draw_String(int x, int y, char* str)
{
	int width = VGUI2_DrawString(x, y, str, VGUI2_GetConsoleFont());
	Draw_ResetTextColor();

	return width + x;
}

int Draw_StringLen(const char* psz, unsigned int font)
{
	return VGUI2_Draw_StringLen(psz, font);
}

void Draw_SetTextColor(float r, float g, float b)
{
	g_engdstAddrs.pfnDrawSetTextColor(&r, &g, &b);

	VGUI2_Draw_SetTextColor(
		static_cast<int>(r * 255.0),
		static_cast<int>(g * 255.0),
		static_cast<int>(b * 255.0));
}

void Draw_GetDefaultColor()
{
	NOT_IMPLEMENTED;
}

void Draw_ResetTextColor()
{
	Draw_GetDefaultColor();
}

void Draw_FillRGBABlend(int x, int y, int w, int h, int r, int g, int b, int a)
{
	g_engdstAddrs.pfnFillRGBA(&x, &y, &w, &h, &r, &g, &b, &a);

	qglDisable(GL_TEXTURE_2D);
	qglEnable(GL_BLEND);

	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	qglColor4f(r / 255.0, g / 255.0, b / 255.0, a / 255.0);

	qglBegin(GL_RELATIVE_HORIZONTAL_LINE_TO_NV);
	qglVertex2f(x, y);
	qglVertex2f(w + x, y);
	qglVertex2f(w + x, h + y);
	qglVertex2f(x, h + y);
	qglEnd();

	qglColor3f(1.0, 1.0, 1.0);

	qglEnable(GL_TEXTURE_2D);
	qglDisable(GL_BLEND);
}

GLuint GL_GenTexture()
{
	GLuint tex;
	qglGenTextures(1, &tex);
	return tex;
}

GLenum oldtarget = TEXTURE0_SGIS;

void GL_UnloadTexture(char* identifier)
{
	NOT_IMPLEMENTED;
}

void GL_SelectTexture(GLenum target)
{
	if (!gl_mtexable)
		return;

	qglSelectTextureSGIS(target);

	if (target == oldtarget)
		return;

	cnttextures[oldtarget - TEXTURE0_SGIS] = currenttexture;
	currenttexture = cnttextures[target - TEXTURE0_SGIS];
	oldtarget = target;
}

void GL_PaletteSelect(int paletteIndex)
{
	if (g_currentpalette != paletteIndex)
	{
		if (qglColorTableEXT)
		{
			g_currentpalette = paletteIndex;
			qglColorTableEXT(GL_SHARED_TEXTURE_PALETTE_EXT, GL_RGB, GL_DYNAMIC_STORAGE_BIT, GL_RGB, GL_UNSIGNED_BYTE, gGLPalette[paletteIndex].colors);
		}
	}
}

void GL_Bind(int texnum)
{
	NOT_IMPLEMENTED;
}

bool ValidateWRect(const wrect_t* prc)
{
	if (!prc)
		return false;

	if ((prc->left >= prc->right) || (prc->top >= prc->bottom))
		return false;

	return true;
}

bool IntersectWRect(const wrect_t* prc1, const wrect_t* prc2, wrect_t* prc)
{
	wrect_t rc;

	if (!prc)
		prc = &rc;

	prc->left = max(prc1->left, prc2->left);
	prc->right = min(prc1->right, prc2->right);

	if (prc->left < prc->right)
	{
		prc->top = max(prc1->top, prc2->top);
		prc->bottom = min(prc1->bottom, prc2->bottom);

		if (prc->top < prc->bottom)
			return true;
	}

	return false;
}

void AdjustSubRect(mspriteframe_t* pFrame, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom, int* pw, int* ph, const wrect_t* prcSubRect)
{
	wrect_t wrect;

	if (!ValidateWRect(prcSubRect))
		return;

	wrect.left = 0;
	wrect.top = 0;
	wrect.right = *pw;
	wrect.bottom = *ph;

	if (!IntersectWRect(prcSubRect, &wrect, &wrect))
		return;

	*pw = wrect.right - wrect.left;
	*ph = wrect.bottom - wrect.top;

#ifdef REGS_FIXES
	*pfLeft = (wrect.left) * 1.0 / pFrame->width;
	*pfRight = (wrect.right) * 1.0 / pFrame->width;

	*pfTop = (wrect.top) * 1.0 / pFrame->height;
	*pfBottom = (wrect.bottom) * 1.0 / pFrame->height;
#else
	*pfLeft = (wrect.left + 0.5) * 1.0 / pFrame->width;
	*pfRight = (wrect.right - 0.5) * 1.0 / pFrame->width;

	*pfTop = (wrect.top + 0.5) * 1.0 / pFrame->height;
	*pfBottom = (wrect.bottom - 0.5) * 1.0 / pFrame->height;
#endif
}

void Draw_Frame(mspriteframe_t* pFrame, int ix, int iy, const wrect_t* prcSubRect)
{
	NOT_IMPLEMENTED;
}

void Draw_SpriteFrame(mspriteframe_t* pFrame, unsigned short* pPalette, int x, int y, const wrect_t* prcSubRect)
{
	Draw_Frame(pFrame, x, y, prcSubRect);
}

void Draw_SpriteFrameHoles(mspriteframe_t* pFrame, unsigned short* pPalette, int x, int y, const wrect_t* prcSubRect)
{
	qglEnable(GL_ALPHA_TEST);

	/* - TODO: implement gl_spriteblend - ScriptedSnark
	if (gl_spriteblend.value != 0.0)
	{
	qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	qglEnable(GL_BLEND);
	}
	*/

	Draw_Frame(pFrame, x, y, prcSubRect);
	qglDisable(GL_ALPHA_TEST);
	qglDisable(GL_BLEND);
}

void Draw_SpriteFrameAdditive(mspriteframe_t* pFrame, unsigned short* pPalette, int x, int y, const wrect_t* prcSubRect)
{
	qglEnable(GL_BLEND);
	qglBlendFunc(GL_ONE, GL_ONE);
	Draw_Frame(pFrame, x, y, prcSubRect);
	qglDisable(GL_BLEND);
}

void Draw_SpriteFrameGeneric(mspriteframe_t* pFrame, unsigned short* pPalette, int x, int y, const wrect_t* prcSubRect, int src, int dest, int width, int height)
{
	int w, h;

	w = pFrame->width;
	pFrame->width = width;
	h = pFrame->height;
	pFrame->height = height;

	qglEnable(GL_BLEND);
	qglBlendFunc(src, dest);
	Draw_Frame(pFrame, x, y, prcSubRect);
	qglDisable(GL_BLEND);

	pFrame->width = w;
	pFrame->height = h;
}

void Draw_Pic(int x, int y, qpic_t* pic)
{
	NOT_IMPLEMENTED;
}

void Draw_BeginDisc()
{
	NOT_IMPLEMENTED;
}
