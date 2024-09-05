#pragma once

#include "vmodes.h"
#include "utlvector.h"
#include <SDL.h>

/**
*	Manages the main window's video modes.
*/
class IVideoMode
{
public:
	virtual const char* GetName() = 0;

	virtual bool Init(void* pvInstance) = 0;
	virtual void Shutdown() = 0;

	virtual bool AddMode(int width, int height, int bpp) = 0;
	virtual vmode_t* GetCurrentMode() = 0;
	virtual vmode_t* GetMode(int num) = 0;
	virtual int GetModeCount() = 0;

	virtual bool IsWindowedMode() const = 0;

	virtual bool GetInitialized() const = 0;
	virtual void SetInitialized(bool init) = 0;

	virtual void UpdateWindowPosition() = 0;

	virtual void FlipScreen() = 0;

	virtual void RestoreVideo() = 0;
	virtual void ReleaseVideo() = 0;
};

class CVideoMode_Common : public IVideoMode
{
public:
	struct bimage_t
	{
		byte* buffer;
		int x;
		int y;
		int width;
		int height;
		bool scaled;
	};

	static const int MAX_MODES = 32;

public:
	CVideoMode_Common();

	bool Init(void* pvInstance) override;

protected:
	void LoadStartupGraphic();
	void CenterEngineWindow(SDL_Window* hWndCenter, int width, int height);
	void AdjustWindowForMode();
	void DrawStartupGraphic(SDL_Window* window);

public:
	void Shutdown() override;

	bool AddMode(int width, int height, int bpp) override;
	vmode_t* GetCurrentMode() override;
	vmode_t* GetMode(int num) override;
	int GetModeCount() override;

	bool IsWindowedMode() const override;

	bool GetInitialized() const override;
	void SetInitialized(bool init) override;

	void UpdateWindowPosition() override;

	void FlipScreen() override;

	void RestoreVideo() override;
	void ReleaseVideo() override;

	virtual int MaxBitsPerPixel();
	virtual void ReleaseFullScreen();
	virtual void ChangeDisplaySettingsToFullscreen();

protected:
	vmode_t m_rgModeList[MAX_MODES] = {};
	vmode_t m_safeMode;
	int m_nNumModes = 0;
	int m_nGameMode = 0;
	bool m_bInitialized = false;
	CUtlVector<CVideoMode_Common::bimage_t> m_ImageID;
	int m_iBaseResX = 0;
	int m_iBaseResY = 0;
	bool m_bWindowed = false;
};

class CVideoMode_OpenGL final : public CVideoMode_Common
{
public:
	CVideoMode_OpenGL(bool windowed);
	virtual ~CVideoMode_OpenGL();

	bool Init(void* pvInstance) override;

	const char* GetName() override { return "gl"; }

	void FlipScreen() override;

	void RestoreVideo() override;
	void ReleaseVideo() override;

	void ReleaseFullScreen() override;
	void ChangeDisplaySettingsToFullscreen() override;
};

extern bool bNeedsFullScreenModeSwitch;

EXTERN_VAR(IVideoMode*, videomode);
EXTERN_VAR(qboolean, g_bDisableMSAAFBO);

extern bool BUsesSDLInput();

extern void VideoMode_Create();

extern bool VideoMode_IsWindowed();

extern void VideoMode_GetVideoModes(vmode_t** liststart, int* count);

extern void VideoMode_GetCurrentVideoMode(int* wide, int* tall, int* bpp);

extern void VideoMode_GetCurrentRenderer(char* name, int namelen, int* windowed, int* hdmodels, int* addons_folder, int* vid_level);

extern void VideoMode_RestoreVideo();

extern void VideoMode_SetVideoMode(int width, int height, int bpp);

extern void VideoMode_SwitchMode(int hardware, int windowed);