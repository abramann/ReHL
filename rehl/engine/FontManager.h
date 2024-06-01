#pragma once

#include "UtlVector.h"
#include "FontAmalgma.h"
#include "vgui\VGUI.h"

class CFontManager
{
public:
	CFontManager();
	~CFontManager();

	bool AddGlyphSetToFont(vgui2::HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags);
	void ClearAllFonts();
	vgui2::HFont _CreateFont();
	CLinuxFont *CreateOrFindWin32Font(const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags);
	void GetCharABCwide(vgui2::HFont font, int ch, int& a, int& b, int& c);
	int GetCharacterWidth(vgui2::HFont font, int ch_0);
	const char * GetFallbackFontName(const char *windowsFontName);
	int GetFontAscent(vgui2::HFont font, wchar_t wch);
	vgui2::HFont GetFontByName(const char *name);
	CLinuxFont *GetFontForChar(vgui2::HFont font, wchar_t wch);
	int GetFontTall(vgui2::HFont font);
	bool GetFontUnderlined(vgui2::HFont font);
	const char * GetForeignFallbackFontName();
	void GetTextSize(vgui2::HFont font, const wchar_t *text, int *const wide, int *const tall);
	bool IsFontForeignLanguageCapable(const char *windowsFontName);
	void SetLanguage(const char *language);

private:
	CUtlVector<CFontAmalgam> m_FontAmalgams;
	CUtlVector<CLinuxFont*> m_Win32Fonts;
	char m_szLanguage[64];
};


CFontManager* FontManager();

