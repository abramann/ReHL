#pragma once

#include "interface.h"
#include "VGui.h"
#include "Color.h"

#define VGUI_SURFACE_INTERFACE_VERSION "VGUI_Surface026"

class IHTMLChromeController;
class IHTMLResponses;

namespace vgui2
{
	class IHTML;
	class IHTMLEvents; 
	class Image;
	struct VGuiVertex;

	class ISurface : public IBaseInterface
	{
	public:
		enum SurfaceFeature_e
		{
			ANTIALIASED_FONTS,
			DROPSHADOW_FONTS,
			ESCAPE_KEY,
			OPENING_NEW_HTML_WINDOWS,
			FRAME_MINIMIZE_MAXIMIZE,
			DIRECT_HWND_RENDER,
		}; 

		virtual void Shutdown() = 0;
		virtual void RunFrame() = 0;
		virtual vgui2::VPANEL GetEmbeddedPanel() = 0;
		virtual void SetEmbeddedPanel(vgui2::VPANEL panel) = 0;
		virtual void PushMakeCurrent(vgui2::VPANEL panel, bool useInsets) = 0;
		virtual void PopMakeCurrent(vgui2::VPANEL panel) = 0;
		virtual void DrawSetColor(int r, int g, int b, int a) = 0;
		virtual void DrawSetColor(Color col) = 0;
		virtual void DrawFilledRect(int x0, int y0, int x1, int y1) = 0;
		virtual void DrawOutlinedRect(int x0, int y0, int x1, int y1) = 0;
		virtual void DrawLine(int x0, int y0, int x1, int y1) = 0;
		virtual void DrawPolyLine(int *px, int *py, int n) = 0;
		virtual void DrawSetTextFont(vgui2::HFont font) = 0;
		virtual void DrawSetTextColor(int r, int g, int b, int a) = 0;
		virtual void DrawSetTextColor(Color col) = 0;
		virtual void DrawSetTextPos(int x, int y) = 0;
		virtual void DrawGetTextPos(int& x, int& y) = 0;
		virtual void DrawPrintText(const wchar_t *text, int textLen) = 0;
		virtual void DrawUnicodeChar(wchar_t ch_0) = 0;
		virtual void DrawUnicodeCharAdd(wchar_t wch) = 0;
		virtual void DrawFlushText() = 0;
		virtual vgui2::IHTML *CreateHTMLWindow(vgui2::IHTMLEvents *events, vgui2::VPANEL context) = 0;
		virtual void PaintHTMLWindow(vgui2::IHTML *htmlwin) = 0;
		virtual void DeleteHTMLWindow(vgui2::IHTML *htmlwin) = 0;
		virtual void DrawSetTextureFile(int id, const char *filename, int hardwareFilter, bool forceReload) = 0;
		virtual void DrawSetTextureRGBA(int id, const unsigned __int8 *rgba, int wide, int tall, int hardwareFilter, bool forceReload) = 0;
		virtual void DrawSetTexture(int id) = 0;
		virtual void DrawGetTextureSize(int id, int& wide, int& tall) = 0;
		virtual void DrawTexturedRect(int x0, int y0, int x1, int y1) = 0;
		virtual bool IsTextureIDValid(int id) = 0;
		virtual int CreateNewTextureID(bool procedural) = 0;
		virtual void GetScreenSize(int& wide, int& tall) = 0;
		virtual void SetAsTopMost(vgui2::VPANEL panel, bool state) = 0;
		virtual void BringToFront(vgui2::VPANEL panel) = 0;
		virtual void SetForegroundWindow(vgui2::VPANEL panel) = 0;
		virtual void SetPanelVisible(vgui2::VPANEL panel, bool state) = 0;
		virtual void SetMinimized(vgui2::VPANEL VPanel, bool state) = 0;
		virtual bool IsMinimized(vgui2::VPANEL panel) = 0;
		virtual void FlashWindow(vgui2::VPANEL, bool) = 0;
		virtual void SetTitle(vgui2::VPANEL panel, const wchar_t *title) = 0;
		virtual void SetAsToolBar(vgui2::VPANEL panel, bool state) = 0;
		virtual void CreatePopup(vgui2::VPANEL panel, bool minimised, bool showTaskbarIcon, bool disabled, bool mouseInput, bool kbInput) = 0;
		virtual void SwapBuffers(vgui2::VPANEL panel) = 0;
		virtual void Invalidate(vgui2::VPANEL panel) = 0;
		virtual void SetCursor(vgui2::HCursor cursor) = 0;
		virtual bool IsCursorVisible() = 0;
		virtual void ApplyChanges() = 0;
		virtual bool IsWithin(int x, int y) = 0;
		virtual bool HasFocus() = 0;
		virtual bool SupportsFeature(vgui2::ISurface::SurfaceFeature_e feature) = 0;
		virtual void RestrictPaintToSinglePanel(vgui2::VPANEL panel) = 0;
		virtual void SetModalPanel(vgui2::VPANEL) = 0;
		virtual vgui2::VPANEL GetModalPanel() = 0;
		virtual void UnlockCursor() = 0;
		virtual void LockCursor() = 0;
		virtual void SetTranslateExtendedKeys(bool state) = 0;
		virtual vgui2::VPANEL GetTopmostPopup() = 0;
		virtual void SetTopLevelFocus(vgui2::VPANEL subFocus) = 0;
		virtual vgui2::HFont CreateFont() = 0;
		virtual bool AddGlyphSetToFont(vgui2::HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange) = 0;
		virtual bool AddCustomFontFile(const char *fontFileName) = 0;
		virtual int GetFontTall(vgui2::HFont font) = 0;
		virtual void GetCharABCwide(vgui2::HFont font, int ch_0, int& a, int& b, int& c) = 0;
		virtual int GetCharacterWidth(vgui2::HFont font, int ch_0) = 0;
		virtual void GetTextSize(vgui2::HFont font, const wchar_t *text, int& wide, int& tall) = 0;
		virtual vgui2::VPANEL GetNotifyPanel() = 0;
		virtual void SetNotifyIcon(vgui2::VPANEL context, vgui2::HTexture icon, vgui2::VPANEL panelToReceiveMessages, const char *text) = 0;
		virtual void PlaySound(const char *fileName) = 0;
		virtual int GetPopupCount() = 0;
		virtual vgui2::VPANEL GetPopup(int index) = 0;
		virtual bool ShouldPaintChildPanel(vgui2::VPANEL panel) = 0;
		virtual bool RecreateContext(vgui2::VPANEL panel) = 0;
		virtual void AddPanel(vgui2::VPANEL panel) = 0;
		virtual void ReleasePanel(vgui2::VPANEL panel) = 0;
		virtual void MovePopupToFront(vgui2::VPANEL panel) = 0;
		virtual void MovePopupToBack(vgui2::VPANEL panel) = 0;
		virtual void SolveTraverse(vgui2::VPANEL panel, bool forceApplySchemeSettings) = 0;
		virtual void PaintTraverse(vgui2::VPANEL panel) = 0;
		virtual void EnableMouseCapture(vgui2::VPANEL panel, bool state) = 0;
		virtual void GetWorkspaceBounds(int& x, int& y, int& wide, int& tall) = 0;
		virtual void GetAbsoluteWindowBounds(int& x, int& y, int& wide, int& tall) = 0;
		virtual void GetProportionalBase(int& width, int& height) = 0;
		virtual void CalculateMouseVisible() = 0;
		virtual bool NeedKBInput() = 0;
		virtual bool HasCursorPosFunctions() = 0;
		virtual void SurfaceGetCursorPos(int& x, int& y) = 0;
		virtual void SurfaceSetCursorPos(int x, int y) = 0;
		virtual void DrawTexturedPolygon(vgui2::VGuiVertex* pVertices, int n) = 0;
		virtual int GetFontAscent(vgui2::HFont font, wchar_t wch) = 0;
		virtual void SetAllowHTMLJavaScript(bool state) = 0;
		virtual void SetLanguage(const char *pchLang) = 0;
		virtual const char *GetLanguage() = 0;
		virtual bool DeleteTextureByID(int id) = 0;
		virtual void DrawUpdateRegionTextureBGRA(int nTextureID, int x, int y, const unsigned __int8 *pchData, int wide, int tall) = 0;
		virtual void DrawSetTextureBGRA(int id, const unsigned __int8 *rgba, int wide, int tall) = 0;
		virtual void CreateBrowser(vgui2::VPANEL panel, IHTMLResponses *pBrowser, bool bPopupWindow, const char *pchUserAgentIdentifier) = 0;
		virtual void RemoveBrowser(vgui2::VPANEL panel, IHTMLResponses *pBrowser) = 0;
		virtual IHTMLChromeController *AccessChromeHTMLController() = 0;

		virtual bool setFullscreenMode(int wide, int tall, int bpp) = 0;
		virtual void setWindowedMode() = 0;
		virtual void SetAsTopMost_(bool state) = 0;
		virtual void SetAsToolBar_(bool state) = 0;
		virtual void PanelRequestFocus(vgui2::VPANEL panel) = 0;
		virtual void EnableMouseCapture_(bool state) = 0;
		virtual void DrawPrintChar(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1) = 0;
		virtual void SetNotifyIcon_(vgui2::Image *, vgui2::VPANEL, const char *) = 0;
		virtual bool SetWatchForComputerUse(bool state) = 0;
		virtual double GetTimeSinceLastUse() = 0;
	};

	ISurface *surface();
}