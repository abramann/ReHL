#include "precompiled.h"
#include "vgui2\VGui_Controls.h"
#include "vgui2\InputInternal.h"
#include "GameUI.h"
#include "GameConsole.h"
#include "CareerUI.h"
#include "VClientVGUI.h"
#include "IHTMLChromeController.h"
#include "vgui2\ILocalize.h"
#include "vgui2\ISchemeManager.h"
#include "vgui2\IVGui.h"
#include "Cursor.h"
#include "EnginePanel.h"


static bool m_bConsoleShowing;

IMouseControl* mousecontrol = nullptr;

cl_enginefunc_t gEngfuncs;

namespace vgui2
{
	IInputInternal* g_InputInternal = nullptr;
}

// gameui
static IGameUI* staticGameUIFuncs = nullptr;
static IGameConsole* staticGameConsole = nullptr;
static ICareerUI* staticCareerUI = nullptr;

static class IClientVGUI* staticClient = nullptr;

// hw
static BaseUISurface* staticSurface = nullptr;
static CStaticPanel* staticPanel = nullptr;

static CBaseUI g_BaseUI;
static CEnginePanel* staticGameUIPanel = nullptr;

CEnginePanel* staticClientDLLPanel;

CBaseUI* g_pBaseUI = nullptr;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CBaseUI, IBaseUI, BASEUI_INTERFACE_VERSION, g_BaseUI);


namespace vgui2
{
	ISurface* g_pSurface = nullptr;
}

CBaseUI::CBaseUI()
{
	m_bConsoleShowing = false;
	g_pBaseUI = &g_BaseUI;
}

CBaseUI::~CBaseUI()
{
	g_pBaseUI = nullptr;
}

void CBaseUI::Initialize(CreateInterfaceFn* factories, int count)
{
	if (staticGameUIFuncs)
		return;

	m_FactoryList[0] = factories[0];
	auto pFSFactory = factories[1];
	m_FactoryList[2] = pFSFactory;

	pFSFactory(FILESYSTEM_INTERFACE_VERSION, nullptr);
	m_hVGuiModule = Sys_LoadModule("vgui2." LIBRARY_PREFIX);
	m_FactoryList[1] = Sys_GetFactory(m_hVGuiModule);
	
#if defined(UNRESOLVED_ISSUE) && defined(_DEBUG)
	m_hChromeModule = reinterpret_cast<CSysModule*>(GetModuleHandleA("chromehtml.dll"));
#else
	m_hChromeModule = Sys_LoadModule("chromehtml." LIBRARY_PREFIX);
#endif

	m_FactoryList[3] = Sys_GetFactory(m_hChromeModule);

	m_iNumFactories = 4;

	char szDllName[512];

	snprintf(szDllName, ARRAYSIZE(szDllName), "valve/cl_dlls/gameui." LIBRARY_PREFIX);
	COM_FixSlashes(szDllName);
	FS_GetLocalCopy(szDllName);
	m_hStaticGameUIModule = Sys_LoadModule(szDllName);

	auto gameUIFactory = Sys_GetFactory(m_hStaticGameUIModule);

	m_FactoryList[m_iNumFactories] = gameUIFactory;

	if (gameUIFactory)
	{
		staticGameUIFuncs = (IGameUI*)(gameUIFactory(GAMEUI_INTERFACE_VERSION, nullptr));
		staticGameConsole = (IGameConsole*)(gameUIFactory(GAMECONSOLE_INTERFACE_VERSION, nullptr));
		staticCareerUI = (ICareerUI*)(gameUIFactory(CAREERUI_INTERFACE_VERSION, nullptr));

		++m_iNumFactories;
	}

	vgui2::g_InputInternal = (vgui2::IInputInternal*)(m_FactoryList[1](VGUI_INPUTINTERNAL_INTERFACE_VERSION, nullptr));

	if (cl_funcs.pClientFactory)
	{
		auto factoryFn = Sys_GetFactory(hClientDLL);

		if (factoryFn)
		{
			m_FactoryList[m_iNumFactories++] = factoryFn;
			staticClient = (IClientVGUI*)(factoryFn(VCLIENTVGUI_INTERFACE_VERSION, nullptr));
		}
	}

	vgui2::VGuiControls_Init("BaseUI", m_FactoryList, m_iNumFactories);
}

void CBaseUI::Start(cl_enginefunc_t* engineFuncs, int interfaceVersion)
{
	memcpy(&gEngfuncs, engineFuncs, sizeof(gEngfuncs));

	staticPanel = (CStaticPanel*)new vgui2::Panel(nullptr, "StaticPanel");
	staticPanel->SetCursor(vgui2::dc_none);
	Color color;
	staticPanel->GetBgColor(color);
	staticPanel->SetBgColor(Color(color.r(), color.g(), color.b(), 0xFF));
	staticPanel->SetBounds(0, 0, 40, 30);
	staticPanel->SetPaintBorderEnabled(false);
	staticPanel->SetPaintBackgroundEnabled(false);
	staticPanel->SetPaintEnabled(false);
	staticPanel->SetCursor(vgui2::dc_none);
	staticPanel->SetVisible(true);
	staticPanel->SetZPos(0);

	staticSurface = (BaseUISurface*)this->m_FactoryList[0](VGUI_SURFACE_INTERFACE_VERSION, 0);
	vgui2::g_pSurface = staticSurface;
//#ifdef UNRESOLVED_ISSUE
//	IHTMLChromeController* pHTMLController = nullptr;
//#else
	IHTMLChromeController* pHTMLController = (IHTMLChromeController *)m_FactoryList[3](CHROMEHTML_INTERFACE_VERSION, 0);
//#endif
	IEngineSurface* pEngineSurface = (IEngineSurface *)m_FactoryList[0](ENGINESURFACE_INTERFACE_VERSION, 0);

	staticSurface->Init(staticPanel->GetVPanel(), pEngineSurface, pHTMLController);
	staticSurface->m_bIgnoreMouseVisCalc = true;

	mousecontrol = staticSurface->getMouseControl();

	// TODO: typo in con printf - Solokiller
	if (!vgui2::scheme()->LoadSchemeFromFile("resource/trackerscheme.res", "BaseUI"))
		gEngfuncs.Con_Printf("Error loading '%s'\n", "resource/trackerScheme.res");

	// Add localization for Half-Life.
	vgui2::localize()->AddFile(vgui2::filesystem(), "resource/valve_%language%.txt");

	// TODO: should be FILENAME_MAX - Solokiller
	char szGameDir[32];

	strncpy(szGameDir, gEngfuncs.pfnGetGameDirectory(), ARRAYSIZE(szGameDir) - 1);
	szGameDir[ARRAYSIZE(szGameDir) - 1] = '\0';

	// Add all fallback localizations.
	for (int i = 0; i < g_fallbackLocalizationFiles.Count(); ++i)
	{
		vgui2::localize()->AddFile(vgui2::filesystem(), g_fallbackLocalizationFiles[i]);

		if (g_fallbackLocalizationFiles[i])
			delete[] g_fallbackLocalizationFiles[i];
	}

	g_fallbackLocalizationFiles.RemoveAll();

	// Add fallback for other games.
	if (memcmp(szGameDir, "valve", 6))
	{
		// TODO: should be FILENAME_MAX - Solokiller
		char szFileName[128];
		snprintf(szFileName, ARRAYSIZE(szFileName) - 1, "resource/%s_%%language%%.txt", szGameDir);
		szFileName[ARRAYSIZE(szFileName) - 1] = '\0';

		vgui2::localize()->AddFile(vgui2::filesystem(), szFileName);
	}

	vgui2::ivgui()->Start();
	vgui2::ivgui()->SetSleep(false);

	staticClientDLLPanel = new CEnginePanel(staticPanel, "BaseClientPanel");

	color = staticClientDLLPanel->GetBgColor(color);

	// Set alpha to maximum.
	staticClientDLLPanel->SetBgColor(Color(color.r(), color.g(), color.b(), 0xFF));

	staticClientDLLPanel->SetPaintBorderEnabled(false);
	staticClientDLLPanel->SetPaintBackgroundEnabled(false);
	staticClientDLLPanel->SetPaintEnabled(false);

	staticClientDLLPanel->SetVisible(true);
	staticClientDLLPanel->SetCursor(vgui2::dc_none);

	// Draw above static panel.
	staticClientDLLPanel->SetZPos(25);

	staticGameUIPanel = new CEnginePanel(staticPanel, "BaseGameUIPanel");
	color = staticGameUIPanel->GetBgColor(color);

	// Set alpha to maximum.
	staticGameUIPanel->SetBgColor(Color(color.r(), color.g(), color.b(), 0xFF));

	staticGameUIPanel->SetPaintBorderEnabled(false);
	staticGameUIPanel->SetPaintBackgroundEnabled(false);
	staticGameUIPanel->SetPaintEnabled(false);

	staticGameUIPanel->SetVisible(true);
	staticGameUIPanel->SetCursor(vgui2::dc_none);

	// Draw above static and client panels.
	staticGameUIPanel->SetZPos(50);

	if (staticGameUIFuncs)
	{
		staticGameUIFuncs->Initialize(m_FactoryList, m_iNumFactories);
	}
	
	if (staticClient)
	{
		NOT_IMPLEMENTED;
		//staticClient->Initialize(m_FactoryList, m_iNumFactories);
		//staticSurface->SetVGUI2MouseControl(true);
	}

	if (staticGameUIFuncs)
	{
		staticGameUIFuncs->Start(&gEngfuncs, CLDLL_INTERFACE_VERSION, &gSystemWrapper);
	}

	staticClientDLLPanel->SetScheme("ClientScheme");

	if (staticClient)
	{
		NOT_IMPLEMENTED;
		//staticClient->Start();
		//staticClient->SetParent(staticClientDLLPanel->GetVPanel());
	}

	int swide, stall;
	vgui2::surface()->GetScreenSize(swide, stall);

	staticPanel->SetBounds(0, 0, swide, stall);
	staticGameUIPanel->SetBounds(0, 0, swide, stall);
	staticClientDLLPanel->SetBounds(0, 0, swide, stall);

	staticGameConsole->Initialize();

	if (staticGameConsole)
	{
		staticGameConsole->SetParent(staticGameUIPanel->GetVPanel());
	}

	staticSurface->m_bIgnoreMouseVisCalc = false;

	staticGameUIFuncs->ActiveGameUI();

	if (COM_CheckParm("-console"))
	{
		ShowConsole();
	}
}

void CBaseUI::Shutdown()
{
	NOT_IMPLEMENTED;
}

int CBaseUI::Key_Event(int down, int keynum, const char* pszCurrentBinding)
{
	NOT_IMPLEMENTED;
	return false;
}

void CBaseUI::CallEngineSurfaceAppHandler(void* pEvent, void* pUserData)
{
}

void CBaseUI::Paint(int x, int y, int right, int bottom)
{
	NOT_IMPLEMENTED;
}

void CBaseUI::HideGameUI()
{
	NOT_IMPLEMENTED;

}

void CBaseUI::ActivateGameUI()
{
	NOT_IMPLEMENTED;

}

void CBaseUI::HideConsole()
{
	NOT_IMPLEMENTED;

	m_bConsoleShowing = 0;
	//	(*(void(__cdecl **)(IGameConsole *))(*(_DWORD *)staticGameConsole + 16))(staticGameConsole);
}

void CBaseUI::ShowConsole()
{
	NOT_IMPLEMENTED;

	m_bConsoleShowing = 1;
	//(*(void(__cdecl **)(IGameConsole *))(*(_DWORD *)staticGameConsole + 8))(staticGameConsole);
}

void VGuiWrap2_ClearConsole()
{
	if (staticGameConsole)
		staticGameConsole->Clear();
}

void VGuiWrap2_ConPrintf(const char * msg)
{
	if (staticGameConsole)
	{
		staticGameConsole->Printf("%s", msg);
		return;
	}

	const size_t uiLength = strlen(msg);

	g_TempConsoleBuffer.InsertMultipleBefore(g_TempConsoleBuffer.Count(), uiLength, msg);
}

void VGuiWrap2_ConDPrintf(const char * msg)
{
	if (staticGameConsole)
	{
		staticGameConsole->DPrintf("%s", msg);
		return;
	}

	const size_t uiLength = strlen(msg);

	g_TempConsoleBuffer.InsertMultipleBefore(g_TempConsoleBuffer.Count(), uiLength, msg);
}

vgui2::VPANEL CStaticPanel::GetCurrentKeyFocus()
{
	return staticClientDLLPanel->GetCurrentKeyFocus();
}

vgui2::VPANEL CStaticPanel::IsWithinTraverse(int x, int y, bool traversePopups)
{
	vgui2::VPANEL panel = BaseClass::IsWithinTraverse(x, y, traversePopups);

	if (panel == GetVPanel())
		panel = NULL_HANDLE;

	return panel;
}