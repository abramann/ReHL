#include "precompiled.h"
#include "qgl.h"
#include "text_draw.h"
#include "gl_vidnt.h"
#include "gl_draw.h"
#include "gl_main.h"
#include "mem.h"
#include "quakedef.h"

#define MAX_TEXELS 524288

typedef struct quake_mode_s
{
	char *name;
	int minimize;
	int maximize;
} quake_mode_t;

quake_mode_t modes[6] =
{
	{"GL_NEAREST", GL_NEAREST, GL_NEAREST},
	{"GL_LINEAR",GL_LINEAR , GL_LINEAR },
	{"GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST } ,

	{"GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST ,GL_LINEAR },

	{"GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR , GL_NEAREST },
	{"GL_LINEAR_MIPMAP_LINEAR",GL_LINEAR_MIPMAP_LINEAR , GL_LINEAR }
};

unsigned int scaled_25071[524288] = { 0 }; //
int g_currentpalette;
int giTotalTextures = 0;

int gl_filter_max = GL_LINEAR;
int gl_filter_min = GL_LINEAR_MIPMAP_LINEAR;
static int texels = 0;

cvar_t gl_ansio = { "gl_ansio", "16" };
cvar_t gl_round_down = { "gl_round_down", "3" , FCVAR_ARCHIVE };
cvar_t gl_picmip = { "gl_picmip", "0", FCVAR_ARCHIVE };
cvar_t gl_palette_tex = { "gl_palette_tex", "1" };
cvar_t gl_texturemode = { "gl_texturemode", "GL_LINEAR_MIPMAP_LINEAR", FCVAR_ARCHIVE };
cvar_t gl_max_size = { "gl_max_size", "256", FCVAR_ARCHIVE };
cvar_t gl_spriteblend = { "gl_spriteblend", "1", FCVAR_ARCHIVE };
cvar_t gl_dither = { "gl_dither", "1", FCVAR_ARCHIVE };
cvarhook_t gl_texturemode_hook = { gl_texturemode_hook_callback };

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

void GL_Texels_f();

qpic_t * LoadTransBMP(char *pszName);

qpic_t * LoadTransPic(char *pszName, qpic_t *ppic);

static gltexture_t gltextures[MAX_GLTEXTURES];

void BoxFilter3x3(unsigned char *out, unsigned char *in, int w, int h, int x, int y);

void ComputeScaledSize(int *wscale, int *hscale, int width, int height);

void GL_ResampleAlphaTexture(unsigned char *in, int inwidth, int inheight, unsigned char *out, int outwidth, int outheight);
void GL_ResampleTexture(unsigned int *in, int inwidth, int inheight, unsigned int *out, int outwidth, int outheight);
short GL_PaletteAdd(unsigned char *pPal, qboolean isSky);
void ApplyScale(unsigned char* data, int width, int height, unsigned char* scaledData, int scaled_width, int scaled_height);

unsigned int trans_25102[307202] = { 0 };

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

void gl_texturemode_hook_callback(cvar_t * cvar)
{
	int indx = 0;

	if (!cvar)
		return;

	const char* s2 = cvar->string;
	for (int i = 0; i < 6; i++)
	{
		if (Q_strcasecmp(modes[i].name, s2))
		{
			if (i == 5)
			{
				Con_Printf("bad filter name\n");
				return;
			}

			indx = i + 1;
		}
		else
		{
			break;
		}
	}

	gl_filter_min = modes[indx].minimize;
	gl_filter_max = modes[indx].maximize;;

	for (int i = 0; i < numgltextures; i++)
	{
		gltexture_t* glt = &gltextures[i];

		GL_Bind(glt->texnum);

		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter_min);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter_max);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, gl_ansio.value);
	}
}

void Draw_Init()
{
	m_bDrawInitialized = true;
	VGUI2_Draw_Init();

	Cmd_AddCommand("gl_texels", GL_Texels_f);
	Cvar_RegisterVariable(&gl_max_size);
	Cvar_RegisterVariable(&gl_round_down);
	Cvar_RegisterVariable(&gl_picmip);
	Cvar_RegisterVariable(&gl_palette_tex);
	Cvar_RegisterVariable(&gl_texturemode);
	Cvar_RegisterVariable(&gl_ansio);
	Cvar_HookVariable(gl_texturemode.name, &gl_texturemode_hook);
	if (Host_GetVideoLevel() > 0)
	{
		Cvar_DirectSet(&gl_ansio, "4");
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, gl_ansio.value);
	}

	for (int i = 0; i < 350; i++)
	{
		gGLPalette[i].tag = -1;
		gGLPalette[i].referenceCount = 0;
	}

	static cachewad_t custom_wad = { 0 };

	menu_wad = (cachewad_t *)Mem_ZeroMalloc(sizeof(cachewad_t));

	Draw_CacheWadInit("cached.wad", 16, menu_wad);

	menu_wad->tempWad = 1;

	Draw_CacheWadHandler(&custom_wad, Draw_MiptexTexture, 32);

	if (qglColorTableEXT && gl_palette_tex.value != 0.0)
	{
		qglEnable(0x81FBu);
	}

	Q_memset(decal_names, 0, 0x2000);

	for (int i = 0; i < ARRAYSIZE(texgammatable); i++)
	{
		texgammatable[i] = i;
	}

	LoadTransBMP("lambda");
	Draw_ResetTextColor();
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

void Draw_TileClear(int x, int y, int w, int h)
{
	Draw_FillRGBA(x, y, w, h, 0, 0, 0, 255);
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
	int ir;
	int ig;
	int ib;

	if (scanf(con_color.string, "%i %i %i", &ir, &ig, &ib) == 3)
		VGUI2_Draw_SetTextColor(ir, ig, ib);
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

short GL_PaletteAdd(unsigned char *pPal, qboolean isSky)
{
	CHECK_REQUIRED;

	if (!qglColorTableEXT)
	{
		return -1;
	}

	unsigned char gammaPal[768];
	for (int i = 0; i < 768; ++i)
		gammaPal[i] = texgammatable[pPal[i]];

	// Calculate a unique identifier based on palette data
	int hash = 0;
	for (int i = 0; i < 768 - 1; i += 2) {
		hash ^= (gammaPal[i] ^ gammaPal[i + 1]) ^ (gammaPal[i] ^ gammaPal[i + 2]);
	}
	for (int i = 767; i >= 0; --i) {
		hash ^= (gammaPal[i] ^ hash);
	}
	hash = abs(hash);

	// Find existing palette with matching identifier
	int paletteIndex = isSky ? 0x158 : 0; // Start search based on sky/non-sky flag
	while (paletteIndex < (isSky ? 350 : 344)) {
		if (gGLPalette[paletteIndex].tag == hash) {
			// Found existing palette, check for identical content
			bool identical = true;
			for (int i = 0; i < 768; ++i) {
				if (gammaPal[i] != gGLPalette[paletteIndex].colors[i]) {
					identical = false;
					break;
				}
			}
			if (identical) {
				// Increase reference count for existing palette
				gGLPalette[paletteIndex].referenceCount++;
				return paletteIndex;
			}
		}
		paletteIndex++;
	}

	// No matching palette found, create a new one
	int newIndex = paletteIndex;
	Q_memcpy(gGLPalette[newIndex].colors, gammaPal, 768);
	gGLPalette[newIndex].referenceCount = 1;
	gGLPalette[newIndex].tag = hash;
	return newIndex;
}

void GL_Bind(int texnum)
{
	if (currenttexture == texnum)
		return;

	int i = (texnum >> 16) - 1;

	currenttexture = texnum;
	qglBindTexture(GL_TEXTURE_2D, texnum);

	if (i >= 0)
		GL_PaletteSelect(i);
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

void GL_Texels_f()
{
	Con_Printf("Current uploaded texels: %i\n", texels);
}

int GL_LoadTexture(char * identifier, GL_TEXTURETYPE textureType, int width, int height, unsigned char * data, int mipmap, int iType, unsigned char * pPal)
{
	return GL_LoadTexture2(identifier, textureType, width, height, data, (qboolean)mipmap, iType, pPal, gl_filter_max);
}

int GL_LoadTexture2(char * identifier, GL_TEXTURETYPE textureType, int width, int height, unsigned char * data, qboolean mipmap, int iType, unsigned char * pPal, int filter)
{
	gltexture_t* slot = nullptr;

	if (identifier[0] == '\0')
	{
		Con_DPrintf("NULL Texture\n");
	}
	else
	{
		// Check if texture already loaded
		for (int i = 0; i < numgltextures; i++)
		{
			gltexture_t * loaded = &gltextures[i];
			slot = &gltextures[i];

			if (Q_strcasecmp(identifier, loaded->identifier) == 0 && width == loaded->width && height == loaded->height)
			{
				if (loaded->servercount > 0)
				{
					loaded->servercount = gHostSpawnCount;
				}

				if (loaded->paletteIndex < 0)
				{
					return loaded->texnum;
				}
				else
				{
					return loaded->texnum | ((loaded->paletteIndex + 1) << 16);
				}
			}
		}
	}

	if (!slot)
	{
		bool reached_max = numgltextures + 1 <= MAX_GLTEXTURES;
		slot = &gltextures[numgltextures++];
		if (!reached_max)
			Sys_Error("Texture Overflow: MAX_GLTEXTURES");
	}
	if (!slot->texnum)
	{
		slot->texnum = GL_GenTexture();
	}

	Q_strncpy(slot->identifier, identifier, 63);
	slot->identifier[63] = '\0';
	slot->width = width;
	slot->height = height;
	slot->mipmap = mipmap;
	if (textureType == GLT_WORLD)
	{
		slot->servercount = gHostSpawnCount;
	}
	else
	{
		slot->servercount = 0;
	}

	slot->paletteIndex = -1;

	GL_Bind(slot->texnum);

	currenttexture = -1;

	int scaled_width;
	int scaled_height;

	if (g_bSupportsNPOTTextures)
	{
		scaled_width = width;
		scaled_height = height;
	}
	else
	{
		ComputeScaledSize(&scaled_width, &scaled_height, width, height);
	}

	unsigned char* textureData = data;

	unsigned char scaledData[16384] = { 0 };

	if (scaled_width != width && scaled_height != height &&
		scaled_width <= 128 && scaled_height < 128 && !mipmap)
	{
		if (fs_perf_warnings.value != 0.0)
		{
			Con_DPrintf("fs_perf_warnings: resampling non-power-of-2 texture %s (%dx%d)", identifier, width, height);
		}

		ApplyScale(data, width, height, scaledData, scaled_width, scaled_height);
		textureData = scaledData;
	}

	short paletteIndex = 0;
	if (!qglColorTableEXT
		|| gl_palette_tex.value == 0.0
		|| iType
		|| (paletteIndex = GL_PaletteAdd(pPal, 0), slot->paletteIndex = paletteIndex, paletteIndex < 0))
	{
		if (g_modfuncs.m_pfnTextureLoad)
			g_modfuncs.m_pfnTextureLoad(identifier, width, height, (char*)data);
		if (textureType == GLT_SPRITE && iType == GLT_WORLD)
			GL_Upload32((unsigned int*)data, width, height, mipmap, 4, filter);
		else
			GL_Upload16(data, width, height, mipmap, iType, pPal, filter);
	}
	else
	{
		qglTexImage2D(GL_TEXTURE_2D, 0, GL_COLOR_INDEX8_EXT, scaled_width, scaled_height, 0, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, textureData);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		qglTexParameterf(GL_TEXTURE_2D, 34046, gl_ansio.value);
	}

	if (slot->paletteIndex < 0)
		return slot->texnum;
	else
		return (slot->texnum | ((paletteIndex + 1) << 16));
}

qpic_t * LoadTransBMP(char *pszName)
{
	return LoadTransPic(pszName, (qpic_t*)W_GetLumpName(0, pszName));
}

qpic_t * LoadTransPic(char *pszName, qpic_t *ppic)
{
	if (!ppic)
	{
		return nullptr;
	}

	int width = ppic->width;
	int height = ppic->height;

	qpic_t* pNewPic = (qpic_t *)Mem_Malloc(sizeof(qpic_t)); // Why allocating 32?

	pNewPic->width = width;
	pNewPic->height = height;

	if (*pszName != '\0')
	{
		for (int i = 0; i < numgltextures; i++)
		{
			gltexture_t* pTex = &gltextures[i];
			if (!strcmp(pszName, pTex->identifier))
			{
				if (ppic->width == pTex->width && ppic->height == pTex->height)
					return ppic;

				pszName[3]++;
				if (!*pszName)
				{
					break;
				}
				else
				{
					i = -1;
				}
			}
		}
	}

	numgltextures++;
	gltexture_t * glt = &gltextures[numgltextures];
	Q_strcpy(glt->identifier, pszName);

	int texnum;
	qglGenTextures(1, (GLuint*)&texnum);

	glt->texnum = texnum;
	glt->servercount = gHostSpawnCount;
	glt->width = width;
	glt->height = height;

	GL_Bind(texnum);

	int size = width * height;

	// RGBA format image
	byte* pImageData = (byte*)Mem_Malloc(size * 4);

	byte* pOriginalData = &ppic->data[size + 2];
	for (int i = 0; i < size; i++)
	{
		pOriginalData[i] = texgammatable[pOriginalData[i]];
		pImageData[i] = (pOriginalData[3 * i] & 0xFFFFFF) | 0xFF000000; // Set alpha to 255 for non-transparent pixels
	}

	GL_Upload32((unsigned int*)pImageData, width, height, false, (int)GLT_DECAL, gl_filter_max);

	CHECK_REQUIRED;

	// Probably wrong implement
	*(DWORD*)pNewPic->data = gltextures[numgltextures].texnum;
	Mem_Free(pImageData);
	return pNewPic;
}

void GL_Upload32(unsigned int *data, int width, int height, qboolean mipmap, int iType, int filter)
{
	int w;
	int h;
	int bpp;
	VideoMode_GetCurrentVideoMode(&w, &h, &bpp);

	static int giTotalTexBytes = 0;

	int size = width * height;

	if (iType != GLT_HUDSPRITE)
		giTotalTexBytes += 2 * size;
	else
		giTotalTexBytes += size;

	giTotalTextures++;

	int scaled_width;
	int scaled_height;

	float blend = gl_spriteblend.value;
	if (blend == 0.0 || iType != GLT_STUDIO && iType != GLT_DECAL && iType != GLT_WORLD || size <= 0)
	{
	}
	else
	{
		int size = width * height;
		for (int i = 0; i < size; i++)
		{
			if (data[i] == 0)
			{
				i++;
				BoxFilter3x3((uchar*)&data[i], (uchar*)data, width, height, i % width, i / width);
			}
			else
			{
				data[i]++;
			}
		}
	}

	if (g_bSupportsNPOTTextures)
	{
		scaled_width = width;
		scaled_height = height;
	}
	else
	{
		ComputeScaledSize(&scaled_width, &scaled_height, width, height);
	}
	if (scaled_width * scaled_height > MAX_TEXELS)
	{
		Sys_Error("GL_LoadTexture: too big");
	}

	int iFormat;
	int iComponent;
	switch (iType)
	{
	case GLT_SYSTEM:
		iFormat = GL_RGBA;
		iComponent = (bpp == 16) ? GL_RGB8 : GL_RGBA8;
		break;

	case GLT_DECAL:
	case GLT_STUDIO:
		iFormat = GL_RGBA;
		iComponent = (bpp == 16) ? GL_RGBA4 : GL_RGBA8;
		break;

	case GLT_HUDSPRITE:
		iFormat = GL_LUMINANCE;
		iComponent = GL_LUMINANCE8;
		break;

	case GLT_WORLD:
		iFormat = GL_RGBA;
		iComponent = (bpp == 16) ? GL_RGB5_A1 : GL_RGBA8;
		break;

	default:
		break;
	}

	if (scaled_width == width && scaled_height == height)
	{
		if (!mipmap)
			qglTexImage2D(GL_TEXTURE_2D, 0, iComponent, width, height, 0, iFormat, GL_UNSIGNED_BYTE, data);
		else
		{
			if (iType != GLT_HUDSPRITE)
				size *= 4;

			Q_memcpy(scaled_25071, data, size);
			::texels += scaled_height * scaled_width;
			DebugBreak();
			// Check here
			qglTexImage2D(GL_TEXTURE_2D, 0, iComponent, scaled_width, scaled_height, 0, iFormat, GL_UNSIGNED_BYTE, scaled_25071);
		}
	}
	else
	{
		if (iType == GLT_HUDSPRITE)
			GL_ResampleAlphaTexture((uchar*)data, width, height, (uchar*)scaled_25071, scaled_width, scaled_height);
		else
			GL_ResampleTexture(data, width, height, scaled_25071, scaled_width, scaled_height);

		::texels += scaled_height * scaled_width;
		qglTexImage2D(GL_TEXTURE_2D, 0, iComponent, scaled_width, scaled_height, 0, iFormat, GL_UNSIGNED_BYTE, scaled_25071);
	}
	if (mipmap)
	{
		for (int i = 1; scaled_width > 1 || scaled_height > 1; i++)
		{
			Call_Function<void, unsigned int*, int, int>(0x3E0F0, scaled_25071, scaled_width, scaled_height);

			scaled_width /= 2;
			scaled_height /= 2;
			if (scaled_width < 1)
				scaled_width = 1;
			if (scaled_height < 1)
				scaled_height = 1;

			texels += scaled_height * scaled_width;
			qglTexImage2D(GL_TEXTURE_2D, i, iComponent, scaled_width, scaled_height, 0, iFormat, GL_UNSIGNED_BYTE, scaled_25071);
		}
	}

	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter_min);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter_max);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, gl_ansio.value);
}

void GL_Upload16(unsigned char *data, int width, int height, qboolean mipmap, int iType, unsigned char *pPal, int filter)
{
	int count = height * width;
	if (count > 1228800)
	{
		Con_Printf("Can't upload (%ix%i) texture, it's > 640*480 bytes\n", width, height);
		return;
	}

	if (iType == GLT_HUDSPRITE)
	{
		Q_memcpy(trans_25102, data, count);
		GL_Upload32(trans_25102, width, height, mipmap, iType, filter);
		return;
	}

	if (!pPal)
		return;

	for (int i = 1; i < 768; i++)
		pPal[i] = texgammatable[pPal[i]];

	if (iType != GLT_STUDIO && iType != GLT_DECAL && iType != GLT_WORLD)
	{
		if (iType)
		{
			Con_Printf("Upload16:Bogus texture type!/n");
			GL_Upload32(trans_25102, width, height, mipmap, iType, filter);
			return;
		}

		if (count & 3 != 0)
			Sys_Error("GL_Upload16: s&3");

		if (gl_dither.value == 0.0)
		{
			for (int i = 0; i < count + 3; i += 4)
			{
				trans_25102[i] = pPal[3 * data[i]] | 0xFF000000;
				trans_25102[i + 1] = pPal[3 * data[i + 1]] | 0xFF000000;
				trans_25102[i + 2] = pPal[3 * data[i + 2]] | 0xFF000000;
				trans_25102[i + 3] = pPal[3 * data[i + 3]] | 0xFF000000;
			}
		}
		else if (count)
		{
			memset(trans_25102, 0, sizeof(int) * 307202);
			for (int i = 0; i < count; i++)
			{
				unsigned char* p = &pPal[3 * data[i]];
				unsigned char r = p[0] | p[0] >> 6;
				unsigned char g = p[1] | p[1] >> 6;
				unsigned char b = p[2] | p[2] >> 6;
				trans_25102[i] = b << 16 | (g << 8) | r | 0xFF000000;
			}
		}
		GL_Upload32(trans_25102, width, height, mipmap, iType, filter);
	}
	else if (count)
	{
		switch (iType)
		{
		case GLT_STUDIO:
			for (int j = 1; j < count; j++)
				trans_25102[j] = pPal[765] & 0xFFFFFF | (data[j] << 24);

			GL_Upload32(trans_25102, width, height, mipmap, iType, filter);
			break;

		case GLT_WORLD:
			for (int k = 0; k != count; ++k)
				trans_25102[k] = (data[k] << 24) | pPal[3 * data[k]] & 0xFFFFFF;
			GL_Upload32(trans_25102, width, height, mipmap, iType, filter);
			break;

		default:
			bool systemType = true;

			for (int l = 1; l < count; l++)
			{
				while (1)
				{
					if (data[l] == 255)
						break;

					trans_25102[l++] = pPal[3 * data[l]] | 0xFF000000;
					if (l == count)
						break;
				}

				if (l == 255)
					break;

				systemType = false;
				trans_25102[l] = 0;
			}

			if (systemType)
				iType = GLT_SYSTEM;

			GL_Upload32(trans_25102, width, height, mipmap, iType, filter);
		}
	}
}

void BoxFilter3x3(unsigned char * out, unsigned char * in, int w, int h, int x, int y)
{
	int r = 0;
	int g = 0;
	int b = 0;

	int validNeighbors = 0;

	for (int i = -1; i < 2; i++)
	{
		int neighborX = x + i;
		int neighborY = y - 1;
		int base = w * neighborY;

		for (int j = 3; j; j--)
		{
			if (neighborX >= 0 && neighborX < w && neighborY >= 0 && neighborY < h)
			{
				int* neighborOffset = (int*)&in[4 * (base + neighborX)];
				if (neighborOffset[3]) // check alpha
				{
					b += neighborOffset[0];
					g += neighborOffset[1];
					r += neighborOffset[2];
					validNeighbors++;
				}
			}
			base += w;
			neighborY++;
		}
	}

	if (!validNeighbors)
	{
		validNeighbors = 1;
	}

	out[0] = b / validNeighbors;
	out[1] = g / validNeighbors;
	out[2] = r / validNeighbors;
}

void ComputeScaledSize(int *wscale, int *hscale, int width, int height)
{
	if (g_bSupportsNPOTTextures)
	{
		if (wscale)
			*wscale = width;
		if (hscale)
			*hscale = height;
	}
	else
	{
		int maxSize = (gl_max_size.value > 128) ? gl_max_size.value : 128;

		int i = 1;
		for (; i < width; i *= 2);

		int roundDown = gl_round_down.value;

		if (roundDown - 1 <= 30 && width < i && (roundDown == 1 || i - width > i >> roundDown))
		{
			i >>= 1;
		}

		int j = 1;
		for (; j < height; j *= 2);

		if (roundDown - 1 <= 30 && height < j && (roundDown == 1 || j - height > j >> roundDown))
		{
			j >>= 1;
		}

		int mip = gl_picmip.value;
		if (mip - 1 <= 30)
		{
			i >>= mip;
			j >>= mip;
		}

		if (wscale)
		{
			if (i > maxSize)
			{
				i = maxSize;
			}

			*wscale = i;
		}

		if (hscale)
		{
			if (j > maxSize)
			{
				j = maxSize;
			}

			*hscale = j;
		}
	}
}

void GL_ResampleAlphaTexture(unsigned char *in, int inwidth, int inheight, unsigned char *out, int outwidth, int outheight)
{
	Call_Function<void, unsigned char *, int, int, unsigned char *, int, int>(0x3DE90, in, inwidth, inheight, out, outwidth, outheight);

	/*
	unsigned char p1[1024];
	unsigned char p2[1024];
	if (outwidth > 0)
	{
		v6 = (inwidth << 16) / outwidth;
		v7 = 0;
		v8 = v6 >> 2;
		do
		{
			v9 = BYTE2(v8);
			v8 += v6;
			p1[v7++] = v9;
		} while (v7 != outwidth);
		for (int i = 0; i < outwidth; i++)
		{
			p1[i] =
		}
		v10 = 3 * ((unsigned int)((inwidth << 16) / outwidth) >> 2);
		for (j = 0; j != outwidth; ++j)
		{
			v12 = BYTE2(v10);
			v10 += v6;
			p2[j] = v12;
		}
	}
	*/
}

void GL_ResampleTexture(unsigned int *in, int inwidth, int inheight, unsigned int *out, int outwidth, int outheight)
{
	Call_Function<void, unsigned int *, int, int, unsigned int *, int, int>(0x3DCD0, in, inwidth, inheight, out, outwidth, outheight);

	/*
	void GL_ResampleTexture(unsigned int* in, int inWidth, int inHeight, unsigned int* out, int outWidth, int outHeight) {
  // Pre-calculate scaling factors (assuming fixed-point representation)
  unsigned int xScale = 0;
  unsigned int yScale = 0;
  if (outWidth > 0) {
	xScale = (inWidth << 16) / outWidth; // 16.16 fixed-point format
	for (int i = 0; i < outWidth; ++i) {
	  p1[i] = 4 * ((xScale * (i + 1)) >> 16); // Calculate weight for each output pixel (horizontal)
	}
  }

  if (outHeight > 0) {
	yScale = (inHeight << 16) / outHeight; // 16.16 fixed-point format
	for (int i = 0; i < outHeight; ++i) {
	  p2[i] = 4 * ((yScale * (i + 1)) >> 16); // Calculate weight for each output pixel (vertical)
	}
  }

  // Resample each output pixel
  for (int y = 0; y < outHeight; ++y) {
	unsigned int* inRow1;  // Pointer to source row with 0.25 weight
	unsigned int* inRow2;  // Pointer to source row with 0.75 weight

	// Calculate source row indices based on scaling and rounding
	double yFactor = (y + 0.25) * ((double)inHeight / outHeight);
	inRow1 = in + inWidth * (int)yFactor;
	yFactor = (y + 0.75) * ((double)inHeight / outHeight);
	inRow2 = in + inWidth * (int)yFactor;

	if (outWidth > 0) {
	  unsigned int* outputPixel = out;
	  for (int x = 0; x < outWidth; ++x) {
		// Bilinear interpolation using pre-calculated weights
		unsigned int sumR = 0, sumG = 0, sumB = 0, sumA = 0;
		sumR += *(inRow1 + p1[x]) & 0xFF;
		sumG += *(inRow1 + p1[x] + 1) & 0xFF;
		sumB += *(inRow1 + p1[x] + 2) & 0xFF;
		sumA += *(inRow1 + p1[x] + 3) & 0xFF;

		sumR += *(inRow2 + p1[x]) & 0xFF;
		sumG += *(inRow2 + p1[x] + 1) & 0xFF;
		sumB += *(inRow2 + p1[x] + 2) & 0xFF;
		sumA += *(inRow2 + p1[x] + 3) & 0xFF;

		sumR += *(inRow1 + p2[x]) & 0xFF;
		sumG += *(inRow1 + p2[x] + 1) & 0xFF;
		sumB += *(inRow1 + p2[x] + 2) & 0xFF;
		sumA += *(inRow1 + p2[x] + 3) & 0xFF;

		sumR += *(inRow2 + p2[x]) & 0xFF;
		sumG += *(inRow2 + p2[x] + 1) & 0xFF;
		sumB += *(inRow2 + p2[x]  + 2) & 0xFF;
		sumA += *(inRow2 + p2[x] + 3) & 0xFF;

		// Average the accumulated values
		*outputPixel = (sumR >> 2) | ((sumG >> 2) << 8) | ((sumB >> 2) << 16) | ((sumA >> 2) << 24);
		outputPixel++;
	  }
	}
	out += outWidth; // Move to the next output row
  }
}
*/
}

void ApplyScale(unsigned char* data, int width, int height, unsigned char* scaledData, int scaled_width, int scaled_height)
{
	NOT_IMPLEMENTED;
	/*
	 // Calculate scaling factors (fixed point 16.16)
  unsigned int widthScale = (width << 16) / scaledWidth;
  unsigned int heightScale = (height << 16) / scaledHeight;

  // Check for invalid scaled height
  if (scaledHeight <= 0) {
	return 0; // Handle error or return something meaningful
  }

  // Loop through scaled height
  for (int y = 0; y < scaledHeight; y += (heightScale >> 2)) {
	// Check for invalid scaled width
	if (scaledWidth <= 0) {
	  break; // Handle error or skip processing
	}

	// Loop through scaled width
	for (int x = 0; x < scaledWidth; x += (widthScale >> 2)) {
	  // Calculate source pixel index (avoid overflow with unsigned int)
	  int sourceIndex = y * width + x;

	  // Copy pixel from source image to output
	  *out++ = *((unsigned char*)imageData + sourceIndex);
	}

	// Move to next row in source image based on scaling factor
	imageData += width * ((heightScale + y) >> 16);
  }
  */
}