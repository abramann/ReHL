#pragma once
#include <cstdint>

void SetupHooks();
uintptr_t AddBase(uintptr_t offset);

#ifdef SHARED_GAME_DATA

#define ADDRESS_OF_DATA(TYPE, ADDR) (TYPE)*(DWORD*)AddBase(ADDR)
#define VAR(TYPE, NAME, ADDR) TYPE* sp_##NAME = ADDRESS_OF_DATA(TYPE*, ADDR);\
									 TYPE& NAME = *sp_##NAME;
#define SVAR(TYPE, NAME, ADDR) VAR(TYPE, NAME, ADDR)
#define VVAR(TYPE, NAME, ADDR, VALUE) VAR(TYPE, NAME, ADDR)
#define SVVAR(TYPE, NAME, ADDR, VALUE) VVAR(TYPE, NAME, ADDR, VALUE)
#define EXTERN_VAR(TYPE, NAME) extern TYPE& NAME;
#define EXTERN_ARRAY(TYPE, NAME, DIM) extern TYPE(&NAME)DIM;

#define ARRAY(TYPE, NAME, DIM, ADDR) TYPE(*sp_##NAME)DIM = ADDRESS_OF_DATA(TYPE(*)DIM, ADDR);\
										TYPE(&##NAME)DIM = *sp_##NAME;
#define SARRAY(TYPE, NAME, DIM, ADDR) ARRAY(TYPE, NAME, DIM, ADDR)
#define VARRAY(TYPE, NAME, DIM, ADDR, VALUE) ARRAY(TYPE, NAME, DIM, ADDR);

#else
#define VAR(TYPE, NAME, ADDR) TYPE NAME;
#define SVAR(TYPE, NAME, ADDR) VAR(TYPE, NAME, ADDR) static VAR(TYPE, NAME, ADDR)
#define VVAR(TYPE, NAME, ADDR, VALUE) TYPE NAME = VALUE;
#define SVVAR(TYPE, NAME, ADDR, VALUE) static VVAR(TYPE, NAME, ADDR, VALUE)
#define EXTERN_VAR(TYPE, NAME) extern TYPE NAME;
#define ARRAY(TYPE, NAME, DIM, ADDR) TYPE NAME##DIM;
#define SARRAY(TYPE, NAME, DIM, ADDR) static ARRAY(TYPE, NAME, DIM, ADDR)
#define VARRAY(TYPE, NAME, DIM, ADDR, VALUE) TYPE NAME##DIM = VALUE;
#define EXTERN_ARRAY(TYPE, NAME, DIM) extern TYPE NAME##DIM;
#endif

#define COMMA ,

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