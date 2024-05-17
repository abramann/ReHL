#pragma once

#include "interface.h"

class IBaseUI : public IBaseInterface
{
public:

	virtual ~IBaseUI() {};

	virtual void Initialize(CreateInterfaceFn* factories, int count) = 0;

	virtual void Start(cl_enginefunc_t* engineFuncs, int interfaceVersion) = 0;

	virtual void Shutdown() = 0;

	virtual int Key_Event(int down, int keynum, const char* pszCurrentBinding) = 0;

	virtual void CallEngineSurfaceAppHandler(void* pEvent, void* pUserData) = 0;

	virtual void Paint(int x, int y, int right, int bottom) = 0;

	virtual void HideGameUI() = 0;

	virtual void ActivateGameUI() = 0;

	virtual void HideConsole() = 0;

	virtual void ShowConsole() = 0;
};

class CBaseUI : public IBaseUI
{
private:
	enum
	{
		MAX_NUM_FACTORIES = 6,
	};

public:
	CBaseUI();
	~CBaseUI();

	void Initialize(CreateInterfaceFn* factories, int count) override;

	void Start(cl_enginefunc_t* engineFuncs, int interfaceVersion) override;

	void Shutdown() override;

	int Key_Event(int down, int keynum, const char* pszCurrentBinding) override;

	void CallEngineSurfaceAppHandler(void* pEvent, void* pUserData) override;

	void Paint(int x, int y, int right, int bottom) override;

	void HideGameUI() override;

	void ActivateGameUI() override;

	void HideConsole() override;

	void ShowConsole() override;

private:
	CreateInterfaceFn m_FactoryList[MAX_NUM_FACTORIES] = {};
	int m_iNumFactories = 0;

	CSysModule* m_hVGuiModule = nullptr;
	CSysModule* m_hStaticGameUIModule = nullptr;
	CSysModule* m_hChromeModule = nullptr;
	bool m_bHidingGameUI = false;

private:
	CBaseUI(const CBaseUI&) = delete;
	CBaseUI& operator=(const CBaseUI&) = delete;
};

#define BASEUI_INTERFACE_VERSION "BaseUI001"