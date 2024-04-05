#include "precompiled.h"
#include "../FakeVGUI/Dar.h"
#include "../FakeVGUI/InputStream.h"
#include "../FakeVGUI/Image.h"
#include <SDL.h>
#include <SDL_syswm.h>


class FontInfoVGUI;

namespace vgui
{
	class Font;
}

static SDL_Cursor* staticDefaultCursor[20];
static SDL_Cursor* staticCurrentCursor;
static vgui::Font* staticFont;
static FontInfoVGUI* staticFontInfoVGUI;
static vgui::Dar<FontInfoVGUI*> staticFontInfoVGUIDar;
static int staticContextCount;

void EngineSurfaceWrap::AppHandler(void * event, void * userData)
{
	NOT_IMPLEMENTED;
}

void EngineSurfaceWrap::lockCursor()
{
	_cursorLocked = true;
}

void EngineSurfaceWrap::unlockCursor()
{
	this->_cursorLocked = false;
	(*(void(__cdecl **)(EngineSurfaceWrap *const, byte*))(this + 120))(this, baseclass_0);
}

void EngineSurfaceWrap::drawLine(int x1, int y1, int x2, int y2)
{
	_engineSurface->drawLine(x1, y1, x2, y2);
}

void EngineSurfaceWrap::drawPolyLine(int * px, int * py, int n)
{
	_engineSurface->drawPolyLine(px, py, n);
}

void EngineSurfaceWrap::drawTexturedPolygon(vgui::VGuiVertex * pVertices, int n)
{
	_engineSurface->drawTexturedPolygon(pVertices, n);
}

void EngineSurfaceWrap::drawSetTextureBGRA(int id, const char * rgba, int wide, int tall, int hardwareFilter, int hasAlphaChannel)
{
	_engineSurface->drawSetTextureRGBA(id, rgba, wide, tall, hardwareFilter, hasAlphaChannel);
}

void EngineSurfaceWrap::drawUpdateRegionTextureBGRA(int nTextureID, int x, int y, const byte * pchData, int wide, int tall)
{
	_engineSurface->drawUpdateRegionTextureBGRA(nTextureID, x, y, pchData, wide, tall);
}

void EngineSurfaceWrap::drawGetTextPos(int & x, int & y)
{
	_engineSurface->drawGetTextPos(x, y);
}

void EngineSurfaceWrap::drawPrintChar(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1)
{
	_engineSurface->drawPrintChar(x, y, wide, tall, s0, t0, s1, t1);
}

void EngineSurfaceWrap::drawPrintCharAdd(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1)
{
	_engineSurface->drawPrintCharAdd(x, y, wide, tall, s0, t0, s1, t1);
}

bool EngineSurfaceWrap::hasFocus()
{
	return true;
}

bool EngineSurfaceWrap::isWithin(int x, int y)
{
	return true;
}

int EngineSurfaceWrap::createNewTextureID()
{
	return _engineSurface->createNewTextureID();
}

void EngineSurfaceWrap::GetMousePos(int & x, int & y)
{
	NOT_IMPLEMENTED;
}

void EngineSurfaceWrap::setCursor(vgui::Cursor * cursor)
{
	NOT_IMPLEMENTED;
}

void EngineSurfaceWrap::swapBuffers()
{
}

void EngineSurfaceWrap::pushMakeCurrent(vgui::Panel * panel, bool useInsets)
{
	NOT_IMPLEMENTED;
}

void EngineSurfaceWrap::popMakeCurrent(vgui::Panel * panel)
{
	_engineSurface->popMakeCurrent();
}

void EngineSurfaceWrap::applyChanges()
{
	NOT_IMPLEMENTED;
}

void EngineSurfaceWrap::drawSetColor(int r, int g, int b, int a)
{
	_engineSurface->drawSetColor(r, g, b, a);
}

void EngineSurfaceWrap::drawFilledRect(int x0, int y0, int x1, int y1)
{
	_engineSurface->drawFilledRect(x0, y0, x1, y1);
}

void EngineSurfaceWrap::drawOutlinedRect(int x0, int y0, int x1, int y1)
{
	_engineSurface->drawOutlinedRect(x0, y0, x1, y1);
}

void EngineSurfaceWrap::drawSetTextFont(vgui::Font * font)
{
	NOT_IMPLEMENTED;
}

void EngineSurfaceWrap::drawSetTextColor(int r, int g, int b, int a)
{
	_engineSurface->drawSetTextColor(r, g, b, a);
}

void EngineSurfaceWrap::drawSetTextPos(int x, int y)
{
	_engineSurface->drawSetTextPos(x, y);
}

void EngineSurfaceWrap::drawPrintText(const char * text, int textLen)
{
	NOT_IMPLEMENTED;
}

void EngineSurfaceWrap::drawSetTextureRGBA(int id, const char * rgba, int wide, int tall)
{
	_engineSurface->drawSetTextureRGBA(id, rgba, wide, tall, false, false);
}

void EngineSurfaceWrap::drawSetTexture(int id)
{
	_engineSurface->drawSetTexture(id);
}

void EngineSurfaceWrap::drawTexturedRect(int x0, int y0, int x1, int y1)
{
	_engineSurface->drawTexturedRect(x0, y0, x1, y1);
}

void EngineSurfaceWrap::invalidate(vgui::Panel * panel)
{
}

void EngineSurfaceWrap::enableMouseCapture(bool state)
{
}
EngineSurfaceWrap::EngineSurfaceWrap(vgui::Panel * embeddedPanel, IEngineSurface * engineSurface) : SurfaceBase::SurfaceBase(embeddedPanel)
{
	vgui::FileInputStream *inputStream;
	_engineSurface = engineSurface;
	staticFont = 0;
	staticFontInfoVGUI = 0;
	_cursorLocked = false;
	++staticContextCount;
	inputStream = new vgui::FileInputStream("valve/gfx/vgui/mouse.tga", false);
	_emulatedMouseImage = new vgui::BitmapTGA((vgui::InputStream*)inputStream, true);
	inputStream->close();
	delete inputStream;

	if (!SDL_WasInit(32))
		{
			SDL_SetHint("SDL_VIDEO_X11_XRANDR", "1");
			SDL_SetHint("SDL_VIDEO_X11_XVIDMODE", "1");
			SDL_InitSubSystem(32);
		}

	staticDefaultCursor[1] = 0;
	for (int i = 2; i < 14; i++)
	{
		staticDefaultCursor[i] = SDL_CreateSystemCursor((SDL_SystemCursor)(i - 2));
	}

	staticCurrentCursor = staticDefaultCursor[2];
}

void EngineSurfaceWrap::setTitle(const char * title)
{
}

bool EngineSurfaceWrap::setFullscreenMode(int wide, int tall, int bpp)
{
	return false;
}

void EngineSurfaceWrap::setWindowedMode()
{
}

void EngineSurfaceWrap::setAsTopMost(bool state)
{
}

void EngineSurfaceWrap::createPopup(vgui::Panel * embeddedPanel)
{
}
