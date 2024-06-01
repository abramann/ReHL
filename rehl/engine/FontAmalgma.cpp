#include "FontAmalgma.h"
#include "precompiled.h"


CFontAmalgam::CFontAmalgam()
{
}

CFontAmalgam::~CFontAmalgam()
{
}

void CFontAmalgam::AddFont(CLinuxFont * font, int lowRange, int highRange)
{
#ifdef USE_GAME_FUNCTIONS
	Call_Method<void, CFontAmalgam, CLinuxFont*, int, int>(0xD2360, this, font, lowRange, highRange);
#else
	CHECK_REQUIRED;
	// Due to CUtlVector different, this function probably won't execute normally

	m_Fonts.InsertBefore(1);

	TFontRange newAdded;
	newAdded.font = font;
	newAdded.lowRange = lowRange;
	newAdded.highRange = highRange;
	m_Fonts.Base()[m_Fonts.Count() - 1] = newAdded;
	
	m_iMaxHeight = max(font->GetHeight(), m_iMaxHeight);
	m_iMaxWidth = max(font->GetMaxCharWidth(), m_iMaxWidth);
#endif
}

int CFontAmalgam::GetCount()
{
	return m_Fonts.Size();
}

int CFontAmalgam::GetFlags(int i)
{
	return m_Fonts.Base()[i].font->GetFlags();
}

CLinuxFont * CFontAmalgam::GetFontForChar(int ch)
{
	int numFonts = m_Fonts.Count();
		if(!numFonts)
			return nullptr;

	for (int i = 0; i < numFonts; i++)
	{
		TFontRange* pTFFont = &m_Fonts.Base()[i];
		if (ch >= pTFFont->lowRange && ch <= pTFFont->highRange)
			if (pTFFont->font->HasChar(ch))
				return pTFFont->font;;
	}

	return nullptr;
}

int CFontAmalgam::GetFontHeight()
{
	if (m_Fonts.Count())
		return m_Fonts.Base()->font->GetHeight();
	else
		return m_iMaxHeight;
}

int CFontAmalgam::GetFontMaxWidth()
{
	return m_iMaxWidth;
}

const char * CFontAmalgam::GetFontName(int i)
{
	return m_Fonts.Base()[i].font->GetFontName();
}

bool CFontAmalgam::GetUnderlined()
{
	if (m_Fonts.Count())
		return m_Fonts.Base()->font->GetUnderlined();
	return false;
}

const char * CFontAmalgam::Name()
{
	return m_szName;
}

void CFontAmalgam::RemoveAll()
{
	m_Fonts.RemoveAll();
}

void CFontAmalgam::SetName(const char * name)
{
	Q_strcpy(m_szName, name);
}
