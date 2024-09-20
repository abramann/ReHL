#include "precompiled.h"
#include "../../../dep/FakeVGUI/App.h"
#include "../../../dep/FakeVGUI/Panel.h"
#include "../../../dep/FakeVGUI/SurfaceBase.h"
#include "GameUI.h"
#include "VClientVGUI.h"
#include "GameConsole.h"

#define BMP_TYPE 0x4D42

#ifdef _WIN32
#include <Windows.h>
#else
typedef struct tagBITMAPFILEHEADER
{
	uint16 bfType;
	uint32 bfSize;
	uint16 bfReserved1;
	uint16 bfReserved2;
	uint32 bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
	uint32 biSize;
	int32 biWidth;
	int32 biHeight;
	uint16 biPlanes;
	uint16 biBitCount;
	uint32 biCompression;
	uint32 biSizeImage;
	int32 biXPelsPerMeter;
	int32 biYPelsPerMeter;
	uint32 biClrUsed;
	uint32 biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQuad
{
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQuad;

struct BITMAPINFO
{
	BITMAPINFOHEADER bmiHeader;
	RGBQuad bmiColors[1];
};
#endif

class CDummyApp : public vgui::App
{
public:
	void main(int argc, char* argv[]) override;
	CDummyApp() : App()
	{

	}
	//~CDummyApp() = delete;
protected:
	void platTick() override;

	char baseclass[556];
};

void CDummyApp::main(int argc, char* argv[])
{
	// Nothing
}

void CDummyApp::platTick()
{
	// Nothing
}

// App for VGUI programs, globally accessed through vgui::App::getInstance().
VAR(CDummyApp, theApp, 0xC1321);

#ifdef SHARED_GAME_DATA
IBaseUI** sp_staticUIFuncs = ADDRESS_OF_DATA(IBaseUI**, 0x70D7);
IBaseUI*& staticUIFuncs = *sp_staticUIFuncs;

EngineSurfaceWrap** sp_staticEngineSurface = ADDRESS_OF_DATA(EngineSurfaceWrap**, 0xC1341);
static EngineSurfaceWrap*& staticEngineSurface = *sp_staticEngineSurface;

CUtlVector<char> * sp_g_TempConsoleBuffer = ADDRESS_OF_DATA(CUtlVector<char> *, 0x7520);
CUtlVector<char> & g_TempConsoleBuffer = *sp_g_TempConsoleBuffer; 

vgui::Panel* * sp_staticPanel = ADDRESS_OF_DATA(vgui::Panel* *, 0xC138C);
vgui::Panel* & staticPanel = *sp_staticPanel;
#else
static IBaseUI* staticUIFuncs = nullptr;
static EngineSurfaceWrap* staticEngineSurface = nullptr;
CUtlVector<char> g_TempConsoleBuffer;
static vgui::Panel* staticPanel = nullptr;
#endif
extern bool scr_drawloading;

qboolean staticExclusiveInputShadow = false;
qboolean isFogEnabled = false;

void VGui_Startup()
{
	VGuiWrap_Startup();
	VGuiWrap2_Startup();
}

void VGui_Shutdown()
{
	VGuiWrap_Shutdown();
	VGuiWrap2_Shutdown();
	EngineSurface::freeEngineSurface();
}

void VGui_CallEngineSurfaceAppHandler(void* event, void* userData)
{
	if (!VGuiWrap2_CallEngineSurfaceAppHandler(event, userData))
		VGuiWrap_CallEngineSurfaceAppHandler(event, userData);
}

vgui::Panel* VGui_GetPanel()
{
	return VGuiWrap_GetPanel();
}

void VGui_ReleaseMouse()
{
	VGuiWrap_ReleaseMouse();
	VGuiWrap2_ReleaseMouse();
}

void VGui_GetMouse()
{
	VGuiWrap_GetMouse();
	VGuiWrap2_GetMouse();
}

void VGui_SetVisible(bool state)
{
	VGuiWrap_SetVisible(state);
	VGuiWrap2_SetVisible(state);
}

void VGui_Paint()
{
	if (VGuiWrap2_UseVGUI1())
	{
		VGuiWrap_Paint(!VGuiWrap2_IsGameUIVisible());
		VGuiWrap2_Paint();
	}
	else
	{
		VGuiWrap_Paint(false);
		VGuiWrap2_Paint();
	}
}

bool VGui_GameUIKeyPressed()
{
	return VGuiWrap2_GameUIKeyPressed();
}

bool VGui_Key_Event(int down, int keynum, const char* pszCurrentBinding)
{
	return VGuiWrap2_Key_Event(down, keynum, pszCurrentBinding) != 0;
}

bool VGui_LoadBMP(FileHandle_t file, byte* buffer, int bufsize, int* width, int* height)
{
	const auto size = FS_Size(file);

	BITMAPFILEHEADER bmfHeader;

	FS_Read(&bmfHeader, sizeof(BITMAPFILEHEADER), 1, file);

	bool bSuccess = false;

	if (bmfHeader.bfType == BMP_TYPE)
	{
		const auto dataSize = size - sizeof(BITMAPFILEHEADER);

		auto pBuffer = reinterpret_cast<byte*>(malloc(dataSize));

		FS_Read(pBuffer, dataSize, 1, file);

		auto pInfo = reinterpret_cast<BITMAPINFO*>(pBuffer);

		*width = pInfo->bmiHeader.biWidth;
		*height = pInfo->bmiHeader.biHeight;

		int iWidth = *width;

		if (*width & 3)
			iWidth = *width + (*width + 4) % 4;

		auto pPalette = pInfo->bmiColors;

		auto pSource = reinterpret_cast<byte*>(pInfo) + bmfHeader.bfOffBits - sizeof(bmfHeader);

		auto pDest = buffer;

		// Convert into an RGBA format.
		for (int y = 0; y < *height; ++y)
		{
			for (int x = 0; x < *width; ++x, pDest += 4)
			{
				auto pPixels = &pSource[x + iWidth * (*height - y - 1)];

				pDest[0] = pPalette[*pPixels].rgbRed;
				pDest[1] = pPalette[*pPixels].rgbGreen;
				pDest[2] = pPalette[*pPixels].rgbBlue;
				pDest[3] = 0xFF;
			}
		}

		free(pBuffer);

		bSuccess = true;
	}

	FS_Close(file);

	return bSuccess;
}

void VGuiWrap_SetRootPanelSize()
{
	return Call_Function<void>(0xC1270);
	auto pRoot = VGuiWrap_GetPanel();

	if (pRoot)
	{
		int x = 0, y = 0;
		rect_t rect;

		rect.left = 0;

		if (VideoMode_IsWindowed())
		{
			SDL_GetWindowPosition(*pmainwindow, &x, &y);
			SDL_GetWindowSize(*pmainwindow, &rect.right, &rect.bottom);
		}
		else
		{
			VideoMode_GetCurrentVideoMode(&rect.right, &rect.bottom, nullptr);
		}

		rect.bottom += rect.top;

		pRoot->setBounds(x, y, rect.right, rect.bottom);
	}
}

void VGuiWrap_Startup()
{
	if (staticEngineSurface)
		return;

	auto pApp = vgui::App::getInstance();
	pApp->reset();

	staticPanel = new vgui::Panel(0, 0, 320, 240);

	auto pScheme = pApp->getScheme();

	staticPanel->setPaintBorderEnabled(false);
	staticPanel->setPaintBackgroundEnabled(false);
	staticPanel->setPaintEnabled(false);
	staticPanel->setCursor(pScheme->getCursor(vgui::Scheme::scu_none));

	auto factoryFn = Sys_GetFactoryThis();

	IEngineSurface* pSurface = (IEngineSurface*)factoryFn(ENGINESURFACE_INTERFACE_VERSION, nullptr);

	staticEngineSurface = new EngineSurfaceWrap(staticPanel, pSurface);

	VGuiWrap_SetRootPanelSize();
}

void VGuiWrap_Shutdown()
{
	NOT_IMPLEMENTED;
}

bool VGuiWrap_CallEngineSurfaceAppHandler(void * event, void * userData)
{
	if (staticEngineSurface)
		staticEngineSurface->AppHandler(event, userData);

	return false;
}

vgui::Panel * VGuiWrap_GetPanel()
{
	g_engdstAddrs.VGui_GetPanel();
	return staticPanel;
}

void VGuiWrap_ReleaseMouse()
{
	NOT_IMPLEMENTED;

}

void VGuiWrap_GetMouse()
{
	NOT_IMPLEMENTED;

}

void VGuiWrap_SetVisible(bool state)
{
	NOT_IMPLEMENTED;
}

void VGuiWrap_Paint(bool paintAll)
{
	g_engdstAddrs.VGui_GetPanel();

	if (!staticPanel)
		return;

	VGuiWrap_SetRootPanelSize();
	staticPanel->repaint();
	auto pApp = vgui::App::getInstance();
	pApp->externalTick();
	if (paintAll)
		staticPanel->paintTraverse();
	else
	{
		int extents[4];
		staticPanel->getAbsExtents(
			extents[0],
			extents[1],
			extents[2],
			extents[3]);
		VGui_ViewportPaintBackground(extents);
	}
}

void VGuiWrap2_Startup()
{
	if (staticUIFuncs)
		return;

	auto factoryFn = Sys_GetFactoryThis();

	CreateInterfaceFn factories[2] =
	{
		factoryFn,
		(CreateInterfaceFn)GetFileSystemFactory() };

	staticUIFuncs = reinterpret_cast<IBaseUI*>(factoryFn(BASEUI_INTERFACE_VERSION, nullptr));

	staticUIFuncs->Initialize(factories, ARRAYSIZE(factories));
	staticUIFuncs->Start(&cl_enginefuncs, CLDLL_INTERFACE_VERSION);

	// Flush temporary buffer
	g_TempConsoleBuffer.AddToTail('\0');
	VGuiWrap2_ConPrintf(g_TempConsoleBuffer.Base());
	g_TempConsoleBuffer.Purge();
}

void VGuiWrap2_Shutdown()
{
	NOT_IMPLEMENTED;
}

bool VGuiWrap2_CallEngineSurfaceAppHandler(void * event, void * userData)
{
	if (staticUIFuncs)
		staticUIFuncs->CallEngineSurfaceAppHandler(event, userData);
	return false;
}

bool VGuiWrap2_IsGameUIVisible()
{
	if (staticGameUIFuncs)
		return staticGameUIFuncs->IsGameUIActive();
	return false;
}

bool VGuiWrap2_UseVGUI1()
{
	if (staticClient)
		NOT_IMPLEMENTED;
		//	return (*(unsigned __int8(__cdecl **)(IClientVGUI *))(*(_DWORD *)staticClient + 20))(staticClient);
	return true;
}

vgui::Panel * VGuiWrap2_GetPanel()
{
	NOT_IMPLEMENTED;
	return nullptr;
}

void VGuiWrap2_ReleaseMouse()
{
	NOT_IMPLEMENTED;
}

void VGuiWrap2_GetMouse()
{
	NOT_IMPLEMENTED;
}

void VGuiWrap2_SetVisible(bool state)
{
	NOT_IMPLEMENTED;
}

bool VGuiWrap2_GameUIKeyPressed()
{
	NOT_IMPLEMENTED;
	return false;
}

bool VGuiWrap2_Key_Event(int down, int keynum, const char * pszCurrentBinding)
{
	if (staticUIFuncs)
		return staticUIFuncs->Key_Event(down, keynum, pszCurrentBinding) == 0;
	return true;
}

void VGuiWrap2_Paint()
{
	if (!staticGameUIFuncs)
		return;

	RECT rect;
	POINT pnt;

	pnt.x = 0;
	pnt.y = 0;
	rect.top = 0;
	if (VideoMode_IsWindowed())
	{
		SDL_GetWindowPosition(*pmainwindow,(int*) &pnt.x, (int*)&pnt.y);
		SDL_GetWindowSize(*pmainwindow, (int*)&rect.right, (int*)&rect.bottom);
	}
	else
	{
		pnt.y = 0;
		pnt.x = 0;
		VideoMode_GetCurrentVideoMode((int*)&rect.right, (int*)&rect.bottom, 0);
	}

	rect.bottom += rect.top;
	AllowFog(false);

	staticUIFuncs->Paint(pnt.x, pnt.y, rect.right, rect.bottom);

	if (!staticClient)//|| (*(unsigned __int8(__cdecl **)(IClientVGUI *))(*(_DWORD *)staticClient + 20))(staticClient))
	{
		qboolean exculsiveInput = staticGameUIFuncs->HasExclusiveInput();
		if (exculsiveInput != staticExclusiveInputShadow)
		{
			if (exculsiveInput)
				VGuiWrap_ReleaseMouse();
			else
			{
				VGuiWrap_GetMouse();
				ClearIOStates();
			}
		}
		staticExclusiveInputShadow = exculsiveInput;
	}
	AllowFog(true);
}

void VGuiWrap2_NotifyOfServerDisconnect()
{
	NOT_IMPLEMENTED;
}

void VGuiWrap2_HideGameUI()
{
	NOT_IMPLEMENTED;
}

bool VGuiWrap2_IsConsoleVisible()
{
	if (staticGameConsole)
		return staticGameConsole->IsConsoleVisible();

	return false;
}

void VGuiWrap2_ShowConsole()
{
	if (staticUIFuncs)
	{
		staticUIFuncs->ActivateGameUI();
		staticUIFuncs->ShowConsole();
	}
}

void VGuiWrap2_ShowDemoPlayer()
{
	NOT_IMPLEMENTED;
}

void VGuiWrap2_HideConsole()
{
	if (staticUIFuncs)
		staticUIFuncs->HideConsole();
}

void VGuiWrap2_LoadingStarted(const char * resourceType, const char * resourceName)
{             
	NOT_IMPLEMENTED;
}

void VGuiWrap2_LoadingFinished(const char * resourceType, const char * resourceName)
{
	NOT_IMPLEMENTED;
}

void VGuiWrap2_NotifyOfServerConnect(const char * game, int IP, int port)
{
	NOT_IMPLEMENTED;
}

CareerStateType VGuiWrap2_IsInCareerMatch()
{
	if (staticCareerUI)
		return g_careerState;

	return CAREER_NONE;
}

void VGui_ViewportPaintBackground(int* extents)
{
	int exts;
	rect_t rect;
	rect_t pnt;

	g_engdstAddrs.VGui_ViewportPaintBackground(&extents);
	GLFinishHud();

	int x = glx;
	int y = gly;
	int width = glwidth;
	int height = glheight;

	SDL_GetWindowPosition(*pmainwindow, &pnt.left, &pnt.top);

	if (VideoMode_IsWindowed())
		SDL_GetWindowSize(*pmainwindow, &rect.right, &rect.bottom);
	else
		VideoMode_GetCurrentVideoMode(&rect.right, &rect.bottom, 0);

	exts = extents[3];
	glx = *extents - pnt.left;
	gly = glheight + pnt.top - exts;
	glwidth = extents[2] - *extents;
	glheight = exts - extents[1];

	SCR_CalcRefdef();
	V_RenderView();
	GLBeginHud();

	if (g_pcls.state == ca_active && g_pcls.signon == ca_connecting)
	{
		AllowFog(false);
		ClientDLL_HudRedraw(g_pcl.intermission == 1);
		AllowFog(true);
	}

	if (!scr_drawloading && (g_pcl.intermission != 1 || key_dest))
		SCR_DrawConsole();

	SCR_DrawLoading();

	if (g_pcls.state == ca_active && g_pcls.signon == 2)
		Sbar_Draw();

	GLFinishHud();

	glx = x;
	gly = y;
	glwidth = width;
	glheight = height;

	SCR_CalcRefdef();
	GLBeginHud();
}

int EXT_FUNC VGuiWrap2_GetLocalizedStringLength(const char * label)
{
	NOT_IMPLEMENTED;
	return 0;
}

void StartLoadingProgressBar(const char * loadingType, int numProgressPoints)
{
	NOT_IMPLEMENTED;
}

void StopLoadingProgressBar()
{
	NOT_IMPLEMENTED;
}

void ContinueLoadingProgressBar(const char * loadingType, int progressPoint, float progressFraction)
{
	NOT_IMPLEMENTED;
}

void SetLoadingProgressBarStatusText(const char * statusText)
{
	NOT_IMPLEMENTED;
}

ICareerUI * VguiWrap2_GetCareerUI()
{
	NOT_IMPLEMENTED;
	return nullptr;
}
