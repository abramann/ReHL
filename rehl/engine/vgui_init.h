#pragma once

#include "FileSystem.h"
#include "const.h"


namespace vgui
{
	class Panel;
}

class ICareerUI;

extern CUtlVector<char> g_TempConsoleBuffer;

// VGUI interface

extern void VGui_ViewportPaintBackground(int* extents);

extern void VGui_Startup();

extern void VGui_Shutdown();

extern void VGui_CallEngineSurfaceAppHandler(void* event, void* userData);

extern vgui::Panel* VGui_GetPanel();

extern void VGui_ReleaseMouse();

extern void VGui_GetMouse();

extern void VGui_SetVisible(bool state);

extern void VGui_Paint();

extern bool VGui_GameUIKeyPressed();

extern bool VGui_Key_Event(int down, int keynum, const char* pszCurrentBinding);

extern bool VGui_LoadBMP(FileHandle_t file, byte* buffer, int bufsize, int* width, int* height);

// VGUI1 wrappers

extern void VGuiWrap_SetRootPanelSize();

extern void VGuiWrap_Startup();

extern void VGuiWrap_Shutdown();

extern bool VGuiWrap_CallEngineSurfaceAppHandler(void* event, void* userData);

extern vgui::Panel* VGuiWrap_GetPanel();

extern void VGuiWrap_ReleaseMouse();

extern void VGuiWrap_GetMouse();

extern void VGuiWrap_SetVisible(bool state);

extern void VGuiWrap_Paint(bool paintAll);

// VGUI2 wrappers

extern void VGuiWrap2_Startup();

extern void VGuiWrap2_Shutdown();

extern bool VGuiWrap2_CallEngineSurfaceAppHandler(void* event, void* userData);

extern bool VGuiWrap2_IsGameUIVisible();

extern bool VGuiWrap2_UseVGUI1();

vgui::Panel* VGuiWrap2_GetPanel();

extern void VGuiWrap2_ReleaseMouse();

extern void VGuiWrap2_GetMouse();

extern void VGuiWrap2_SetVisible(bool state);

extern bool VGuiWrap2_GameUIKeyPressed();

extern bool VGuiWrap2_Key_Event(int down, int keynum, const char* pszCurrentBinding);

extern void VGuiWrap2_Paint();

extern void VGuiWrap2_NotifyOfServerDisconnect();

extern void VGuiWrap2_HideGameUI();

extern bool VGuiWrap2_IsConsoleVisible();

extern void VGuiWrap2_ShowConsole();

extern void VGuiWrap2_ShowDemoPlayer();

extern void VGuiWrap2_HideConsole();

extern void VGuiWrap2_ClearConsole();

extern void VGuiWrap2_ConPrintf(const char* msg);

extern void VGuiWrap2_ConDPrintf(const char* msg);

extern void VGuiWrap2_LoadingStarted(const char* resourceType, const char* resourceName);

extern void VGuiWrap2_LoadingFinished(const char* resourceType, const char* resourceName);

extern void VGuiWrap2_NotifyOfServerConnect(const char* game, int IP, int port);

int VGuiWrap2_IsInCareerMatch();

extern ICareerUI* VguiWrap2_GetCareerUI();

extern int VGuiWrap2_GetLocalizedStringLength(const char* label);

extern void VGuiWrap2_GetMouseDelta(int* x, int* y);

extern void VGui2_OnDisconnectFromServer(int eLoginFailure);

extern void StartLoadingProgressBar(const char* loadingType, int numProgressPoints);

extern void ContinueLoadingProgressBar(const char* loadingType, int progressPoint, float progressFraction);

extern void SetLoadingProgressBarStatusText(const char* statusText);

extern void StartLoadingProgressBar(const char *loadingType, int numProgressPoints);

extern void StopLoadingProgressBar();

extern void SetLoadingProgressBarStatusText(const char *statusText);

extern void ContinueLoadingProgressBar(const char *loadingType, int progressPoint, float progressFraction);

extern void SetSecondaryProgressBar(float progress);

extern void SetSecondaryProgressBarText(const char* statusText);

extern void ValidateCDKey(int force, int inConnect);

extern void RegisterTutorMessageShown(int mid);

extern int GetTimesTutorMessageShown(int mid);

extern void ProcessTutorMessageDecayBuffer(int* buffer, int bufferLength);

extern void ConstructTutorMessageDecayBuffer(int* buffer, int bufferLength);

extern void ResetTutorMessageDecayData();