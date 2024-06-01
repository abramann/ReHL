#include "precompiled.h"
#include "text_draw.h"
#include "vgui2\ilocalize.h"
#include "vgui2\ISchemeManager.h"
#include "vgui2\IScheme.h"


static vgui2::HFont _consoleFont = NULL;
static vgui2::HFont _creditsFont = NULL;

using namespace vgui2;

static Color _col(255, 255, 255, 255);

void VGUI2_Draw_Init()
{
	auto pSchemeManager = vgui2::scheme();
	auto defScheme = pSchemeManager->GetDefaultScheme();
	auto pScheme = pSchemeManager->GetIScheme(defScheme);
	if (!_consoleFont)
		_consoleFont = pScheme->GetFont("EngineFont", 1);
	if (!_creditsFont)
		_creditsFont = pScheme->GetFont("Legacy_CreditsFont", 1);
}

void VGUI2_ResetCurrentTexture()
{
	g_BaseUISurface.m_iCurrentTexture = 0;
}

unsigned int VGUI2_GetConsoleFont()
{
	return static_cast<unsigned int>(_consoleFont);
}

unsigned int VGUI2_GetCreditsFont()
{
	return static_cast<unsigned int>(_creditsFont);
}

void VGUI2_Draw_SetTextColor(int r, int g, int b)
{
	_col.SetColor(r, g, b, -1);
}

void VGUI2_Draw_SetTextColorAlpha(int r, int g, int b, int a)
{
	NOT_IMPLEMENTED;
}

const wchar_t* VGUI2_Find_String(const char* str)
{
	static wchar_t tmpString[1024];

	if (str && *str == '#')
	{
		auto pszLocalized = vgui2::localize()->Find(str);
		if (pszLocalized)
			return pszLocalized;
	}

	vgui2::localize()->ConvertANSIToUnicode(str, tmpString,	4096);
	return tmpString;
}

int VGUI2_GetFontWide(wchar_t ch, unsigned int font)
{
	int a, b, c;
	vgui2::surface()->GetCharABCwide(font, ch, a, b, c);

	return c + b + a;
}

int VGUI2_GetFontTall(unsigned int font)
{
	return vgui2::surface()->GetFontTall(font);
}

int VGUI2_Draw_StringLen(const char* psz, unsigned int font)
{
	return VGUI2_Draw_StringLenW(VGUI2_Find_String(psz), font);
}

int VGUI2_Draw_StringLenW(const wchar_t* wsz, unsigned int font)
{
	int len = 0;

	for (size_t i = 0; i < wcslen(wsz); ++i)
	{
		len += VGUI2_GetFontWide(wsz[i], font);
	}

	return len;
}

int VGUI2_DrawString(int x, int y, const char* str, unsigned int font)
{
	NOT_IMPLEMENTED;
	g_BaseUISurface._engineSurface->resetViewPort();

	vgui2::surface()->DrawSetTextFont(font);
	vgui2::surface()->DrawSetTextPos(x, y);
	vgui2::surface()->DrawSetTextColor(_col);


	auto pszString = VGUI2_Find_String(str);

	for (size_t i = 0; i < wcslen(pszString); ++i)
	{
		if (!iswprint(pszString[i]))
		{
			// TODO: this could modify localized strings managed by ILocalize - Solokiller
			const_cast<wchar_t*>(pszString)[i] = L' ';
		}
	}

	vgui2::surface()->DrawPrintText(pszString, wcslen(pszString));
	vgui2::surface()->DrawFlushText();

	return VGUI2_Draw_StringLenW(pszString, font);
}

int VGUI2_DrawStringClient(int x, int y, const char* str, int r, int g, int b)
{
	NOT_IMPLEMENTED;
	return 0;
}

int VGUI2_DrawStringReverseClient(int x, int y, const char* str, int r, int g, int b)
{
	NOT_IMPLEMENTED;
	return 0;
}

int VGUI2_Draw_Character(int x, int y, int ch, unsigned int font)
{
	NOT_IMPLEMENTED;
	return 0;
}

int VGUI2_Draw_CharacterAdd(int x, int y, int ch, int r, int g, int b, unsigned int font)
{
	NOT_IMPLEMENTED;
	return 0;
}

int VGUI2_MessageFontInfo(short* pWidth, unsigned int font)
{
	vgui2::surface()->DrawSetTextFont(font);

	if (pWidth)
	{
		for (int i = 0; i < 256; ++i)
		{
			pWidth[i] = VGUI2_GetFontWide(i, font);
		}
	}

	return VGUI2_GetFontTall(font);
}
