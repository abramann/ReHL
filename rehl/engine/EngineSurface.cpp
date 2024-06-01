#include "precompiled.h"
#include "vgui2\VGui.h"


EngineSurface* EngineSurface::s_pEngineSurface = nullptr;

void EngineSurface::createRenderPlat()
{
	// Nothing
}

void EngineSurface::deleteRenderPlat()
{
	// Nothing
}

void EngineSurface::freeEngineSurface()
{
	// TODO: doesn't work because this pointer is never set. - Solokiller
	if (s_pEngineSurface)
	{
		delete s_pEngineSurface;
		s_pEngineSurface = nullptr;
	}
}

EngineSurface _g_EngineSurface_singleton;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(EngineSurface, IEngineSurface, ENGINESURFACE_INTERFACE_VERSION, _g_EngineSurface_singleton);

bool g_bScissor = false;
rect_t g_ScissorRect = {};

struct TCoordRect
{
	float s0;
	float t0;
	float s1;
	float t1;
};

float InterpTCoord(int val, int min, int max, float tMin, float tMax)
{
	return tMin + (tMax - tMin) * (static_cast<float>(val - min) / static_cast<float>(max - min));
}

bool IsRectEmpty(const rect_t* rect)
{
	return rect->left == rect->right && rect->top == rect->bottom;
}

bool IntersectRect(rect_t* dest, const rect_t* src1, const rect_t* src2)
{
	if (!src1 || !dest || !src2)
		return false;

	if (!IsRectEmpty(src1) && !IsRectEmpty(src2) &&
		src2->right > src1->left &&
		src2->left < src1->right &&
		src2->bottom > src1->top &&
		src2->top < src1->bottom)
	{
		dest->left = max(src1->left, src2->left);
		dest->right = min(src1->right, src2->right);

		dest->top = max(src1->top, src2->top);
		dest->bottom = min(src1->bottom, src2->bottom);

		return true;
	}

	dest->left = 0;
	dest->top = 0;
	dest->right = 0;
	dest->bottom = 0;

	return false;
}

bool Scissorrect_tCoords(int x0, int y0, int x1, int y1,
	float s0, float t0, float s1, float t1,
	rect_t* pOut, TCoordRect* pTCoords)
{
	if (g_bScissor)
	{
		if (pOut)
		{
			rect_t in =
			{
				x0, y0, x1, y1 };

			if (IntersectRect(pOut, &g_ScissorRect, &in))
			{
				if (pTCoords)
				{
					pTCoords->s0 = InterpTCoord(pOut->left, x0, x1, s0, s1);
					pTCoords->s1 = InterpTCoord(pOut->right, x0, x1, s0, s1);

					pTCoords->t0 = InterpTCoord(pOut->top, y0, y1, t0, t1);
					pTCoords->t1 = InterpTCoord(pOut->bottom, y0, y1, t0, t1);
				}

				return true;
			}
		}
	}
	else
	{
		pOut->left = x0;
		pOut->right = x1;
		pOut->top = y0;
		pOut->bottom = y1;

		if (pTCoords)
		{
			pTCoords->s0 = s0;
			pTCoords->t0 = t0;
			pTCoords->s1 = s1;
			pTCoords->t1 = t1;
		}

		return true;
	}

	return false;
}

bool ScissorRect(int x0, int y0, int x1, int y1, rect_t* pOut)
{
	return Scissorrect_tCoords(x0, y0, x1, y1, 0, 0, 0, 0, pOut, nullptr);
}

struct Texture
{
	int _id;
	int _wide;
	int _tall;
	float _s0;
	float _t0;
	float _s1;
	float _t1;
};

namespace
{
	
}

struct VertexBuffer_t
{
	float texcoords[2];
	float vertex[2];
};

bool VguiSurfaceTexturesLessFunc(const Texture& lhs, const Texture& rhs)
{
	return lhs._id < rhs._id;
}

static CUtlRBTree<Texture, int> g_VGuiSurfaceTextures(0, 128, VguiSurfaceTexturesLessFunc);

static Texture* staticTextureCurrent = nullptr;

static VertexBuffer_t g_VertexBuffer[256];
static int g_iVertexBufferEntriesUsed = 0;
static float g_vgui_projection_matrix[16];

static Texture* staticGetTextureById(int id)
{
	Texture dummy;
	dummy._id = id;

	auto index = g_VGuiSurfaceTextures.Find(dummy);

	if (index != g_VGuiSurfaceTextures.InvalidIndex())
		return &g_VGuiSurfaceTextures[index];

	return nullptr;
}

EngineSurface::~EngineSurface()
{
	deleteRenderPlat();
	_renderPlat = 0;
	s_pEngineSurface = 0;
}

void EngineSurface::pushMakeCurrent(int* insets, int* absExtents, int* clipRect, bool translateToScreenSpace)
{
	int x = 0, y = 0;

	if (translateToScreenSpace)
	{
		if (VideoMode_IsWindowed())
		{
			SDL_GetWindowPosition(*pmainwindow, &x, &y);
		}
	}

	rect_t rect;

	rect.top = 0;

	if (VideoMode_IsWindowed())
	{
		SDL_GetWindowSize(*pmainwindow, &rect.right, &rect.bottom);
	}
	else
	{
		VideoMode_GetCurrentVideoMode(&rect.right, &rect.bottom, nullptr);
	}

	rect.bottom += rect.top;

	// TODO: operations out of order? - Solokiller
	qglPushMatrix();
	qglMatrixMode(GL_PROJECTION);
	qglLoadIdentity();
	qglOrtho(0, rect.right, rect.bottom, 0, -1, 1);

	qglMatrixMode(GL_MODELVIEW);
	qglPushMatrix();
	qglLoadIdentity();

	int wide = insets[0];
	int tall = insets[1];

	qglTranslatef(wide, tall, 0);

	const int iOffsetX = absExtents[0] - x;
	const int iOffsetY = absExtents[1] - y;
	qglTranslatef(iOffsetX, iOffsetY, 0);

	g_bScissor = true;

	g_ScissorRect.left = clipRect[0] - x - (insets[0] + iOffsetX);
	g_ScissorRect.right = clipRect[2] - x - (insets[0] + iOffsetX);
	g_ScissorRect.top = clipRect[1] - y - (insets[1] + iOffsetY);
	g_ScissorRect.bottom = clipRect[3] - y - (insets[1] + iOffsetY);

	qglEnable(GL_BLEND);
	qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void EngineSurface::popMakeCurrent()
{
	drawFlushText();

	// TODO: operations out of order? - Solokiller
	qglMatrixMode(GL_PROJECTION);
	qglMatrixMode(GL_MODELVIEW);
	qglPopMatrix();
	qglPopMatrix();

	g_bScissor = false;

	qglEnable(GL_TEXTURE_2D);
}

void EngineSurface::drawFilledRect(int x0, int y0, int x1, int y1)
{
	if (_drawColor[3] != 255)
	{
		rect_t rcOut;

		if (ScissorRect(x0, y0, x1, y1, &rcOut))
		{
			qglDisable(GL_TEXTURE_2D);

			qglColor4ub(_drawColor[0], _drawColor[1], _drawColor[2], 255 - _drawColor[3]);

			qglBegin(GL_QUADS);

			qglVertex2f(rcOut.left, rcOut.top);
			qglVertex2f(rcOut.right, rcOut.top);
			qglVertex2f(rcOut.right, rcOut.bottom);
			qglVertex2f(rcOut.left, rcOut.bottom);

			qglEnd();

			qglEnable(GL_TEXTURE_2D);
		}
	}
}

void EngineSurface::drawOutlinedRect(int x0, int y0, int x1, int y1)
{
	if (_drawColor[3] != 255)
	{
		qglDisable(GL_TEXTURE_2D);
		qglColor4ub(_drawColor[0], _drawColor[1], _drawColor[2], 255 - _drawColor[3]);

		drawFilledRect(x0, y0, x1, y0 + 1);
		drawFilledRect(x0, y1 - 1, x1, y1);
		drawFilledRect(x0, y0 + 1, x0 + 1, y1 - 1);
		drawFilledRect(x1 - 1, y0 + 1, x1, y1 - 1);
	}
}

void EngineSurface::drawLine(int x1, int y1, int x2, int y2)
{
	qglDisable(GL_TEXTURE_2D);
	qglEnable(GL_BLEND);
	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	qglBlendFunc(GL_SRC_ALPHA, GL_ONE);

	qglColor4ub(_drawColor[0], _drawColor[1], _drawColor[2], 255 - _drawColor[3]);

	qglBegin(GL_LINES);

	qglVertex2f(x1, y1);
	qglVertex2f(x2, y2);

	qglEnd();

	qglColor3f(1, 1, 1);

	qglEnable(GL_TEXTURE_2D);
	qglDisable(GL_BLEND);
}

void EngineSurface::drawPolyLine(int* px, int* py, int numPoints)
{
	qglDisable(GL_TEXTURE_2D);
	qglEnable(GL_BLEND);
	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	qglBlendFunc(GL_SRC_ALPHA, GL_ONE);

	qglColor4ub(_drawColor[0], _drawColor[1], _drawColor[2], 255 - _drawColor[3]);

	qglBegin(GL_LINES);

	for (int i = 1; i < numPoints; ++i)
	{
		qglVertex2f(px[i - 1], py[i - 1]);
		qglVertex2f(px[i], py[i]);
	}

	qglEnd();

	qglColor3f(1, 1, 1);

	qglEnable(GL_TEXTURE_2D);
	qglDisable(GL_BLEND);
}

void EngineSurface::drawTexturedPolygon(vgui2::VGuiVertex* pVertices, int n)
{
	if (_drawColor[3] == 255)
		return;

	qglColor4ub(
		_drawColor[0],
		_drawColor[1],
		_drawColor[2],
		255 - _drawColor[3]);

	qglEnable(GL_TEXTURE_2D);


	qglBegin(GL_POLYGON);

	for (int i = 0; i < n; i++)
	{
		vgui2::VGuiVertex pNext = pVertices[i];
		qglTexCoord2f(pNext.u, pNext.v);
		qglVertex2f(pNext.x, pNext.y);
	}

	qglEnd();
}

void EngineSurface::drawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall, int hardwareFilter, int hasAlphaChannel)
{
	auto pTexture = staticGetTextureById(id);

	if (!pTexture)
	{
		Texture newTexture;

		memset(&newTexture, 0, sizeof(newTexture));

		newTexture._id = id;

		pTexture = &g_VGuiSurfaceTextures[g_VGuiSurfaceTextures.Insert(newTexture)];
	}

	if (!pTexture)
		return;

	pTexture->_id = id;
	pTexture->_wide = wide;
	pTexture->_tall = tall;

	int pow2Wide;
	int pow2Tall;

	for (int i = 0; i < 32; ++i)
	{
		pow2Wide = 1 << i;

		if (wide <= pow2Wide)
			break;
	}

	for (int i = 0; i < 32; ++i)
	{
		pow2Tall = 1 << i;

		if (tall <= pow2Tall)
			break;
	}

	int* pExpanded = nullptr;

	const void* pData = rgba;

	// Convert to power of 2 texture.
	if (wide != pow2Wide || tall != pow2Tall)
	{
		pExpanded = new int[pow2Wide * pow2Tall];

		pData = pExpanded;

		memset(pExpanded, 0, 4 * pow2Wide * pow2Tall);

		const int* pSrc = reinterpret_cast<const int*>(rgba);
		int* pDest = pExpanded;

		for (int y = 0; y < tall; ++y)
		{
			for (int x = 0; x < wide; ++x)
			{
				pDest[x] = pSrc[x];
			}

			pDest += pow2Wide;
			pSrc += wide;
		}
	}

	pTexture->_s0 = 0;
	pTexture->_t0 = 0;
	pTexture->_s1 = static_cast<double>(wide) / pow2Wide;
	pTexture->_t1 = static_cast<double>(tall) / pow2Tall;

	staticTextureCurrent = pTexture;

	currenttexture = id;

	qglBindTexture(GL_TEXTURE_2D, id);

	if (hardwareFilter)
	{
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	int w, h, bpp;
	VideoMode_GetCurrentVideoMode(&w, &h, &bpp);

	if (hasAlphaChannel || bpp == 32)
	{
		qglTexImage2D(
			GL_TEXTURE_2D,
			0, GL_RGBA8,
			pow2Wide, pow2Tall,
			0,
			GL_RGBA, GL_UNSIGNED_BYTE,
			pData);
	}
	else
	{
		qglTexImage2D(
			GL_TEXTURE_2D,
			0, GL_RGB5_A1,
			pow2Wide, pow2Tall,
			0,
			GL_RGBA, GL_UNSIGNED_BYTE,
			pData);
	}

	if (pExpanded)
		delete[] pExpanded;
}

void EngineSurface::drawSetTexture(int id)
{
	if (id != currenttexture)
	{
		drawFlushText();
		currenttexture = id;
	}

	staticTextureCurrent = staticGetTextureById(id);

	qglBindTexture(GL_TEXTURE_2D, id);
}

void EngineSurface::drawTexturedRect(int x0, int y0, int x1, int y1)
{
	if (!staticTextureCurrent || _drawColor[3] == 255)
		return;

	rect_t rcOut;
	TCoordRect tRect;

	if (!Scissorrect_tCoords(
		x0, y0, x1, y1,
		staticTextureCurrent->_s0,
		staticTextureCurrent->_t0,
		staticTextureCurrent->_s1,
		staticTextureCurrent->_t1,
		&rcOut,
		&tRect))
	{
		return;
	}

	qglEnable(GL_TEXTURE_2D);

	qglBindTexture(GL_TEXTURE_2D, staticTextureCurrent->_id);

	currenttexture = staticTextureCurrent->_id;

	qglColor4ub(
		_drawColor[0],
		_drawColor[1],
		_drawColor[2],
		255 - _drawColor[3]);

	qglBegin(GL_QUADS);

	qglTexCoord2f(tRect.s0, tRect.t0);
	qglVertex2f(rcOut.left, rcOut.top);

	qglTexCoord2f(tRect.s1, tRect.t0);
	qglVertex2f(rcOut.right, rcOut.top);

	qglTexCoord2f(tRect.s1, tRect.t1);
	qglVertex2f(rcOut.right, rcOut.bottom);

	qglTexCoord2f(tRect.s0, tRect.t1);
	qglVertex2f(rcOut.left, rcOut.bottom);

	qglEnd();
}

int EngineSurface::createNewTextureID(void)
{
	return GL_GenTexture();
}

void EngineSurface::drawSetColor(int r, int g, int b, int a)
{
	_drawColor[0] = r;
	_drawColor[1] = g;
	_drawColor[2] = b;
	_drawColor[3] = a;
}

void EngineSurface::drawSetTextColor(int r, int g, int b, int a)
{
	if (_drawTextColor[0] != r
		|| _drawTextColor[1] != g
		|| _drawTextColor[2] != b
		|| _drawTextColor[3] != a)
	{
		drawFlushText();

		_drawTextColor[0] = r;
		_drawTextColor[1] = g;
		_drawTextColor[2] = b;
		_drawTextColor[3] = a;
	}
}

void EngineSurface::drawSetTextPos(int x, int y)
{
	_drawTextPos[0] = x;
	_drawTextPos[1] = y;
}

void EngineSurface::drawGetTextPos(int & x, int & y)
{
	CHECK_REQUIRED;

	x = _drawTextPos[0];
	y = _drawTextPos[1];
}

void EngineSurface::drawPrintChar(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1)
{
	rect_t rcOut;
	TCoordRect tRect;

	if (_drawTextColor[3] == 255 ||
		!Scissorrect_tCoords(
			x, y,
			x + wide, y + tall,
			s0, t0, s1, t1,
			&rcOut, &tRect))
		return;

	if (g_iVertexBufferEntriesUsed >= ARRAYSIZE(g_VertexBuffer))
	{
		drawFlushText();
		g_iVertexBufferEntriesUsed = 0;
	}

	auto& vertex1 = g_VertexBuffer[g_iVertexBufferEntriesUsed++];

	vertex1.texcoords[0] = tRect.s0;
	vertex1.texcoords[1] = tRect.t0;
	vertex1.vertex[0] = rcOut.left;
	vertex1.vertex[1] = rcOut.top;

	auto& vertex2 = g_VertexBuffer[g_iVertexBufferEntriesUsed++];

	vertex2.texcoords[0] = tRect.s1;
	vertex2.texcoords[1] = tRect.t0;
	vertex2.vertex[0] = rcOut.right;
	vertex2.vertex[1] = rcOut.top;

	auto& vertex3 = g_VertexBuffer[g_iVertexBufferEntriesUsed++];

	vertex3.texcoords[0] = tRect.s1;
	vertex3.texcoords[1] = tRect.t1;
	vertex3.vertex[0] = rcOut.right;
	vertex3.vertex[1] = rcOut.bottom;

	auto& vertex4 = g_VertexBuffer[g_iVertexBufferEntriesUsed++];

	vertex4.texcoords[0] = tRect.s0;
	vertex4.texcoords[1] = tRect.t1;
	vertex4.vertex[0] = rcOut.left;
	vertex4.vertex[1] = rcOut.bottom;
}

void EngineSurface::drawPrintCharAdd(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1)
{
	rect_t rcOut;
	TCoordRect tRect;

	if (_drawTextColor[3] == 255 ||
		!Scissorrect_tCoords(
			x, y,
			x + wide, y + tall,
			s0, t0, s1, t1,
			&rcOut, &tRect))
		return;

	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	qglBlendFunc(GL_EQUAL, GL_ONE);
	qglEnable(GL_BLEND);
	qglEnable(GL_ALPHA_TEST);

	qglColor4ub(
		_drawTextColor[0],
		_drawTextColor[1],
		_drawTextColor[2],
		255 - _drawTextColor[3]);

	qglBegin(GL_QUADS);

	qglTexCoord2f(tRect.s0, tRect.t0);
	qglVertex2f(rcOut.left, rcOut.top);

	qglTexCoord2f(tRect.s1, tRect.t0);
	qglVertex2f(rcOut.right, rcOut.top);

	qglTexCoord2f(tRect.s1, tRect.t1);
	qglVertex2f(rcOut.right, rcOut.bottom);

	qglTexCoord2f(tRect.s0, tRect.t1);
	qglVertex2f(rcOut.left, rcOut.bottom);

	qglEnd();

	qglDisable(GL_ALPHA_TEST);
	qglDisable(GL_BLEND);
}

void EngineSurface::drawSetTextureFile(int id, const char* filename, int hardwareFilter, bool forceReload)
{
	auto pTexture = staticGetTextureById(id);

	if (!pTexture || forceReload)
	{
		char name[512];

		byte buf[512 * 512];

		int width, height;

		snprintf(name, ARRAYSIZE(name), "%s.tga", filename);

		bool bLoaded = false;

		if (LoadTGA(name, buf, sizeof(buf), &width, &height))
		{
			bLoaded = true;
		}
		else
		{
			snprintf(name, ARRAYSIZE(name), "%s.bmp", filename);

			FileHandle_t hFile = FS_Open(name, "rb");

			if (hFile != FILESYSTEM_INVALID_HANDLE)
			{
				VGui_LoadBMP(hFile, buf, sizeof(buf), &width, &height);
				// TODO: check result of call - Solokiller
				bLoaded = true;
			}
		}

		if (bLoaded)
		{
			drawSetTextureRGBA(id, buf, width, height, hardwareFilter, false);

			pTexture = staticGetTextureById(id);
		}

		if (fs_perf_warnings.value && (!IsPowerOfTwo(width) || !IsPowerOfTwo(height)))
		{
			Con_DPrintf("fs_perf_warnings: Resampling non-power-of-2 image '%s' (%dx%d)\n", filename, width, height);
		}
	}

	if (pTexture)
		drawSetTexture(id);
}

void EngineSurface::drawGetTextureSize(int id, int& wide, int& tall)
{
	auto pTexture = staticGetTextureById(id);

	if (pTexture)
	{
		wide = pTexture->_wide;
		tall = pTexture->_tall;
	}
	else
	{
		wide = 0;
		tall = 0;
	}
}

int EngineSurface::isTextureIdValid(int id)
{
	return staticGetTextureById(id) != nullptr;
}

void EngineSurface::drawSetSubTextureRGBA(int textureID, int drawX, int drawY, const unsigned char* rgba, int subTextureWide, int subTextureTall)
{
	qglTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		drawX, drawY,
		subTextureWide, subTextureTall,
		GL_RGBA, GL_UNSIGNED_BYTE,
		rgba);
}

void EngineSurface::drawFlushText()
{
	if (g_iVertexBufferEntriesUsed)
	{
		qglEnableClientState(GL_VERTEX_ARRAY);
		qglEnableClientState(GL_TEXTURE_COORD_ARRAY);

		qglEnable(GL_TEXTURE_2D);
		qglEnable(GL_BLEND);

		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		qglColor4ub(
			_drawTextColor[0],
			_drawTextColor[1],
			_drawTextColor[2],
			255 - _drawTextColor[3]);

		qglTexCoordPointer(2, GL_FLOAT, sizeof(float) * 4, g_VertexBuffer[0].texcoords);
		qglVertexPointer(2, GL_FLOAT, sizeof(float) * 4, g_VertexBuffer[0].vertex);

		qglDrawArrays(GL_QUADS, 0, g_iVertexBufferEntriesUsed);

		qglDisable(GL_BLEND);

		g_iVertexBufferEntriesUsed = 0;
	}
}

void EngineSurface::resetViewPort()
{
	// Nothing
}

void EngineSurface::drawSetTextureBGRA(int id, const unsigned char* rgba, int wide, int tall, int hardwareFilter, int hasAlphaChannel)
{
	auto pTexture = staticGetTextureById(id);

	if (!pTexture)
	{
		Texture newTexture;

		memset(&newTexture, 0, sizeof(newTexture));

		newTexture._id = id;

		pTexture = &g_VGuiSurfaceTextures[g_VGuiSurfaceTextures.Insert(newTexture)];
	}

	if (!pTexture)
		return;

	pTexture->_id = id;
	pTexture->_wide = wide;
	pTexture->_tall = tall;

	int pow2Wide;
	int pow2Tall;

	for (int i = 0; i < 32; ++i)
	{
		pow2Wide = 1 << i;

		if (wide <= pow2Wide)
			break;
	}

	for (int i = 0; i < 32; ++i)
	{
		pow2Tall = 1 << i;

		if (tall <= pow2Tall)
			break;
	}

	int* pExpanded = nullptr;

	const void* pData = rgba;

	// Convert to power of 2 texture.
	if (wide != pow2Wide || tall != pow2Tall)
	{
		pExpanded = new int[pow2Wide * pow2Tall];

		pData = pExpanded;

		memset(pExpanded, 0, 4 * pow2Wide * pow2Tall);

		const int* pSrc = reinterpret_cast<const int*>(rgba);
		int* pDest = pExpanded;

		for (int y = 0; y < tall; ++y)
		{
			for (int x = 0; x < wide; ++x)
			{
				pDest[x] = pSrc[x];
			}

			pDest += pow2Wide;
			pSrc += wide;
		}
	}

	pTexture->_s0 = 0;
	pTexture->_t0 = 0;
	pTexture->_s1 = static_cast<double>(wide) / pow2Wide;
	pTexture->_t1 = static_cast<double>(tall) / pow2Tall;

	staticTextureCurrent = pTexture;

	currenttexture = id;

	qglBindTexture(GL_TEXTURE_2D, id);

	if (hardwareFilter)
	{
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	qglTexImage2D(
		GL_TEXTURE_2D,
		0, GL_RGBA8,
		pow2Wide, pow2Tall,
		0,
		GL_BGRA, GL_UNSIGNED_BYTE,
		pData);

	if (pExpanded)
		delete[] pExpanded;
}

void EngineSurface::drawUpdateRegionTextureBGRA(int nTextureID, int x, int y, const uchar* pchData, int wide, int tall)
{
	auto pTexture = staticGetTextureById(nTextureID);

	qglBindTexture(GL_TEXTURE_2D, nTextureID);

	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	qglPixelStorei(GL_UNPACK_ROW_LENGTH, pTexture->_wide);

	qglTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		0, y,
		pTexture->_wide, tall,
		GL_BGRA, GL_UNSIGNED_BYTE,
		&pchData[4 * y * pTexture->_wide]);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
}

IEngineSurface::~IEngineSurface()
{
}