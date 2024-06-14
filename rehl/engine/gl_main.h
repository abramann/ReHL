#pragma once

#ifdef SHARED_GAME_DATA
extern int& currenttexture;
extern int* cnttextures;
#else
extern int currenttexture;

extern int cnttextures[2];
#endif