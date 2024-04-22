#pragma once


inline uintptr_t makeAddress(uintptr_t offset)
{
	static HMODULE ModuleGameHW = GetModuleHandleA("GameHW.dll");

	if (!ModuleGameHW)
	{
		ModuleGameHW = LoadLibraryA("GameHW.dll");
	}

	return (uintptr_t)offset + (uintptr_t)ModuleGameHW;
}

template<typename ReturnType, typename... Arguments>
__forceinline ReturnType Call_Function(const uintptr_t offset, Arguments... args)
{
	auto address = makeAddress(offset);
	return reinterpret_cast<ReturnType(__cdecl *)(Arguments...)>(address)(args...);
}

template<typename ReturnType, typename ThisType, typename... Arguments>
__forceinline ReturnType Call_Method(const uintptr_t offset, ThisType *const self, Arguments... args)
{
	auto address = makeAddress(offset);
	return reinterpret_cast<ReturnType(__thiscall *)(ThisType *, Arguments...)>(address)(self, args...);
}

