#pragma once
#include "interface.h"


#define GAMECONSOLE_INTERFACE_VERSION "GameConsole003"

class IGameConsole : IBaseInterface
{
public:
	virtual ~IGameConsole();

	virtual void Activate();
	virtual void Initialize();
	virtual void Hide();
	virtual void Clear();
	virtual bool IsConsoleVisible();
	virtual void Printf(const char *format, ...);
	virtual void DPrintf(const char *format, ...);
	virtual void SetParent(int parent);
};

#ifdef SHARED_GAME_DATA
extern IGameConsole*& staticGameConsole;
#else
extern IGameConsole* staticGameConsole;
#endif