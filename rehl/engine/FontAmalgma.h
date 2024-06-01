#pragma once

#include "UtlVector.h"
#include "LinuxFont.h"

class CFontAmalgam
{
public:
	CFontAmalgam();
	~CFontAmalgam();

	void AddFont(CLinuxFont *font, int lowRange, int highRange);
	int GetCount();
	int GetFlags(int i);
	CLinuxFont *GetFontForChar(int ch);
	int GetFontHeight();
	int GetFontMaxWidth();
	const char *GetFontName(int i);
	bool GetUnderlined();
	const char *Name();
	void RemoveAll();
	void SetName(const char *name);

	struct TFontRange
	{
		int lowRange;
		int highRange;
		CLinuxFont *font;
	};

private:

	CUtlVector<CFontAmalgam::TFontRange> m_Fonts;
	char m_szName[32];
	int m_iMaxWidth;
	int m_iMaxHeight;
};
