#pragma once

#include "dll_api.h"
#include "Dar.h"


namespace vgui
{
	struct VGuiVertex;
}

namespace vgui
{
	class App;
	class Cursor;
	class Panel;
	class ImagePanel;
	class Font;

	class VGUI_DLL_API SurfaceBase
	{
	protected:
		~SurfaceBase();

		virtual void addModeInfo(int wide, int tall, int bpp);

	public:
		SurfaceBase(vgui::Panel* pPanel);
		SurfaceBase(const SurfaceBase& surfaceBase);

		virtual vgui::Panel * getPanel(void);
		virtual void requestSwap(void);
		virtual void resetModeInfo(void);
		virtual int getModeInfoCount(void);
		virtual bool getModeInfo(int index, int& wide, int& tall, int& bpp);
		virtual vgui::App* getApp(void);
		virtual void setEmulatedCursorVisible(bool state);
		virtual void setEmulatedCursorPos(int x, int y);
		virtual void setTitle(const char* title) = 0;
		virtual bool setFullscreenMode(int wide, int tall, int bpp) = 0;
		virtual void setWindowedMode() = 0;
		virtual void setAsTopMost(bool state) = 0;
		virtual void createPopup(vgui::Panel* embeddedPanel) = 0;
		virtual bool hasFocus() = 0;
		virtual bool isWithin(int x, int y) = 0;
		virtual int createNewTextureID() = 0;
		virtual void GetMousePos(int& x, int& y) = 0;
		virtual void drawSetColor(int r, int g, int b, int a) = 0;
		virtual void drawFilledRect(int x0, int y0, int x1, int y1) = 0;
		virtual void drawOutlinedRect(int x0, int y0, int x1, int y1) = 0;
		virtual void drawSetTextFont(vgui::Font* font) = 0;
		virtual void drawSetTextColor(int r, int g, int b, int a) = 0;
		virtual void drawSetTextPos(int x, int y) = 0;
		virtual void drawPrintText(const char* text, int textLen) = 0;
		virtual void drawSetTextureRGBA(int id, const  unsigned char * rgba, int wide, int tall) = 0;
		virtual void drawSetTexture(int id) = 0;
		virtual void drawTexturedRect(int x0, int y0, int x1, int y1) = 0;
		virtual void invalidate(vgui::Panel* panel) = 0;
		virtual void enableMouseCapture(bool state) = 0;
		virtual void setCursor(vgui::Cursor* cursor) = 0;
		virtual void swapBuffers() = 0;
		virtual void pushMakeCurrent(vgui::Panel* panel, bool useInsets) = 0;
		virtual void popMakeCurrent(vgui::Panel* panel) = 0;
		virtual void applyChanges() = 0;

		// TODO: more virtual functions, possibly part of an interface - Solokiller
		virtual void AppHandler(void* event, void* userData) = 0;
		virtual void lockCursor() = 0;
		virtual void unlockCursor() = 0;

		virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
		virtual void drawPolyLine(int* px, int* py, int n) = 0;
		virtual void drawTexturedPolygon(vgui::VGuiVertex* pVertices, int n) = 0;
		virtual void drawSetTextureBGRA(int id, const  unsigned char * rgba, int wide, int tall, int hardwareFilter, int hasAlphaChannel) = 0;
		virtual void drawUpdateRegionTextureBGRA(int nTextureID, int x, int y, const byte* pchData, int wide, int tall) = 0;
		virtual void drawGetTextPos(int& x, int& y) = 0;
		virtual void drawPrintChar(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1) = 0;
		virtual void drawPrintCharAdd(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1) = 0;

	private:
		bool _needsSwap;
		vgui::App *_app;
		vgui::Panel *_embeddedPanel;
		Dar<char*> _modeInfoDar;
		vgui::ImagePanel *_emulatedCursor;
		vgui::Cursor *_currentCursor;
	};
}