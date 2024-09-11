#include "precompiled.h"
#include "VGui_Controls.h"
#include "IVGui.h"
#include "IPanel.h"
#include "IInput.h"
#include "ISystem.h"
#include "IScheme.h"
#include "ISchemeManager.h"
#include "ILocalize.h"
#include "IKeyValues.h"
#include "FileSystem.h"

IKeyValues* g_pKeyValuesInterface;


void* GetInterface(const char* interface, CreateInterfaceFn * factoryList, int numFactories);

namespace vgui2
{
#ifdef SHARED_GAME_DATA
	IInput* * sp_g_pInputInterface = ADDRESS_OF_DATA(IInput* *, 0xDB3E1);
	IInput* & g_pInputInterface = *sp_g_pInputInterface;

	ISystem* * sp_g_pSystemInterface = ADDRESS_OF_DATA(ISystem* *, 0xDB3D0);
	ISystem* & g_pSystemInterface = *sp_g_pSystemInterface;

	IPanel* * sp_g_pPanelInterface = ADDRESS_OF_DATA(IPanel* *, 0xDB396);
	IPanel* & g_pPanelInterface = *sp_g_pPanelInterface;

	IVGui* * sp_g_pVGuiInterface = ADDRESS_OF_DATA(IVGui* *, 0xDB389);
	IVGui* & g_pVGuiInterface = *sp_g_pVGuiInterface;

	ILocalize* * sp_g_pLocalizeInterface = ADDRESS_OF_DATA(ILocalize* *, 0xDB3F2);
	ILocalize* & g_pLocalizeInterface = *sp_g_pLocalizeInterface;

	ISchemeManager* * sp_g_pSchemeInterface = ADDRESS_OF_DATA(ISchemeManager* *, 0xDB2B1);
	ISchemeManager* & g_pSchemeInterface = *sp_g_pSchemeInterface;

	IFileSystem** sp_g_pFileSystemInterface = ADDRESS_OF_DATA(IFileSystem* *, 0xDB3FF);
	IFileSystem* & g_pFileSystemInterface = *sp_g_pFileSystemInterface;
	
	ISurface* * sp_g_pSurfaceInterface = ADDRESS_OF_DATA(ISurface* *, 0xDB3AC);
	ISurface* & g_pSurfaceInterface = *sp_g_pSurfaceInterface;

#else
	IInput* g_pInputInterface;
	ISystem* g_pSystemInterface;
	IPanel* g_pPanelInterface;
	IVGui* g_pVGuiInterface;
	ILocalize* g_pLocalizeInterface;
	ISchemeManager* g_pSchemeInterface;
	class IFileSystem* g_pFileSystemInterface;
	ISurface* g_pSurfaceInterface; #endif
#endif
	char g_szControlsModuleName[256];

	IPanel *ipanel()
	{
		return g_pPanelInterface;
	}

	IVGui * ivgui()
	{
		return g_pVGuiInterface;
	}

	ISchemeManager *scheme()
	{
		return g_pSchemeInterface;
	}

	ISurface *vgui2::surface()
	{
		return g_pSurfaceInterface;
	}

	IInput* vgui2::input()
	{
		return g_pInputInterface;
	}

	IFileSystem* filesystem()
	{
		return g_pFileSystemInterface;
	}

	ILocalize* localize()
	{
		return g_pLocalizeInterface;
	}

	bool VGuiControls_Init(const char * moduleName, CreateInterfaceFn * factoryList, int numFactories)
	{
		Q_strcpy(g_szControlsModuleName, moduleName);
		vgui2::g_szControlsModuleName[ARRAYSIZE(g_szControlsModuleName) - 1] = 0;

#ifdef _WIN32
#ifndef UNRESOLVED_ISSUE
		setlocale(LC_CTYPE, "");
		setlocale(LC_TIME, "");
		setlocale(LC_COLLATE, "");
		setlocale(LC_MONETARY, "");
#endif
#else
		setlocale(LC_ALL, "");
		setlocale(LC_CTYPE, "");
		setlocale(LC_MONETARY, "");
		setlocale(LC_NUMERIC, "");
#endif
		if (numFactories <= 0)
		{
			g_pVGuiInterface = 0;
			g_pLocalizeInterface = 0;
			g_pFileSystemInterface = 0;
			g_pPanelInterface = 0;
			g_pSurfaceInterface = 0;
			g_pSchemeInterface = 0;
			g_pSystemInterface = 0;
			g_pInputInterface = 0;
			g_pKeyValuesInterface = 0;
			return false;
		}

		g_pVGuiInterface = (IVGui*)GetInterface(VGUI_IVGUI_INTERFACE_VERSION, factoryList, numFactories);
		g_pPanelInterface = (IPanel*)GetInterface(VGUI_PANEL_INTERFACE_VERSION, factoryList, numFactories);
		g_pSurfaceInterface = (ISurface*)GetInterface(VGUI_SURFACE_INTERFACE_VERSION, factoryList, numFactories);
		g_pSchemeInterface = (ISchemeManager*)GetInterface(VGUI_SCHEME_INTERFACE_VERSION, factoryList, numFactories);
		g_pSystemInterface = (ISystem*)GetInterface(VGUI_SYSTEM_INTERFACE_VERSION, factoryList, numFactories);
		g_pInputInterface = (IInput*)GetInterface(VGUI_INPUT_INTERFACE_VERSION, factoryList, numFactories);
		g_pLocalizeInterface = (ILocalize*)GetInterface(VGUI_LOCALIZE_INTERFACE_VERSION, factoryList, numFactories);
		g_pFileSystemInterface = (IFileSystem*)GetInterface(FILESYSTEM_INTERFACE_VERSION, factoryList, numFactories);
		g_pKeyValuesInterface = (IKeyValues*)GetInterface(VGUI_KEYVALUES_INTERFACE_VERSION, factoryList, numFactories);

		if (g_pVGuiInterface)
		{
			g_pVGuiInterface->Init(factoryList, numFactories);
			if (g_pKeyValuesInterface)
			{
				g_pKeyValuesInterface->RegisterSizeofKeyValues(24);
			}

			if (vgui2::g_pSchemeInterface
				&& vgui2::g_pSurfaceInterface
				&& vgui2::g_pSystemInterface
				&& vgui2::g_pInputInterface
				&& vgui2::g_pVGuiInterface
				&& vgui2::g_pFileSystemInterface
				&& vgui2::g_pLocalizeInterface)
			{
				if (g_pKeyValuesInterface && g_pPanelInterface)
					return true;
			}
		}
		return false;
	}

	void vgui2::VGuiControls_Shutdown()
	{
		// Nothing
	}

	const char* GetControlsModuleName()
	{
		return g_szControlsModuleName;
	}
}

void* GetInterface(const char* interface, CreateInterfaceFn * factoryList, int numFactories)
{
	for (int i = 0; i < numFactories; i++)
	{
		if (factoryList[i])
		{
			void* pInterface = (void*)factoryList[i](interface, NULL);
			if (pInterface)
				return pInterface;
		}
	}
	return nullptr;
}
