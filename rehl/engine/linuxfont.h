#pragma once

#include "UtlrbTree.h"
#ifdef _WIN32
#include <Windows.h>
#endif

typedef void* GLXContext;

class CLinuxFont
{
public:
	struct abc_t
	{
		char a;
		char b;
		char c;
		char reserved;
	};

	struct abc_cache_t
	{
		wchar_t wch;
		abc_t abc;
	};

	struct font_name_entry
	{
		char *m_pchFile;
		char *m_pchFriendlyName;
	};
#ifdef _WIN32
#ifndef SHARE_GAME_DATA
	static bool ms_bIdentifiedOSVersion;
	static bool ms_bIsWindowsXPorLater;
#endif
#else
	static bool  ms_bSetFriendlyNameCacheLessFunc;
	static CUtlRBTree<CLinuxFont::font_name_entry, int> m_FriendlyNameCache;

#endif
	CLinuxFont();
	~CLinuxFont();

	static bool ExtendedABCWidthsCacheLessFunc(const abc_cache_t *const lhs, const abc_cache_t *const rhs);

	bool FindFontDataFile(const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, char *dataFile, int dataFileChars);
	bool Create(const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags);
	void ApplyRotaryEffectToTexture(int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, unsigned __int8 *rgba);
	void ApplyScanlineEffectToTexture(int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, unsigned __int8 *rgba);
	void ApplyDropShadowToTexture(int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, int charWide, int charTall, unsigned __int8 *rgba);
	void ApplyOutlineToTexture(int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, int charWide, int charTall, unsigned __int8 *rgba);
	void ApplyGaussianBlurToTexture(int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, unsigned __int8 *rgba);
	void GetCharRGBA(int ch_0, int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, unsigned __int8 *prgba);
	bool IsEqualTo(const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags);
	bool IsValid();
	int GetHeight();
	int GetAscent();
	int GetMaxCharWidth();
	const char* GetFontName();
	bool GetUnderlined();
	int GetFlags();
	void *SetAsActiveFont(GLXContext cglContext);
	bool HasChar(wchar_t wch);
	void GetCharABCWidths_(int ch, int& a, int& b, int& c);
private:
	char m_szName[32];
	int m_iTall;
	int m_iWeight;
	int m_iFlags;
	bool m_bAntiAliased; 
	bool m_bRotary;
	bool m_bAdditive;
	int m_iDropShadowOffset;
	bool m_bUnderlined; // Same offset
#ifndef _WIN32
	int m_iOutlineSize;
#endif
	int m_iHeight;
	int m_iMaxCharWidth;
	int m_iAscent; // Win Off=0x40, Linux Off=0x44
	abc_t m_abcWidths[128]; // Win Off=0x44v confirmed
	CUtlRBTree<abc_cache_t, short unsigned int> m_ExtendedABCWidthsCache; // Win Off=0x244 Lin Off=0x48
#ifdef _WIN32
	HFONT m_hFont; // Off=0x25C
	HDC m_hdc; // off=0x260
	HBITMAP m_hBitmap;
#endif
	int m_rgiBitmapSize[2];
	char *m_pBuf; // off=0x270 confirmed
	int m_iScanLines; // off=0x274 confimed
	int m_iBlur; // off=0x278 confirmed
	float *m_pGaussianDistribution; // confimed
#ifdef _WIN32
#else
	FT_Face face;
	int m_iFxpHeight;
#endif
};
