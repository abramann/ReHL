#include "precompiled.h"
#include "FontManager.h"

CFontManager* FontManager()
{
	return Call_Function<CFontManager*>(0xCFA00);
}

CFontManager::CFontManager()
{
}

CFontManager::~CFontManager()
{
}

bool CFontManager::AddGlyphSetToFont(vgui2::HFont font, const char * windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
{
	//NOT_IMPLEMENTED_IGNORE;
	return Call_Method<bool, CFontManager, vgui2::HFont, const char*, int, int, int, int, int>(0xCFD70, this,
		font, windowsFontName, tall, weight, blur, scanlines, flags);
}

void CFontManager::ClearAllFonts()
{
	NOT_IMPLEMENTED;

}

vgui2::HFont CFontManager::CreateFont()
{
	//NOT_IMPLEMENTED_IGNORE;
	return Call_Method<vgui2::HFont, CFontManager>(0xCFCA0, this);
}

CLinuxFont * CFontManager::CreateOrFindWin32Font(const char * windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void CFontManager::GetCharABCwide(vgui2::HFont font, int ch_0, int& a, int& b, int& c)
{
	//NOT_IMPLEMENTED_IGNORE;
	Call_Method<void, CFontManager, vgui2::HFont, int, int&, int&, int&>(0xD0090,this, font, ch_0, a, b, c);
}

int CFontManager::GetCharacterWidth(vgui2::HFont font, int ch_0)
{
	NOT_IMPLEMENTED;

	return 0;
}

const char * CFontManager::GetFallbackFontName(const char * windowsFontName)
{
	NOT_IMPLEMENTED;

	return nullptr;
}

int CFontManager::GetFontAscent(vgui2::HFont font, wchar_t wch)
{
	NOT_IMPLEMENTED;

	return 0;
}

vgui2::HFont CFontManager::GetFontByName(const char * name)
{
	NOT_IMPLEMENTED;

	return vgui2::HFont();
}

CLinuxFont * CFontManager::GetFontForChar(vgui2::HFont font, wchar_t wch)
{
	NOT_IMPLEMENTED;
	return 0;
}

int CFontManager::GetFontTall(vgui2::HFont font)
{
	//NOT_IMPLEMENTED_IGNORE;
	return Call_Method<int, CFontManager, vgui2::HFont>(0xD0100,this, font);
}

bool CFontManager::GetFontUnderlined(vgui2::HFont font)
{
	NOT_IMPLEMENTED;

	return false;
}

const char * CFontManager::GetForeignFallbackFontName()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void CFontManager::GetTextSize(vgui2::HFont font, const wchar_t * text, int& wide, int& tall)
{
	NOT_IMPLEMENTED;

}

bool CFontManager::IsFontForeignLanguageCapable(const char * windowsFontName)
{
	NOT_IMPLEMENTED;

	return false;
}

void CFontManager::SetLanguage(const char * language)
{
	NOT_IMPLEMENTED;

}
