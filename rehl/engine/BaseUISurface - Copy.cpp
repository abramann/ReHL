#include "precompiled.h"
#include "BaseUISurface.h"
#include "vgui2\ISchemeManager.h"
#include "IHTMLChromeController.h"
#include <SDL.h>
#include <SDL_system.h>

static int staticContextCount;
static SDL_Cursor* staticDefaultCursor[20];
static SDL_Cursor* staticCurrentCursor;

static vgui2::Dar<vgui2::VPANEL> staticPopupList;

cvar_t sdl_double_click_size = { "sdl_double_click_size", "2" };
cvar_t sdl_double_click_time = { "sdl_double_click_time", "400" };;

BaseUISurface g_BaseUISurface;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(BaseUISurface, ISurface, VGUI_SURFACE_INTERFACE_VERSION, g_BaseUISurface);

void BaseUISurface::Init(vgui2::VPANEL embeddedPanel, IEngineSurface * engineSurface, IHTMLChromeController * pChromeController)
{
	_engineSurface = engineSurface;
	SetEmbeddedPanel(embeddedPanel);
	if (SteamApps())
	{
		auto lang = SteamApps()->GetCurrentGameLanguage();
		SetLanguage(lang);
	}

	CHECK_REQUIRED;
	m_pEmulatedMouseImage = vgui2::scheme()->GetImage("gfx/vgui/mouse", false);

	AddCustomFontFile("valve/resource/marlett.ttf");

	Cvar_RegisterVariable(&sdl_double_click_size);
	Cvar_RegisterVariable(&sdl_double_click_time);

	m_pChromeController = pChromeController;
	if (pChromeController)
	{
		pChromeController->Init("htmlcache", "htmlcookies");
		m_pChromeController->SetCefThreadTargetFrameRate(60);
	}
}

bool BaseUISurface::m_bTranslateExtendedKeys = true;

BaseUISurface::BaseUISurface()
{
	++staticContextCount;
	_restrictedPanel = 0;
	m_hCurrentFont = 0;
	_cursorLocked = 0;
	m_iCurrentTexture = 0;
	m_pChromeController = 0;
	_needMouse = true;
	_needKB = true;
	m_bVGUI2MouseControl = 0;
	m_bFullScreen = 0;
	m_deltay = 0;
	m_deltax = 0;

	m_bTranslateExtendedKeys = true;
	if (!SDL_WasInit(32))
	{
		SDL_SetHint("SDL_VIDEO_X11_XRANDR", "1");
		SDL_SetHint("SDL_VIDEO_X11_XVIDMODE", "1");
		SDL_InitSubSystem(32);
	}
	staticDefaultCursor[1] = 0;
	staticDefaultCursor[2] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	staticDefaultCursor[3] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	staticDefaultCursor[4] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
	staticDefaultCursor[6] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
	staticDefaultCursor[7] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
	staticDefaultCursor[8] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
	staticDefaultCursor[9] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
	staticDefaultCursor[10] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
	staticDefaultCursor[11] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
	staticDefaultCursor[12] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
	staticDefaultCursor[13] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
	staticDefaultCursor[14] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	staticCurrentCursor = staticDefaultCursor[2];
}

BaseUISurface::~BaseUISurface()
{
	NOT_IMPLEMENTED;
}

void BaseUISurface::Shutdown()
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::RunFrame()
{	
}

vgui2::VPANEL BaseUISurface::GetEmbeddedPanel()
{
	NOT_IMPLEMENTED;

	return vgui2::VPANEL();
}

void BaseUISurface::SetEmbeddedPanel(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::PushMakeCurrent(vgui2::VPANEL panel, bool useInsets)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::PopMakeCurrent(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawSetColor(int r, int g, int b, int a)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawSetColor(Color col)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawFilledRect(int x0, int y0, int x1, int y1)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawOutlinedRect(int x0, int y0, int x1, int y1)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawLine(int x0, int y0, int x1, int y1)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawPolyLine(int * px, int * py, int n)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawSetTextFont(vgui2::HFont font)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawSetTextColor(int r, int g, int b, int a)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawSetTextColor(Color col)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawSetTextPos(int x, int y)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawGetTextPos(int * const x, int * const y)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawPrintText(const wchar_t * text, int textLen)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawUnicodeChar(wchar_t ch_0)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawUnicodeCharAdd(wchar_t wch)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawFlushText()
{
	NOT_IMPLEMENTED;

}

vgui2::IHTML * BaseUISurface::CreateHTMLWindow(vgui2::IHTMLEvents * events, vgui2::VPANEL context)
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void BaseUISurface::PaintHTMLWindow(vgui2::IHTML * htmlwin)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DeleteHTMLWindow(vgui2::IHTML * htmlwin)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawSetTextureFile(int id, const char * filename, int hardwareFilter, bool forceReload)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawSetTextureRGBA(int id, const unsigned __int8 * rgba, int wide, int tall, int hardwareFilter, bool forceReload)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawSetTexture(int id)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawGetTextureSize(int id, int * const wide, int * const tall)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawTexturedRect(int x0, int y0, int x1, int y1)
{
	NOT_IMPLEMENTED;

}

bool BaseUISurface::IsTextureIDValid(int id)
{
	NOT_IMPLEMENTED;

	return false;
}

int BaseUISurface::CreateNewTextureID(bool procedural)
{
	NOT_IMPLEMENTED;

	return 0;
}

void BaseUISurface::GetScreenSize(int & wide, int & tall)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetAsTopMost(vgui2::VPANEL panel, bool state)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::BringToFront(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetForegroundWindow(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetPanelVisible(vgui2::VPANEL panel, bool state)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetMinimized(vgui2::VPANEL VPanel, bool state)
{
	NOT_IMPLEMENTED;

}

bool BaseUISurface::IsMinimized(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

	return false;
}

void BaseUISurface::FlashWindow(vgui2::VPANEL, bool)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetTitle(vgui2::VPANEL panel, const wchar_t * title)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetAsToolBar(vgui2::VPANEL panel, bool state)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::CreatePopup(vgui2::VPANEL panel, bool minimised, bool showTaskbarIcon, bool disabled, bool mouseInput, bool kbInput)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SwapBuffers(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::Invalidate(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetCursor(vgui2::HCursor cursor)
{
	NOT_IMPLEMENTED;

}

bool BaseUISurface::IsCursorVisible()
{
	return false;
}

void BaseUISurface::ApplyChanges()
{
	NOT_IMPLEMENTED;

}

bool BaseUISurface::IsWithin(int x, int y)
{
	NOT_IMPLEMENTED;

	return false;
}

bool BaseUISurface::HasFocus()
{
	NOT_IMPLEMENTED;

	return false;
}

bool BaseUISurface::SupportsFeature(vgui2::ISurface::SurfaceFeature_e feature)
{
	NOT_IMPLEMENTED;

	return false;
}

void BaseUISurface::RestrictPaintToSinglePanel(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetModalPanel(vgui2::VPANEL)
{
	NOT_IMPLEMENTED;

}

vgui2::VPANEL BaseUISurface::GetModalPanel()
{
	NOT_IMPLEMENTED;

	return vgui2::VPANEL();
}

void BaseUISurface::UnlockCursor()
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::LockCursor()
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetTranslateExtendedKeys(bool state)
{
	NOT_IMPLEMENTED;

}

vgui2::VPANEL BaseUISurface::GetTopmostPopup()
{
	NOT_IMPLEMENTED;

	return vgui2::VPANEL();
}

void BaseUISurface::SetTopLevelFocus(vgui2::VPANEL subFocus)
{
	NOT_IMPLEMENTED;

}

vgui2::HFont BaseUISurface::CreateFont()
{
	NOT_IMPLEMENTED;

	return vgui2::HFont();
}

bool BaseUISurface::AddGlyphSetToFont(vgui2::HFont font, const char * windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange)
{
	NOT_IMPLEMENTED;

	return false;
}

bool BaseUISurface::AddCustomFontFile(const char * fontFileName)
{
	NOT_IMPLEMENTED;

	return false;
}

int BaseUISurface::GetFontTall(vgui2::HFont font)
{
	NOT_IMPLEMENTED;

	return 0;
}

void BaseUISurface::GetCharABCwide(vgui2::HFont font, int ch_0, int * const a, int * const b, int * const c)
{
	NOT_IMPLEMENTED;

}

int BaseUISurface::GetCharacterWidth(vgui2::HFont font, int ch_0)
{
	NOT_IMPLEMENTED;

	return 0;
}

void BaseUISurface::GetTextSize(vgui2::HFont font, const wchar_t * text, int * const wide, int * const tall)
{
	NOT_IMPLEMENTED;

}

vgui2::VPANEL BaseUISurface::GetNotifyPanel()
{
	NOT_IMPLEMENTED;

	return vgui2::VPANEL();
}

void BaseUISurface::SetNotifyIcon(vgui2::VPANEL context, vgui2::HTexture icon, vgui2::VPANEL panelToReceiveMessages, const char * text)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::PlaySound(const char * fileName)
{
	NOT_IMPLEMENTED;

}

int BaseUISurface::GetPopupCount()
{
	NOT_IMPLEMENTED;

	return 0;
}

vgui2::VPANEL BaseUISurface::GetPopup(int index)
{
	NOT_IMPLEMENTED;

	return vgui2::VPANEL();
}

bool BaseUISurface::ShouldPaintChildPanel(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

	return false;
}

bool BaseUISurface::RecreateContext(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

	return false;
}

void BaseUISurface::AddPanel(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::ReleasePanel(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::MovePopupToFront(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::MovePopupToBack(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SolveTraverse(vgui2::VPANEL panel, bool forceApplySchemeSettings)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::PaintTraverse(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::EnableMouseCapture(vgui2::VPANEL panel, bool state)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::GetWorkspaceBounds(int * const x, int * const y, int * const wide, int * const tall)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::GetAbsoluteWindowBounds(int * const x, int * const y, int * const wide, int * const tall)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::GetProportionalBase(int * const width, int * const height)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::CalculateMouseVisible()
{
	NOT_IMPLEMENTED;

}

bool BaseUISurface::NeedKBInput()
{
	NOT_IMPLEMENTED;

	return false;
}

bool BaseUISurface::HasCursorPosFunctions()
{
	NOT_IMPLEMENTED;

	return false;
}

void BaseUISurface::SurfaceGetCursorPos(int * const x, int * const y)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SurfaceSetCursorPos(int x, int y)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawTexturedPolygon(vgui2::VGuiVertex * pVertices, int n)
{
	NOT_IMPLEMENTED;

}

int BaseUISurface::GetFontAscent(vgui2::HFont font, wchar_t wch)
{
	NOT_IMPLEMENTED;

	return 0;
}

void BaseUISurface::SetAllowHTMLJavaScript(bool state)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetLanguage(const char * pchLang)
{
	NOT_IMPLEMENTED;

}

const char * BaseUISurface::GetLanguage()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

bool BaseUISurface::DeleteTextureByID(int id)
{
	NOT_IMPLEMENTED;

	return false;
}

void BaseUISurface::DrawUpdateRegionTextureBGRA(int nTextureID, int x, int y, const unsigned __int8 * pchData, int wide, int tall)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawSetTextureBGRA(int id, const unsigned __int8 * rgba, int wide, int tall)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::CreateBrowser(vgui2::VPANEL panel, IHTMLResponses * pBrowser, bool bPopupWindow, const char * pchUserAgentIdentifier)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::RemoveBrowser(vgui2::VPANEL panel, IHTMLResponses * pBrowser)
{
	NOT_IMPLEMENTED;

}

IHTMLChromeController * BaseUISurface::AccessChromeHTMLController()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

bool BaseUISurface::setFullscreenMode(int wide, int tall, int bpp)
{
	NOT_IMPLEMENTED;

	return false;
}

void BaseUISurface::setWindowedMode()
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetAsTopMost(bool state)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetAsToolBar(bool state)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::PanelRequestFocus(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::EnableMouseCapture(bool state)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::DrawPrintChar(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1)
{
	NOT_IMPLEMENTED;

}

void BaseUISurface::SetNotifyIcon(vgui2::Image *, vgui2::VPANEL, const char *)
{
	NOT_IMPLEMENTED;

}

bool BaseUISurface::SetWatchForComputerUse(bool state)
{
	NOT_IMPLEMENTED;

	return false;
}

double BaseUISurface::GetTimeSinceLastUse()
{
	NOT_IMPLEMENTED;

	return 0.0;
}

bool BaseUISurface::VGUI2MouseControl()
{
	NOT_IMPLEMENTED;

	return false;
}

void BaseUISurface::SetVGUI2MouseControl(bool state)
{
	NOT_IMPLEMENTED;
}

