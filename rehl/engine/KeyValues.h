#pragma once

#include "interface.h"

class IFileSystem;

class KeyValues : public IBaseInterface
{
public:
	enum types_t
	{
		TYPE_NONE ,
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_PTR,
		TYPE_WSTRING,
		TYPE_COLOR,
		TYPE_UINT64,
	};

	KeyValues(const char *SetName);
	KeyValues(const char *SetName, const char *firstKey, const char *firstValue);;
	KeyValues(const char *SetName, const char *firstKey, const char *firstValue, const char *secondKey, const char *secondValue);
	KeyValues(const char *SetName, const char *firstKey, int firstValue);
	KeyValues(const char *SetName, const char *firstKey, int firstValue, const char *secondKey, int secondValue);
	KeyValues(const char *SetName, const char *firstKey, const wchar_t *firstValue);


	virtual ~KeyValues();

	virtual const char *GetName() const;
	virtual int GetNameSymbol() const;
	virtual bool LoadFromFile(IFileSystem *filesystem, const char *resourceName, const char *pathID);
	virtual bool SaveToFile(IFileSystem *filesystem, const char *resourceName, const char *pathID);
	virtual KeyValues *FindKey(const char *keyName, bool bCreate) const;
	virtual KeyValues *FindKey(const int keySymbol) const;
	virtual KeyValues *CreateNewKey();
	virtual void RemoveSubKey(KeyValues *subKey);
	virtual KeyValues *GetFirstSubKey();
	virtual KeyValues *GetNextKey();
	virtual int GetInt(const char *keyName, int defaultValue);
	virtual float GetFloat(const char *keyName, float defaultValue);
	virtual const char *GetString(const char *keyName, const char *defaultValue);
	virtual const wchar_t *GetWString(const char *keyName, const wchar_t *defaultValue);
	virtual void *GetPtr(const char *keyName, void *defaultValue);
	virtual bool IsEmpty(const char *keyName);
	virtual void SetWString(const char *keyName, const wchar_t *value);
	virtual void SetString(const char *keyName, const char *value);
	virtual void SetInt(const char *keyName, int value);
	virtual void SetFloat(const char *keyName, float value);
	virtual void SetPtr(const char *keyName, void *value);
	virtual KeyValues *MakeCopy() const;
	virtual void Clear();
	virtual types_t GetDataType(const char *keyName);
	virtual void deleteThis();

private:
	int m_iKeyName;
	//KeyValues::$63A2C287A639B69FE1AD2E033DBC1C1C _anon_0;
	unsigned __int16 m_iDataType;
	int16 m_iAllocationSize;
	KeyValues *m_pPeer;
	KeyValues *m_pSub;
};