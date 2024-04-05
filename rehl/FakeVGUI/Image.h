#pragma once

#include "dll_api.h"
#include "Scheme.h"


namespace vgui
{
	class InputStream;
	class Panel;
	class Color;
	class Font;

	class VGUI_DLL_API Image
	{
	public:
		virtual void setPos(int x, int y);
		virtual void getPos(int *const x, int *const y);
		virtual void getSize(int *const wide, int *const tall);
		virtual void setColor(vgui::Color *p_color);
		virtual void getColor(vgui::Color *const color);
		virtual void setSize(int wide, int tall);
		virtual void drawSetColor(vgui::Scheme::SchemeColor sc);
		virtual void drawSetColor(int r, int g, int b, int a);
		virtual void drawFilledRect(int x0, int y0, int x1, int y1);
		virtual void drawOutlinedRect(int x0, int y0, int x1, int y1);
		virtual void drawSetTextFont(vgui::Scheme::SchemeFont sf);
		virtual void drawSetTextFont(vgui::Font *font);
		virtual void drawSetTextColor(vgui::Scheme::SchemeColor sc);
		virtual void drawSetTextColor(int r, int g, int b, int a);
		virtual void drawSetTextPos(int x, int y);
		virtual void drawPrintText(const char *str, int strlen);
		virtual void drawPrintText(int x, int y, const char *str, int strlen);
		virtual void drawPrintChar(char ch);
		virtual void drawPrintChar(int x, int y, char ch_0);
		virtual void drawSetTextureRGBA(int id, const char *rgba, int wide, int tall);
		virtual void drawSetTexture(int id);
		virtual void drawTexturedRect(int x0, int y0, int x1, int y1);
		virtual void paint(vgui::Panel *panel);
		virtual void doPaint(vgui::Panel *panel);
	};

	class VGUI_DLL_API Bitmap : public vgui::Image
	{
	public:
		virtual void setSize(int wide, int tall);
		virtual void paint(vgui::Panel *panel);
		virtual void setRGBA(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	};

	class VGUI_DLL_API BitmapTGA : public Bitmap
	{
	public:
		BitmapTGA(vgui::InputStream *is, bool invertAlpha);

		virtual bool loadTGA(vgui::InputStream *is, bool invertAlpha);
	};
}