#pragma once
#include <cstdint>

void SetupHooks();
uintptr_t AddBase(uintptr_t offset);

#define ADDRESS_OF_DATA(TYPE, ADDR) (TYPE)*(DWORD*)AddBase(ADDR)
#define ADDRESS_OF_DATAPTR(TYPE, ADDR) (TYPE)**(DWORD**)AddBase(ADDR)

template<typename T>
void DirectHook(uintptr_t offset, T directedFunction);

template<typename ReturnType, typename... Arguments>
__forceinline ReturnType Call_Function(const uintptr_t offset, Arguments... args)
{
	auto address = AddBase(offset);
	return reinterpret_cast<ReturnType(__cdecl *)(Arguments...)>(address)(args...);
}

template<typename ReturnType, typename ThisType, typename... Arguments>
__forceinline ReturnType Call_Method(const uintptr_t offset, ThisType *const self, Arguments... args)
{
	auto address = AddBase(offset);
	return reinterpret_cast<ReturnType(__thiscall *)(ThisType *, Arguments...)>(address)(self, args...);
}