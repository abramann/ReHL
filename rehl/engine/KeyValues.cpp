#include "precompiled.h"
#include "KeyValues.h"

KeyValues::KeyValues(const char * SetName)
{
	NOT_IMPLEMENTED;
}

KeyValues::KeyValues(const char * SetName, const char * firstKey, const char * firstValue)
{
	NOT_IMPLEMENTED;

}

KeyValues::KeyValues(const char * SetName, const char * firstKey, const char * firstValue, const char * secondKey, const char * secondValue)
{
	NOT_IMPLEMENTED;

}

KeyValues::KeyValues(const char * SetName, const char * firstKey, int firstValue)
{
	NOT_IMPLEMENTED;

}

KeyValues::KeyValues(const char * SetName, const char * firstKey, int firstValue, const char * secondKey, int secondValue)
{
	NOT_IMPLEMENTED;

}

KeyValues::KeyValues(const char * SetName, const char * firstKey, const wchar_t * firstValue)
{
	NOT_IMPLEMENTED;

}

KeyValues::~KeyValues()
{
	NOT_IMPLEMENTED;

}

const char * KeyValues::GetName() const
{
	NOT_IMPLEMENTED;

	return nullptr;
}

int KeyValues::GetNameSymbol() const
{
	NOT_IMPLEMENTED;

	return 0;
}

bool KeyValues::LoadFromFile(IFileSystem * filesystem, const char * resourceName, const char * pathID)
{
	NOT_IMPLEMENTED;

	return false;
}

bool KeyValues::SaveToFile(IFileSystem * filesystem, const char * resourceName, const char * pathID)
{
	NOT_IMPLEMENTED;

	return false;
}

KeyValues * KeyValues::FindKey(const char * keyName, bool bCreate) const
{
	NOT_IMPLEMENTED;

	return nullptr;
}

KeyValues * KeyValues::FindKey(const int keySymbol) const
{
	NOT_IMPLEMENTED;

	return nullptr;
}

KeyValues * KeyValues::CreateNewKey()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void KeyValues::RemoveSubKey(KeyValues * subKey)
{
	NOT_IMPLEMENTED;

}

KeyValues * KeyValues::GetFirstSubKey()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

KeyValues * KeyValues::GetNextKey()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

int KeyValues::GetInt(const char * keyName, int defaultValue)
{
	NOT_IMPLEMENTED;

	return 0;
}

float KeyValues::GetFloat(const char * keyName, float defaultValue)
{
	NOT_IMPLEMENTED;

	return 0.0f;
}

const char * KeyValues::GetString(const char * keyName, const char * defaultValue)
{
	NOT_IMPLEMENTED;

	return nullptr;
}

const wchar_t * KeyValues::GetWString(const char * keyName, const wchar_t * defaultValue)
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void * KeyValues::GetPtr(const char * keyName, void * defaultValue)
{
	NOT_IMPLEMENTED;

	return nullptr;
}

bool KeyValues::IsEmpty(const char * keyName)
{
	NOT_IMPLEMENTED;

	return false;
}

void KeyValues::SetWString(const char * keyName, const wchar_t * value)
{
	NOT_IMPLEMENTED;

}

void KeyValues::SetString(const char * keyName, const char * value)
{
	NOT_IMPLEMENTED;

}

void KeyValues::SetInt(const char * keyName, int value)
{
	NOT_IMPLEMENTED;

}

void KeyValues::SetFloat(const char * keyName, float value)
{
	NOT_IMPLEMENTED;

}

void KeyValues::SetPtr(const char * keyName, void * value)
{
	NOT_IMPLEMENTED;

}

KeyValues * KeyValues::MakeCopy() const
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void KeyValues::Clear()
{
	NOT_IMPLEMENTED;
}


KeyValues::types_t KeyValues::GetDataType(const char * keyName)
{
	NOT_IMPLEMENTED;

	return types_t();
}

void KeyValues::deleteThis()
{
	NOT_IMPLEMENTED;

}
