#pragma once

#include "IBaseSystem.h"
//#include "vgui.h"

typedef unsigned char uchar;

namespace vgui
{
	struct VGuiVertex;
}

class IEngineSurface : public IBaseInterface
{
public:
	virtual ~IEngineSurface();

	virtual void pushMakeCurrent(int *xTranlate, int * p_x0, int * clipRect, bool translateToScreenSpace) = 0;
	virtual void popMakeCurrent(void) = 0;
	virtual void drawFilledRect(int x0, int y0, int x1, int y1) = 0;
	virtual void drawOutlinedRect(int x0, int y0, int x1, int y1) = 0;
	virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
	virtual void drawPolyLine(int *px, int *py, int numPoints) = 0;
	virtual void drawTexturedPolygon(vgui::VGuiVertex * pVertices, int n) = 0;
	virtual void drawSetTextureRGBA(int id, const char *rgba, int wide, int tall, int hardwareFilter, int hasAlphaChannel) = 0;
	virtual void drawSetTexture(int id) = 0;
	virtual void drawTexturedRect(int x0, int y0, int x1, int y1) = 0;
	virtual int createNewTextureID(void) = 0;
	virtual void drawSetColor(int r, int g, int b, int a) = 0;
	virtual void drawSetTextColor(int r, int g, int b, int a) = 0;
	virtual void drawSetTextPos(int x, int y) = 0;
	virtual void drawGetTextPos(int &x, int &y) = 0;
	virtual void drawPrintChar(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1) = 0;
	virtual void drawPrintCharAdd(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1) = 0;
	virtual void drawSetTextureFile(int id, const char *filename, int hardwareFilter, bool forceReload) = 0;
	virtual void drawGetTextureSize(int id, int *const wide, int *const tall) = 0;
	virtual int isTextureIdValid(int id) = 0;
	virtual void drawSetSubTextureRGBA(int textureID, int drawX, int drawY, const unsigned __int8 *rgba, int subTextureWide, int subTextureTall) = 0;
	virtual void drawFlushText(void) = 0;
	virtual void resetViewPort(void) = 0;
	virtual void drawSetTextureBGRA(int id, const char *rgba, int wide, int tall, int hardwareFilter, int hasAlphaChannel) = 0;
	virtual void drawUpdateRegionTextureBGRA(int nTextureID, int x, int y, const uchar *pchData, int wide, int tall) = 0;
};

class EngineSurface : public IEngineSurface
{
public:
	virtual ~EngineSurface();

	virtual void pushMakeCurrent(int *xTranlate, int * p_x0, int * clipRect, bool translateToScreenSpace);
	virtual void popMakeCurrent(void);
	virtual void drawFilledRect(int x0, int y0, int x1, int y1);
	virtual void drawOutlinedRect(int x0, int y0, int x1, int y1);
	virtual void drawLine(int x1, int y1, int x2, int y2);
	virtual void drawPolyLine(int *px, int *py, int numPoints);
	virtual void drawTexturedPolygon(vgui::VGuiVertex * pVertices, int n);
	virtual void drawSetTextureRGBA(int id, const char *rgba, int wide, int tall, int hardwareFilter, int hasAlphaChannel);
	virtual void drawSetTexture(int id);
	virtual void drawTexturedRect(int x0, int y0, int x1, int y1);
	virtual int createNewTextureID(void);
	virtual void drawSetColor(int r, int g, int b, int a);
	virtual void drawSetTextColor(int r, int g, int b, int a);
	virtual void drawSetTextPos(int x, int y);
	virtual void drawGetTextPos(int &x, int &y);
	virtual void drawPrintChar(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1);
	virtual void drawPrintCharAdd(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1);
	virtual void drawSetTextureFile(int id, const char *filename, int hardwareFilter, bool forceReload);
	virtual void drawGetTextureSize(int id, int *const wide, int *const tall);
	virtual int isTextureIdValid(int id);
	virtual void drawSetSubTextureRGBA(int textureID, int drawX, int drawY, const unsigned __int8 *rgba, int subTextureWide, int subTextureTall);
	virtual void drawFlushText(void);
	virtual void resetViewPort(void);
	virtual void drawSetTextureBGRA(int id, const char *rgba, int wide, int tall, int hardwareFilter, int hasAlphaChannel);
	virtual void drawUpdateRegionTextureBGRA(int nTextureID, int x, int y, const uchar *pchData, int wide, int tall);

	// non-virtual
	NOBODY void createRenderPlat();
	NOBODY void deleteRenderPlat();
	static void freeEngineSurface();

	static EngineSurface *s_pEngineSurface;
};
