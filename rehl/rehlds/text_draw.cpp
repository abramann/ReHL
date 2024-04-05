#include "precompiled.h"
#include "text_draw.h"
//#include "vgui2_scheme.h"

static vgui2::HFont _consoleFont = NULL;
static vgui2::HFont _creditsFont = NULL;

using namespace vgui2;

void VGUI2_Draw_Init()
{		
	NOT_IMPLEMENTED;

	/*
	auto pSchemeManager = vgui2::scheme();
	auto defScheme = pSchemeManager->GetDefaultScheme();
	auto pScheme = pSchemeManager->GetIScheme(defScheme);
	if (!_consoleFont)
		_consoleFont = pScheme->GetFont("EngineFont", 1);
	if (!_creditsFont)
		_creditsFont = pScheme->GetFont("Legacy_CreditsFont", 1);
		*/
}

void VGUI2_ResetCurrentTexture()
{
	NOT_IMPLEMENTED;
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
	NOT_IMPLEMENTED;
}

void VGUI2_Draw_SetTextColorAlpha(int r, int g, int b, int a)
{
	NOT_IMPLEMENTED;
}

const wchar_t* VGUI2_Find_String(const char* str)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

int VGUI2_GetFontWide(wchar_t ch, unsigned int font)
{
	NOT_IMPLEMENTED;
	return 0;
}

int VGUI2_GetFontTall(unsigned int font)
{
	NOT_IMPLEMENTED;
	return 0;
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
	return 0;
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
	NOT_IMPLEMENTED;
	return 0;
}
