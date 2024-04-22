#pragma once

#include "vgui2/ISurface.h"
#include "IMouseControl.h"
#include "utlvector.h"
#include "utlsymbol.h"
#include "vgui2/MouseCode.h"

extern IMouseControl* mousecontrol;

namespace vgui2
{
	class IImage;
}

class BaseUISurface : public vgui2::ISurface, public IMouseControl
{
public:
	
	void Init(vgui2::VPANEL embeddedPanel, IEngineSurface *engineSurface, IHTMLChromeController *pChromeController);

	BaseUISurface();

	virtual ~BaseUISurface();

	// ISurface
	virtual void Shutdown();
	virtual void RunFrame();
	virtual vgui2::VPANEL GetEmbeddedPanel();
	virtual void SetEmbeddedPanel(vgui2::VPANEL panel);
	virtual void PushMakeCurrent(vgui2::VPANEL panel, bool useInsets);
	virtual void PopMakeCurrent(vgui2::VPANEL panel);
	virtual void DrawSetColor(int r, int g, int b, int a);
	virtual void DrawSetColor(Color col);
	virtual void DrawFilledRect(int x0, int y0, int x1, int y1);
	virtual void DrawOutlinedRect(int x0, int y0, int x1, int y1);
	virtual void DrawLine(int x0, int y0, int x1, int y1);
	virtual void DrawPolyLine(int *px, int *py, int n);
	virtual void DrawSetTextFont(vgui2::HFont font);
	virtual void DrawSetTextColor(int r, int g, int b, int a);
	virtual void DrawSetTextColor(Color col);
	virtual void DrawSetTextPos(int x, int y);
	virtual void DrawGetTextPos(int& x, int& y);
	virtual void DrawPrintText(const wchar_t *text, int textLen);
	virtual void DrawUnicodeChar(wchar_t ch_0);
	virtual void DrawUnicodeCharAdd(wchar_t wch);
	virtual void DrawFlushText();
	virtual vgui2::IHTML *CreateHTMLWindow(vgui2::IHTMLEvents *events, vgui2::VPANEL context);
	virtual void PaintHTMLWindow(vgui2::IHTML *htmlwin);
	virtual void DeleteHTMLWindow(vgui2::IHTML *htmlwin);
	virtual void DrawSetTextureFile(int id, const char *filename, int hardwareFilter, bool forceReload);
	virtual void DrawSetTextureRGBA(int id, const unsigned __int8 *rgba, int wide, int tall, int hardwareFilter, bool forceReload);
	virtual void DrawSetTexture(int id);
	virtual void DrawGetTextureSize(int id, int& wide, int& tall);
	virtual void DrawTexturedRect(int x0, int y0, int x1, int y1);
	virtual bool IsTextureIDValid(int id);
	virtual int CreateNewTextureID(bool procedural);
	virtual void GetScreenSize(int& wide, int& tall);
	virtual void SetAsTopMost(vgui2::VPANEL panel, bool state);
	virtual void BringToFront(vgui2::VPANEL panel);
	virtual void SetForegroundWindow(vgui2::VPANEL panel);
	virtual void SetPanelVisible(vgui2::VPANEL panel, bool state);
	virtual void SetMinimized(vgui2::VPANEL VPanel, bool state);
	virtual bool IsMinimized(vgui2::VPANEL panel);
	virtual void FlashWindow(vgui2::VPANEL, bool);
	virtual void SetTitle(vgui2::VPANEL panel, const wchar_t *title);
	virtual void SetAsToolBar(vgui2::VPANEL panel, bool state);
	virtual void CreatePopup(vgui2::VPANEL panel, bool minimised, bool showTaskbarIcon, bool disabled, bool mouseInput, bool kbInput);
	virtual void SwapBuffers(vgui2::VPANEL panel);
	virtual void Invalidate(vgui2::VPANEL panel);
	virtual void SetCursor(vgui2::HCursor cursor);
	virtual bool IsCursorVisible();
	virtual void ApplyChanges();
	virtual bool IsWithin(int x, int y);
	virtual bool HasFocus();
	virtual bool SupportsFeature(vgui2::ISurface::SurfaceFeature_e feature);
	virtual void RestrictPaintToSinglePanel(vgui2::VPANEL panel);
	virtual void SetModalPanel(vgui2::VPANEL);
	virtual vgui2::VPANEL GetModalPanel();
	virtual void UnlockCursor();
	virtual void LockCursor();
	virtual void SetTranslateExtendedKeys(bool state);
	virtual vgui2::VPANEL GetTopmostPopup();
	virtual void SetTopLevelFocus(vgui2::VPANEL subFocus);
	virtual vgui2::HFont CreateFont();
	virtual bool AddGlyphSetToFont(vgui2::HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange);
	virtual bool AddCustomFontFile(const char *fontFileName);
	virtual int GetFontTall(vgui2::HFont font);
	virtual void GetCharABCwide(vgui2::HFont font, int ch_0, int& a, int&b, int& c);
	virtual int GetCharacterWidth(vgui2::HFont font, int ch_0);
	virtual void GetTextSize(vgui2::HFont font, const wchar_t *text, int& wide, int& tall);
	virtual vgui2::VPANEL GetNotifyPanel();
	virtual void SetNotifyIcon(vgui2::VPANEL context, vgui2::HTexture icon, vgui2::VPANEL panelToReceiveMessages, const char *text);
	virtual void PlaySound(const char *fileName);
	virtual int GetPopupCount();
	virtual vgui2::VPANEL GetPopup(int index);
	virtual bool ShouldPaintChildPanel(vgui2::VPANEL panel);
	virtual bool RecreateContext(vgui2::VPANEL panel);
	virtual void AddPanel(vgui2::VPANEL panel);
	virtual void ReleasePanel(vgui2::VPANEL panel);
	virtual void MovePopupToFront(vgui2::VPANEL panel);
	virtual void MovePopupToBack(vgui2::VPANEL panel);
	virtual void SolveTraverse(vgui2::VPANEL panel, bool forceApplySchemeSettings);
	virtual void PaintTraverse(vgui2::VPANEL panel);
	virtual void EnableMouseCapture(vgui2::VPANEL panel, bool state);
	virtual void GetWorkspaceBounds(int& x, int& y, int& wide, int& tall);
	virtual void GetAbsoluteWindowBounds(int& x, int& y, int& wide, int& tall);
	virtual void GetProportionalBase(int& width, int& height);
	virtual void CalculateMouseVisible();
	virtual bool NeedKBInput();
	virtual bool HasCursorPosFunctions();
	virtual void SurfaceGetCursorPos(int& x, int& y);
	virtual void SurfaceSetCursorPos(int x, int y);
	virtual void DrawTexturedPolygon(vgui2::VGuiVertex *pVertices, int n);
	virtual int GetFontAscent(vgui2::HFont font, wchar_t wch);
	virtual void SetAllowHTMLJavaScript(bool state);
	virtual void SetLanguage(const char *pchLang);
	virtual const char *GetLanguage();
	virtual bool DeleteTextureByID(int id);
	virtual void DrawUpdateRegionTextureBGRA(int nTextureID, int x, int y, const unsigned __int8 *pchData, int wide, int tall);
	virtual void DrawSetTextureBGRA(int id, const unsigned __int8 *rgba, int wide, int tall);
	virtual void CreateBrowser(vgui2::VPANEL panel, IHTMLResponses *pBrowser, bool bPopupWindow, const char *pchUserAgentIdentifier);
	virtual void RemoveBrowser(vgui2::VPANEL panel, IHTMLResponses *pBrowser);
	virtual IHTMLChromeController *AccessChromeHTMLController();
	virtual bool setFullscreenMode(int wide, int tall, int bpp);

	virtual void setWindowedMode();
	virtual void SetAsTopMost_(bool state);
	virtual void SetAsToolBar_(bool state);
	virtual void PanelRequestFocus(vgui2::VPANEL panel);
	virtual void EnableMouseCapture_(bool state);
	virtual void DrawPrintChar(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1);
	virtual void SetNotifyIcon_(vgui2::Image *, vgui2::VPANEL, const char *);
	virtual bool SetWatchForComputerUse(bool state);
	virtual double GetTimeSinceLastUse();

	// IMouseControl
	virtual bool VGUI2MouseControl();
	virtual void SetVGUI2MouseControl(bool state);

	// non virtual
	void GetMouseDelta(int& x, int& y);
	void SetScreenBounds(int x, int y, int wide, int tall);
	void AppHandler(void* event, void* userData);
	void DrawSetTextureRGBAWithAlphaChannel(int id, const unsigned char* rgba, int wide, int tall, int hardwareFilter);
	void DrawSetSubTextureRGBA(int textureID, int drawX, int drawY, const byte* rgba, int subTextureWide, int subTextureTall);
	void InternalSchemeSettingsTraverse(vgui2::VPANEL panel, bool forceApplySchemeSettings);
	void InternalThinkTraverse(vgui2::VPANEL panel);
	void InternalSolveTraverse(vgui2::VPANEL panel);

	static bool m_bTranslateExtendedKeys;

//private:
	vgui2::IImage *m_pEmulatedMouseImage;
	IEngineSurface *_engineSurface;
	vgui2::VPANEL _embeddedPanel;
	vgui2::HCursor _currentCursor;
	vgui2::VPANEL _restrictedPanel;
	vgui2::HFont m_hCurrentFont;
	int m_iCurrentTexture;
	int m_iSurfaceBounds[4];
	CUtlVector<CUtlSymbol> m_CustomFontFileNames;
	IHTMLChromeController *m_pChromeController;
	bool _cursorLocked;
	bool _needMouse;
	bool _needKB;
	bool m_bVGUI2MouseControl;
	bool m_bAllowJavaScript;
	char m_szLanguage[32];
	bool m_bFullScreen;
	int m_deltax;
	int m_deltay;
	bool m_bIgnoreMouseVisCalc;
	unsigned int m_MouseButtonDownTimeStamp;
	int m_MouseButtonDownX;
	int m_MouseButtonDownY;
	bool m_bGotMouseButtonDown;
};

extern BaseUISurface g_BaseUISurface;