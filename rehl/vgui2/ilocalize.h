#pragma once


class KeyValues;

#define VGUI_LOCALIZE_INTERFACE_VERSION "VGUI_Localize003"

namespace vgui2
{
	typedef unsigned int StringIndex_t;

	class ILocalize : public IBaseInterface
	{
	public:
		virtual ~ILocalize();

		virtual bool AddFile(IFileSystem *fileSystem, const char *szFileName) = 0;
		virtual void RemoveAll() = 0;
		virtual wchar_t * Find(const char *pName) = 0;
		virtual int ConvertANSIToUnicode(const char *ansi, wchar_t *unicode, int unicodeBufferSizeInBytes) = 0;
		virtual int ConvertUnicodeToANSI(const wchar_t *unicode, char *ansi, int ansiBufferSize) = 0;
		virtual vgui2::StringIndex_t * FindIndex(const char *pName) = 0;
		virtual void ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, wchar_t *formatString, int numFormatParameters, ...) = 0;
		virtual const char * GetNameByIndex(vgui2::StringIndex_t index) = 0;
		virtual wchar_t * GetValueByIndex(vgui2::StringIndex_t index) = 0;
		virtual vgui2::StringIndex_t * GetFirstStringIndex() = 0;
		virtual vgui2::StringIndex_t * GetNextStringIndex(vgui2::StringIndex_t index) = 0;
		virtual void AddString(const char *pString, wchar_t *pValue, const char *fileName) = 0;
		virtual void SetValueByIndex(vgui2::StringIndex_t index, wchar_t *newValue) = 0;
		virtual bool SaveToFile(IFileSystem *fileSystem, const char *szFileName) = 0;
		virtual int GetLocalizationFileCount() = 0;
		virtual const char * GetLocalizationFileName(int index) = 0;
		virtual const char * GetFileNameByIndex(vgui2::StringIndex_t index) = 0;
		virtual void ReloadLocalizationFiles(IFileSystem *filesystem) = 0;
		virtual void ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, const char *tokenName, KeyValues *localizationVariables) = 0;
		virtual void ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, vgui2::StringIndex_t unlocalizedTextSymbol, KeyValues *localizationVariables) = 0;
	};

	ILocalize* localize();
}