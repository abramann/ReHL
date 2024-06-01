#include "FontManager.h"
#include "FontAmalgma.h"
#include "precompiled.h"


#ifdef _WIN32
#include <Windows.h>

const char* g_szValidAsianFonts[] = { "Marlett" };
const char* ForeignFallbackFonts[] = { "russian", "system", "japanese", "win98japanese", "thai", "system"};
const char* windowsFontName = "Tahoma";
#else
const char* g_szValidAsianFonts[] = { "Marlett" , "WenQuanYi Zen Hei", "unifont" };
const char* windowsFontName = "WenQuanYi Zen Hei";

#error The content of the array must be checked with USE_DEFAULT_FOREIGN_FALLBACK_FONT = false because GCC skipped it

const char* ForeignFallbackFonts[] = { "russian", "system", "japanese", "win98japanese", "thai", "system" };
#endif

const bool USE_DEFAULT_FOREIGN_FALLBACK_FONT = true;

#ifdef SHARE_GAME_DATA
CFontManager* p_g_FontManager = (CFontManager*)ADDRESS_OF_DATA(0xCFA01);
CFontManager& g_FontManager = *p_g_FontManager;
#else
CFontManager g_FontManager;
#endif

CFontManager::CFontManager() : m_FontAmalgams(100)
{
	m_FontAmalgams.InsertBefore(m_FontAmalgams.Count());

#ifdef _WIN32
	setlocale(LC_CTYPE, "");
	setlocale(LC_TIME, "");
	setlocale(LC_COLLATE, "");
	setlocale(LC_MONETARY, "");
#else
	setlocale(LC_ALL, "");
	setlocale(LC_CTYPE, "");
	setlocale(LC_MONETARY, "");
	setlocale(LC_NUMERIC, "");
#endif
}


CFontManager::~CFontManager()
{
}

bool CFontManager::AddGlyphSetToFont(vgui2::HFont font, const char * windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
{
#ifdef RUN_GAME_FUNCTIONS
	return Call_Method<bool, CFontManager, vgui2::HFont, const char*, int, int, int, int, int>(0xCFD70, this, font, windowsFontName, tall, weight, blur, scanlines, flags);
#else
	if (m_FontAmalgams.Base()[font].GetCount())
		return false;

	CLinuxFont* winfont = CreateOrFindWin32Font(windowsFontName, tall, weight, blur, scanlines, flags);
	for (const char* fontName = windowsFontName; fontName; fontName = GetFallbackFontName(fontName))
	{
		if (IsFontForeignLanguageCapable(windowsFontName))
		{
			if (winfont)
			{
				m_FontAmalgams.Base()[font].AddFont(winfont, 0, 0xFFFF);
				return true;
			}
		}
		else
		{
			const char* foreginFallbackFont = GetForeignFallbackFontName();
			if (winfont && _strcmpi(foreginFallbackFont, fontName) == 0)
			{
				m_FontAmalgams.Base()[font].AddFont(winfont, 0, 0xFFFF);
				return true;
			}

			CLinuxFont* addedFont = CreateOrFindWin32Font(foreginFallbackFont, tall, weight, blur, scanlines, flags);
			if (addedFont)
			{
				if (winfont)
					m_FontAmalgams.Base()[font].AddFont(winfont, 0, 255);

				m_FontAmalgams.Base()[font].AddFont(addedFont, 256, 0xFFFF);
				return true;
			}
		}
	}

	return false;
#endif
}

void CFontManager::ClearAllFonts()
{
	for (int i = 0; i < m_Win32Fonts.Count(); i++)
	{
		delete m_Win32Fonts.Base()[i];
	}

	m_Win32Fonts.RemoveAll();

	for (int i = 0; i < m_FontAmalgams.Count(); i++)
	{
		m_FontAmalgams.Base()[i].RemoveAll();
	}
}

vgui2::HFont CFontManager::_CreateFont()
{
	return m_FontAmalgams.InsertBefore(m_FontAmalgams.Count());
}

CLinuxFont * CFontManager::CreateOrFindWin32Font(const char * windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
{	
	CLinuxFont* pFont;
	for (int i = 0; i < m_Win32Fonts.Size(); i++)
	{
		pFont  = m_Win32Fonts.Base()[i];
		if (pFont->IsEqualTo(windowsFontName, tall, weight, blur, scanlines, flags))
		{
			return pFont;
		}
	}

	pFont = new CLinuxFont();

	if (pFont->Create(windowsFontName, tall, weight, blur, scanlines, flags))
	{
		// Check from InsertBefore
		m_Win32Fonts.InsertBefore(1, pFont);
		return pFont;
	}

	delete pFont;
	return nullptr;
}

void CFontManager::GetCharABCwide(vgui2::HFont font, int ch, int& a, int& b, int& c)
{
	CLinuxFont* pFont = m_FontAmalgams.Base()[font].GetFontForChar(ch);
	if (pFont)
		pFont->GetCharABCWidths_(ch, a, b, c);
	else
	{
		a = 0;
		b = m_FontAmalgams.Base()[font].GetFontMaxWidth();
		c = 0;
	}
}

int CFontManager::GetCharacterWidth(vgui2::HFont font, int ch_0)
{
	return 0;
}

const char * CFontManager::GetFallbackFontName(const char * windowsFontName)
{
	return nullptr;
}

int CFontManager::GetFontAscent(vgui2::HFont font, wchar_t wch)
{
	CLinuxFont* pFont = GetFontForChar(font, wch);
	if (pFont)
		return pFont->GetAscent();

	return NULL;
}

vgui2::HFont CFontManager::GetFontByName(const char * name)
{
	return vgui2::HFont();
}

CLinuxFont * CFontManager::GetFontForChar(vgui2::HFont font, wchar_t wch)
{
	return nullptr;
}

int CFontManager::GetFontTall(vgui2::HFont font)
{
	return 0;
}

bool CFontManager::GetFontUnderlined(vgui2::HFont font)
{
	return false;
}

bool CFontManager::IsFontForeignLanguageCapable(const char * windowsFontName)
{
	//if (!g_szValidAsianFonts)
	//	return false;

	for (int i = 0; i < ARRAYSIZE(g_szValidAsianFonts); i++)
	{
		if (!_strcmpi(g_szValidAsianFonts[i], windowsFontName))
			return true;
	}

	return false;
}

void CFontManager::SetLanguage(const char * language)
{
}

const char* CFontManager::GetForeignFallbackFontName()
{
	if (USE_DEFAULT_FOREIGN_FALLBACK_FONT)
		return windowsFontName;

	int i = 0;
	for (; i < ARRAYSIZE(ForeignFallbackFonts); i += 2)
	{
		if (strcmp(m_szLanguage, ForeignFallbackFonts[i]) == 0)
			return ForeignFallbackFonts[i + 1];
	}

	return ForeignFallbackFonts[i + 1];
}

void CFontManager::GetTextSize(vgui2::HFont font, const wchar_t * text, int * const wide, int * const tall)
{
}

CFontManager* FontManager()
{
	return &g_FontManager;
}
