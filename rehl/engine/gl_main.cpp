#include "precompiled.h"

#ifdef SHARED_GAME_DATA
int * sp_currenttexture = ADDRESS_OF_DATA(int *, 0x3C128);
int & currenttexture = *sp_currenttexture;

int* cnttextures = ADDRESS_OF_DATA(int*, 0x3C137);
#else
int currenttexture = -1; // to avoid unnecessary texture sets

int cnttextures[2] = { -1, -1 }; // cached
#endif

model_t* R_LoadMapSprite(const char* szFilename)
{
	NOT_IMPLEMENTED;
	return nullptr;
}