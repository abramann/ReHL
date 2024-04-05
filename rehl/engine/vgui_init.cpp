#include "precompiled.h"
#include "enginesurface.h"
#include "../FakeVGUI/App.h"
#include "../FakeVGUI/Panel.h"


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

static EngineSurfaceWrap* staticEngineSurface = nullptr;
static vgui::Panel* staticPanel = nullptr;

void VGui_ViewportPaintBackground(int * extents)
{
	NOT_IMPLEMENTED;
}

void VGui_Startup()
{
	VGuiWrap_Startup();
	VGuiWrap2_Startup();
}

void VGui_Shutdown()
{
	VGuiWrap_Shutdown();
	VGuiWrap2_Shutdown();
	NOT_IMPLEMENTED;
	//EngineSurface::freeEngineSurface();
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
	auto pRoot = VGuiWrap_GetPanel();

	if (pRoot)
	{
		int x = 0, y = 0;
		rect_t rect;

		rect.left = 0;

		if (VideoMode_IsWindowed())
		{
			SDL_GetWindowPosition(pmainwindow, &x, &y);
			SDL_GetWindowSize(pmainwindow, &rect.right, &rect.bottom);
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

	IEngineSurface* pSurface = (IEngineSurface*)factoryFn(VENGINE_HLDS_API_VERSION, nullptr);

	staticEngineSurface = new EngineSurfaceWrap(staticPanel, pSurface);

	VGuiWrap_SetRootPanelSize();
}

void VGuiWrap_Shutdown()
{
	NOT_IMPLEMENTED;
}

bool VGuiWrap_CallEngineSurfaceAppHandler(void * event, void * userData)
{
	NOT_IMPLEMENTED;

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
	NOT_IMPLEMENTED;
}

void VGuiWrap2_Startup()
{
	NOT_IMPLEMENTED;
}

void VGuiWrap2_Shutdown()
{
	NOT_IMPLEMENTED;
}

bool VGuiWrap2_CallEngineSurfaceAppHandler(void * event, void * userData)
{
	NOT_IMPLEMENTED;
	return false;
}

bool VGuiWrap2_IsGameUIVisible()
{
	NOT_IMPLEMENTED;
	return false;
}

bool VGuiWrap2_UseVGUI1()
{
	NOT_IMPLEMENTED;
	return false;
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
	NOT_IMPLEMENTED;
	return false;
}

void VGuiWrap2_Paint()
{
	NOT_IMPLEMENTED;
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
	NOT_IMPLEMENTED;
	return false;
}

void VGuiWrap2_ShowConsole()
{
	NOT_IMPLEMENTED;
}

void VGuiWrap2_ShowDemoPlayer()
{
	NOT_IMPLEMENTED;
}

void VGuiWrap2_HideConsole()
{
	NOT_IMPLEMENTED;
}

void VGuiWrap2_ClearConsole()
{
	NOT_IMPLEMENTED;
}

void VGuiWrap2_ConPrintf(const char * msg)
{
	NOT_IMPLEMENTED;
}

void VGuiWrap2_ConDPrintf(const char * msg)
{
	NOT_IMPLEMENTED;
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

int VGuiWrap2_IsInCareerMatch()
{
	NOT_IMPLEMENTED;
	return 0;
}

int EXT_FUNC VGuiWrap2_GetLocalizedStringLength(const char * label)
{
	NOT_IMPLEMENTED;
	return 0;
}

ICareerUI * VguiWrap2_GetCareerUI()
{
	NOT_IMPLEMENTED;
	return nullptr;
}

class CDummyApp : public vgui::App
{
public:
	void main(int argc, char* argv[]) override;
	//~CDummyApp() = delete;
protected:
	void platTick() override;
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
static CDummyApp theApp;