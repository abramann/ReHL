#include "LinuxFont.h"
#include "precompiled.h"

#ifdef _WIN32
#pragma warning(disable:4996) 
#include <sphelper.h>
#include <Windows.h>
#pragma warning(default: 4996)
#endif

#ifdef _MSC_VER 
//not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

bool IntLess(int a, int b)
{
	return a < b;
}

#ifdef WIN32
#ifdef SHARE_GAME_DATA
bool*  p_ms_bIdentifiedOSVersion = (bool*)ADDRESS_OF_DATA(0xD06F3);
bool& ms_bIdentifiedOSVersion = *p_ms_bIdentifiedOSVersion;

bool*  p_ms_bIsWindowsXPorLater = (bool*)ADDRESS_OF_DATA(0xD072F);
bool& ms_bIsWindowsXPorLater = *p_ms_bIsWindowsXPorLater;

OSVERSIONINFOA* p_VerInfo = (OSVERSIONINFOA*)ADDRESS_OF_DATA(0xD0705);

#else
bool CLinuxFont::ms_bIdentifiedOSVersion = false;
bool CLinuxFont::ms_bIsWindowsXPorLater = false;

static OSVERSIONINFOA VerInfo;
#endif
#else
bool CLinuxFont::ms_bSetFriendlyNameCacheLessFunc = false;
#endif

CLinuxFont::CLinuxFont() :m_ExtendedABCWidthsCache(256, 0, (CUtlRBTree<CLinuxFont::abc_cache_t, short unsigned int>::LessFunc_t)ExtendedABCWidthsCacheLessFunc)
{
#ifdef _WIN32
	if (!ms_bIdentifiedOSVersion)
	{
		ms_bIdentifiedOSVersion = true;
#ifdef SHARE_GAME_DATA
		ZeroMemory(p_VerInfo, sizeof(OSVERSIONINFOA));
		GetVersionExA(p_VerInfo);
		ms_bIsWindowsXPorLater = p_VerInfo->dwMajorVersion >= 5;
#else
		ZeroMemory(&VerInfo, sizeof(OSVERSIONINFOA));
		GetVersionExA(&VerInfo);
		ms_bIsWindowsXPorLater = VerInfo.dwMajorVersion >= 5;
#endif
	}
#else
	if (!ms_bSetFriendlyNameCacheLessFunc)
	{
		m_FriendlyNameCache.SetLessFunc((CUtlRBTree<CLinuxFont::font_name_entry, int>::LessFunc_t)IntLess);
		ms_bSetFriendlyNameCacheLessFunc = true;
	}
#endif
}

CLinuxFont::~CLinuxFont()
{
	// Check
}

bool CLinuxFont::ExtendedABCWidthsCacheLessFunc(const abc_cache_t * const lhs, const abc_cache_t * const rhs)
{
	return lhs->wch < rhs->wch;
}

bool CLinuxFont::FindFontDataFile(const char * windowsFontName, int tall, int weight, int blur, int scanlines, int flags, char * dataFile, int dataFileChars)
{
	return false;
}

#ifdef _WIN32
int WINAPI OneTimeFontCreate(const LOGFONTA *, const TEXTMETRICA *, DWORD, LPARAM)
{
	return 0;
}
#endif

bool CLinuxFont::Create(const char * windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
{
#ifdef USE_GAME_FUNCTIONS
	return Call_Method<bool, CLinuxFont, const char *, int, int, int, int, int >(0xD07C0, this, windowsFontName, tall, weight, blur, scanlines, flags);
#else
	Q_strcpy(m_szName, windowsFontName);

	m_bAntiAliased = (flags & 0x10) != 0;
	m_bUnderlined = (flags & 2) != 0;
	m_iTall = tall;
	m_iDropShadowOffset = (flags & 0x80) != 0;
	m_bRotary = (flags & 0x40) != 0;
	m_iWeight = weight;
	m_iBlur = blur;
	m_iFlags = flags;
	m_iScanLines = scanlines;
	m_iHeight = tall;
	
	int iCharSet = (flags >> 2) & 2;
	if (_strcmpi(windowsFontName, "win98japanese") == 0)
	{
		iCharSet = SHIFTJIS_CHARSET;
		Q_strcpy(m_szName, "Tahoma");
	}

	m_hdc = CreateCompatibleDC(0);

	LOGFONTA Logfont;
	Logfont.lfPitchAndFamily = 0;
	Logfont.lfCharSet = DEFAULT_CHARSET;
	Q_strcpy(Logfont.lfFaceName, m_szName);
	
	EnumFontFamiliesExA(m_hdc, &Logfont, OneTimeFontCreate, 0, NULL);

	HFONT hFont = CreateFontA(
		tall, 0, 0, 0,
		m_iWeight,
		flags & 1,
		m_bUnderlined,
		flags & 4, 
		iCharSet,
		0,
		0,
		m_bAntiAliased ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY, 
		0,
		windowsFontName);

	m_hFont = hFont;

	if (!hFont)
		return false;

	SetMapMode(m_hdc, 1);
	SelectObject(m_hdc, (HGDIOBJ)m_hFont);
	SetTextAlign(m_hdc, 1);

	TEXTMETRICA TextMeric;
	GetTextMetricsA(m_hdc, &TextMeric);

	m_bUnderlined = TextMeric.tmUnderlined;
	m_iAscent = TextMeric.tmAscent;
	m_iMaxCharWidth = TextMeric.tmMaxCharWidth;
	m_iHeight = TextMeric.tmHeight;

	m_rgiBitmapSize[0] = m_iMaxCharWidth;
	m_rgiBitmapSize[1] = m_iHeight;

	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFO));

	bmi.bmiHeader.biHeight = -TextMeric.tmHeight;
	bmi.bmiHeader.biWidth = TextMeric.tmMaxCharWidth;
	bmi.bmiColors->rgbBlue = TextMeric.tmHeight;
	bmi.bmiHeader.biSize = 40;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = 0;

	HBITMAP bitmap =  CreateDIBSection(m_hdc, &bmi, 0, (void**)&m_pBuf, NULL, NULL);
	m_hBitmap = bitmap;
	SelectObject(m_hdc, m_hBitmap);

	memset(m_abcWidths, 0, ARRAYSIZE(m_abcWidths) * sizeof(abc_t));

	ABC abcs[128];
	if (GetCharABCWidthsW(m_hdc, 0, 127, abcs)
		|| GetCharABCWidthsA(m_hdc, 0, 127, abcs))
	{
		for (int i = 0; i < 128; i++)
		{
			ABC abc = abcs[i];

			m_abcWidths[i].a = abc.abcA - m_iBlur;
			m_abcWidths[i].b = abc.abcB + m_iDropShadowOffset + 2 * blur;
			m_abcWidths[i].c = abc.abcC - (blur + m_iDropShadowOffset);
		}
	}
	else
	{
		CHECK_REQUIRED;
		// TODO: Find the correct value from values in stack
		static int overhang = 2;

		overhang = overhang == 2 ? 6 : 2;

		for (int i = 0; i < 128; i++)
		{
			m_abcWidths[i].a = overhang;
			m_abcWidths[i].b = overhang;
			m_abcWidths[i].c = overhang;
		}
	}

	if (m_iBlur < 2)
		return true;

	CHECK_REQUIRED;

	m_pGaussianDistribution = new float[1 + 2 * m_iBlur];

	if (m_iBlur & 0x40000000 != 0)
		return true;

	double sigma = m_iBlur * 0.683;
	double a = pow(2, sigma);
	double b = a * 2;
	double c = 1.0 / sqrt(a * 6.28);

	for (int i = 0; i < m_iBlur * 2; i++)
		m_pGaussianDistribution[i] = pow(2.7, -(pow(2, i - m_iBlur) / b)) * c;
#endif
}

void CLinuxFont::ApplyRotaryEffectToTexture(int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, unsigned __int8 * rgba)
{
	// TODO: Implement
}

void CLinuxFont::ApplyScanlineEffectToTexture(int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, unsigned __int8 * rgba)
{	
	// TODO: Implement
}

void CLinuxFont::ApplyDropShadowToTexture(int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, int charWide, int charTall, unsigned __int8 * rgba)
{
	// TODO: Implement
}

void CLinuxFont::ApplyOutlineToTexture(int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, int charWide, int charTall, unsigned __int8 * rgba)
{
	// TODO: Implement
}

void CLinuxFont::ApplyGaussianBlurToTexture(int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, unsigned __int8 * rgba)
{
	// TODO: Implement
}

void CLinuxFont::GetCharRGBA(int ch_0, int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, unsigned __int8 * prgba)
{
	// TODO: Implement
}

bool CLinuxFont::IsEqualTo(const char * windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
{
	if (strcasecmp(windowsFontName, this->m_szName) == 0 &&
		m_iTall == tall &&
		m_iWeight == weight &&
		m_iBlur == blur &&
		m_iFlags == flags)
		return true;
	return false;
}

bool CLinuxFont::IsValid()
{
	return *m_szName != '\0';
}

int CLinuxFont::GetHeight()
{
	return m_iHeight;
}

int CLinuxFont::GetAscent()
{
	return m_iAscent;
}

int CLinuxFont::GetMaxCharWidth()
{
	return m_iMaxCharWidth;
}

const char * CLinuxFont::GetFontName()
{
	return m_szName;
}

bool CLinuxFont::GetUnderlined()
{
	return m_bUnderlined;
}

int CLinuxFont::GetFlags()
{
	return m_iFlags;
}

void * CLinuxFont::SetAsActiveFont(GLXContext cglContext)
{
#ifdef _WIN32
	// Check in Windows
#else
	// Nothing
#endif
	return nullptr;
}

bool CLinuxFont::HasChar(wchar_t wch)
{
#ifdef _WIN32
	// Check in Windows probably nothing
	return true;
#else
	return FT_Get_Char_Index(this->face, wch) != 0
#endif
}

void CLinuxFont::GetCharABCWidths_(int ch, int& a, int& b, int& c)
{
#ifdef USE_GAME_FUNCTIONS
	Call_Method<void, CLinuxFont, int, int&, int&, int&>(0xD15A0, this, ch, a, b, c);
#else
	if (ch < 128)
	{
		a = m_abcWidths[ch].a;
		b = m_abcWidths[ch].b;
		c = m_abcWidths[ch].c;
	}
	else
	{
		CLinuxFont::abc_cache_t finder;
		memset(&finder.abc, 0, 4);
		finder.wch = ch;
		int i = m_ExtendedABCWidthsCache.Find(finder);
		if (i)
		{
			ABC abc;
			if (GetCharABCWidthsW(m_hdc, ch, ch, &abc))
			{
				finder.abc.a = abc.abcA;
				finder.abc.b = abc.abcB;
				finder.abc.c = abc.abcC;
			}
			else
			{
				CHECK_REQUIRED;
				// This part never executed, so it have high chance wrong implement 

				char multiByteStr[6];
				SIZE size;
				WideCharToMultiByte(0, 0, (LPCWCH)&ch, 1, multiByteStr, 6, 0, 0);
				if (GetTextExtentPoint32A(m_hdc, multiByteStr, strlen(multiByteStr), &size))
					finder.abc.b = size.cx;
				else
					finder.abc.b = m_iMaxCharWidth;;

				finder.abc.a = 0;
				finder.abc.c = 0;
			}
		}
		m_ExtendedABCWidthsCache.Insert(finder);
	}
#endif
}
