#include "precompiled.h"
#include "qgl.h"
#include "sys_getmodes.h"
#include "gl_draw.h"
#include "gl_vidnt.h"


#include <SDL.h>
#include <SDL_syswm.h>

void CheckTextureExtensions();
void CheckMultiTextureExtensions();
void CheckATINPatchExtensions();

cvar_t gl_ztrick = { "gl_ztrick", "0" };
cvar_t gl_vsync = { "gl_vsync", "1", FCVAR_ARCHIVE };

rect_t window_rect = {};

int window_center_x = 0;
int window_center_y = 0;

int gGLHardwareType = 0;

int TEXTURE0_SGIS = GL_ASYNC_READ_PIXELS_SGIX;
int TEXTURE1_SGIS = GL_MAX_ASYNC_TEX_IMAGE_SGIX;
int TEXTURE2_SGIS = GL_MAX_ASYNC_DRAW_PIXELS_SGIX;

int gl_mtexable = 0;

SDL_Window* pmainwindow = nullptr;

struct FBO_Container_t
{
	GLuint s_hBackBufferFBO;
	GLuint s_hBackBufferCB;
	GLuint s_hBackBufferDB;
	GLuint s_hBackBufferTex;
};

static bool gfMiniDriver = false;
static bool s_bEnforceAspect = false;
static bool bDoMSAAFBO = true;
static bool bDoScaledFBO = true;
static bool s_bSupportsBlitTexturing = false;

static int gl_filter_min = GL_LINEAR;

const char* gl_vendor = nullptr;
const char* gl_renderer = nullptr;
const char* gl_version = nullptr;
const char* gl_extensions = nullptr;

bool g_bSupportsNPOTTextures = false;
bool atismoothing = false;

static FBO_Container_t s_MSAAFBO;
static FBO_Container_t s_BackBufferFBO;

static float s_fXMouseAspectAdjustment = 1.0;
static float s_fYMouseAspectAdjustment = 1.0;

void FreeFBOObjects();
#ifdef WIN32
bool bSetupPixelFormat(HDC hdc);
#endif

float GetXMouseAspectRatioAdjustment()
{
	return s_fXMouseAspectAdjustment;
}

float GetYMouseAspectRatioAdjustment()
{
	return s_fYMouseAspectAdjustment;
}

void GL_Init()
{
	gl_vendor = reinterpret_cast<const char*>(qglGetString(GL_VENDOR));
	gl_renderer = reinterpret_cast<const char*>(qglGetString(GL_RENDERER));
	gl_version = reinterpret_cast<const char*>(qglGetString(GL_VERSION));
	gl_extensions = reinterpret_cast<const char*>(qglGetString(GL_EXTENSIONS));
	g_bSupportsNPOTTextures = 0;

	int r;
	int g;
	int b;
	int a;
	int depth;

	if (SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &r))
	{
		r = 0;
		Con_DPrintf("Failed to get GL RED size (%s)\n", SDL_GetError());
	}

	if (SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &g))
	{
		g = 0;
		Con_DPrintf("Failed to get GL GREEN size (%s)\n", SDL_GetError());
	}

	if (SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &b))
	{
		b = 0;
		Con_DPrintf("Failed to get GL BLUE size (%s)\n", SDL_GetError());
	}

	if (SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &a))
	{
		a = 0;
		Con_DPrintf("Failed to get GL ALPHA size (%s)\n", SDL_GetError());
	}

	if (SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &depth))
	{
		depth = 0;
		Con_DPrintf("Failed to get GL DEPTH size (%s)\n", SDL_GetError());
	}


	CheckTextureExtensions();
	CheckMultiTextureExtensions();
	CheckATINPatchExtensions();

	if (gl_extensions && Q_strstr(gl_extensions, "GL_ARB_texture_non_power_of_two"))
		g_bSupportsNPOTTextures = true;

	qglClearColor(1.0, 0.0, 0.0, 0.0);
	qglCullFace(GL_FRONT);
	qglEnable(GL_TEXTURE_2D);
	qglEnable(GL_ALPHA_TEST);
	qglAlphaFunc(GL_NOTEQUAL, 0.0);
	qglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	qglShadeModel(GL_FLAT);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, gl_ansio.value);
	qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	GL_Config();
}

void GL_BeginRendering(int* x, int* y, int* width, int* height)
{
	NOT_IMPLEMENTED;
}

void GL_EndRendering()
{
	NOT_IMPLEMENTED;
}

bool GL_SetMode(SDL_Window* mainwindow, HDC* pmaindc, HGLRC* pbaseRC, const char* pszDriver, const char* pszCmdLine)
{
	gfMiniDriver = false;

	if (pmaindc)
		*pmaindc = 0;

	*pbaseRC = 0;
	*pmaindc = 0;

	// There's an if statement here in the Windows version to see if a global bool is false.
	// It disables the second context creation.
	// The boolean is only ever false, explaining why it's missing in the Linux version. - Solokiller
	// if( !g_bDisableMSAAFBO )
	//{
	s_bEnforceAspect = COM_CheckParm("-stretchaspect") == 0;

	if (COM_CheckParm("-nomsaa"))
		bDoMSAAFBO = false;

	if (COM_CheckParm("-nofbo"))
		bDoScaledFBO = false;

	if (Host_GetVideoLevel() > 0)
	{
		gl_filter_min = GL_LINEAR_MIPMAP_NEAREST;
		bDoScaledFBO = false;
	}

#ifdef WIN32
	const int freqIndex = COM_CheckParm("-freq");

	if (freqIndex)
	{
		if (VideoMode_IsWindowed())
		{
			Con_DPrintf("Setting frequency in windowed mode is unsupported\n");
		}
		else
		{
			DEVMODEA DevMode;
			DEVMODEA target;

			memset(&DevMode, 0, sizeof(DevMode));
			DevMode.dmSize = sizeof(DevMode);

			EnumDisplaySettingsA(nullptr, ENUM_CURRENT_SETTINGS, &DevMode);

			memset(&target, 0, sizeof(DevMode));
			target.dmSize = sizeof(target);

			target.dmDisplayFrequency = atoi(com_argv[freqIndex + 1]);
			target.dmFields |= DM_DISPLAYFREQUENCY;

			Con_DPrintf("Setting monitor frequency to %d\n", target.dmDisplayFrequency);

			if (ChangeDisplaySettingsExA(nullptr, &target, NULL, CDS_FULLSCREEN, nullptr) && ChangeDisplaySettingsA(&target, CDS_FULLSCREEN))
				Con_DPrintf("Frequency %d is not supported by your monitor\n", target.dmDisplayFrequency);
		}
	}
#endif

	// TODO: remove obsolete parameters - Solokiller
	QGL_Init(pszDriver, pszCmdLine);
	s_bSupportsBlitTexturing = false;

	gl_extensions = reinterpret_cast<const char*>(qglGetString(GL_EXTENSIONS));

	if (gl_extensions &&
		Q_strstr(gl_extensions, "GL_EXT_framebuffer_multisample_blit_scaled"))
		s_bSupportsBlitTexturing = true;

	if (COM_CheckParm("-directblit"))
		s_bSupportsBlitTexturing = true;

	if (COM_CheckParm("-nodirectblit"))
		s_bSupportsBlitTexturing = false;

	if (!qglGenFramebuffersEXT || !qglBindFramebufferEXT || !qglBlitFramebufferEXT)
		bDoScaledFBO = false;

	if (gl_extensions && !Q_strstr(gl_extensions, "GL_ARB_texture_rectangle") && !Q_strstr(gl_extensions, "GL_NV_texture_rectangle"))
		bDoScaledFBO = false;

	s_MSAAFBO.s_hBackBufferFBO = 0;

	if (VideoMode_IsWindowed())
		return true;

	s_MSAAFBO.s_hBackBufferCB = 0;
	s_MSAAFBO.s_hBackBufferDB = 0;
	s_MSAAFBO.s_hBackBufferTex = 0;
	s_BackBufferFBO.s_hBackBufferFBO = 0;
	s_BackBufferFBO.s_hBackBufferCB = 0;
	s_BackBufferFBO.s_hBackBufferDB = 0;
	s_BackBufferFBO.s_hBackBufferTex = 0;

	int wide;
	int tall;

	VideoMode_GetCurrentVideoMode(&wide, &tall, nullptr);

	if (qglRenderbufferStorageMultisampleEXT && bDoMSAAFBO && gl_extensions && Q_strstr(gl_extensions, "GL_EXT_framebuffer_multisample"))
	{
		qglGenFramebuffersEXT(1, &s_MSAAFBO.s_hBackBufferFBO);
		qglBindFramebufferEXT(GL_FRAMEBUFFER, s_MSAAFBO.s_hBackBufferFBO);

		qglGenRenderbuffersEXT(1, &s_MSAAFBO.s_hBackBufferCB);
		qglBindRenderbufferEXT(GL_RENDERBUFFER, s_MSAAFBO.s_hBackBufferCB);

		qglRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER, 4, GL_RGBA8, wide, tall);
		qglFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, s_MSAAFBO.s_hBackBufferCB);

		qglGenRenderbuffersEXT(1, &s_MSAAFBO.s_hBackBufferDB);
		qglBindRenderbufferEXT(GL_RENDERBUFFER, s_MSAAFBO.s_hBackBufferDB);

		qglRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, wide, tall);

		qglFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, s_MSAAFBO.s_hBackBufferDB);

		if (qglCheckFramebufferStatusEXT(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			if (s_MSAAFBO.s_hBackBufferFBO)
				qglDeleteFramebuffersEXT(1, &s_MSAAFBO.s_hBackBufferFBO);
			s_MSAAFBO.s_hBackBufferFBO = 0;

			if (s_MSAAFBO.s_hBackBufferCB)
				qglDeleteRenderbuffersEXT(1, &s_MSAAFBO.s_hBackBufferCB);
			s_MSAAFBO.s_hBackBufferCB = 0;

			if (s_MSAAFBO.s_hBackBufferDB)
				qglDeleteRenderbuffersEXT(1, &s_MSAAFBO.s_hBackBufferDB);
			s_MSAAFBO.s_hBackBufferDB = 0;

			if (s_MSAAFBO.s_hBackBufferTex)
				qglDeleteTextures(1, &s_MSAAFBO.s_hBackBufferTex);
			s_MSAAFBO.s_hBackBufferTex = 0;

			Con_DPrintf("Error initializing MSAA frame buffer\n");
		}
	}
	else
	{
		Con_DPrintf("MSAA backbuffer rendering disabled.\n");
		s_MSAAFBO.s_hBackBufferFBO = 0;
	}

	if (bDoScaledFBO)
	{
		if (s_MSAAFBO.s_hBackBufferFBO)
			glEnable(GL_MULTISAMPLE);

		glEnable(GL_TEXTURE_RECTANGLE);
		qglGenFramebuffersEXT(1, &s_BackBufferFBO.s_hBackBufferFBO);
		qglBindFramebufferEXT(GL_FRAMEBUFFER, s_BackBufferFBO.s_hBackBufferFBO);

		if (!s_MSAAFBO.s_hBackBufferFBO)
		{
			qglGenRenderbuffersEXT(1, &s_BackBufferFBO.s_hBackBufferDB);
			qglBindRenderbufferEXT(GL_RENDERBUFFER, s_BackBufferFBO.s_hBackBufferDB);
			qglRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, wide, tall);
			qglFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, s_BackBufferFBO.s_hBackBufferDB);
		}

		glGenTextures(1, &s_BackBufferFBO.s_hBackBufferTex);

		glBindTexture(GL_TEXTURE_RECTANGLE, s_BackBufferFBO.s_hBackBufferTex);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, wide, tall, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		qglFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, s_BackBufferFBO.s_hBackBufferTex, 0);

		glBindTexture(GL_TEXTURE_RECTANGLE, 0);

		glDisable(GL_TEXTURE_RECTANGLE);
	}

	if (!bDoScaledFBO || qglCheckFramebufferStatusEXT(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		FreeFBOObjects();

		SDL_DisplayMode requestedMode;
		SDL_DisplayMode mode;

		requestedMode.format = 0;
		requestedMode.refresh_rate = 0;
		requestedMode.w = wide;
		requestedMode.h = tall;

		if (!SDL_GetClosestDisplayMode(0, &requestedMode, &mode))
			Sys_Error("Error initializing Main frame buffer\n");
		if (bDoScaledFBO)
			Con_DPrintf("FBO backbuffer rendering disabled due to create error.\n");
		else
			Con_DPrintf("FBO backbuffer rendering disabled.\n");
		SDL_SetWindowDisplayMode(mainwindow, &mode);

		bNeedsFullScreenModeSwitch = true;
		VideoMode_RestoreVideo();
	}

	if (bDoScaledFBO)
	{
		qglBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	}

	return true;
	//} //!g_bDisableMSAAFBO

#ifdef WIN32
	SDL_SysWMinfo wmInfo;

	SDL_VERSION(&wmInfo.version);
	
	SDL_GetWindowWMInfo(mainwindow, &wmInfo);

	HDC hDC = GetDC(wmInfo.info.win.window);

	*pmaindc = hDC;

	// TODO: why? - Solokiller
	s_BackBufferFBO.s_hBackBufferFBO = 0;

	if (!bSetupPixelFormat(hDC))
		return false;

	auto context = qwglCreateContext(hDC);

	*pbaseRC = context;

	if (!context)
		return false;

	return qwglMakeCurrent(hDC, context) != FALSE;
#else
	return true;
#endif
}

bool VID_Init(unsigned short *palette)
{
	Cvar_RegisterVariable(&gl_ztrick);
	Cvar_RegisterVariable(&gl_vsync);
	if (COM_CheckParm("-gl_log"))
		Cmd_AddCommand((char *)"gl_log", GLimp_EnableLogging);

	return true;
}

void VID_UpdateWindowVars(rect_t* prc, int x, int y)
{
	window_rect = *prc;

	if (pmainwindow)
	{
		int w, h;
		SDL_GetWindowSize(reinterpret_cast<SDL_Window*>(pmainwindow), &w, &h);

		int x_0, y_0;
		SDL_GetWindowPosition(reinterpret_cast<SDL_Window*>(pmainwindow), &x_0, &y_0);

		prc->left = x_0;
		prc->right = x_0 + w;

		prc->top = y_0;
		prc->bottom = y_0 + h;
		window_center_x = x_0 + w / 2;
		window_center_y = y_0 + h / 2;
	}
	else
	{
		window_center_x = x;
		window_center_y = y;
	}
}

void VID_TakeSnapshotRect(const char* pFilename, int x, int y, int w, int h)
{
	NOT_IMPLEMENTED;
}

void VID_TakeSnapshot(const char* pFilename)
{
	NOT_IMPLEMENTED;
}


void VID_WriteBuffer(const char* pFilename)
{
	NOT_IMPLEMENTED;
}

void VID_SetPalette(unsigned char *palette)
{
	NOT_IMPLEMENTED;
}

NOBODY void VID_ShiftPalette(unsigned char *palette)
{
	NOT_IMPLEMENTED;
}

void FreeFBOObjects()
{
	if (s_MSAAFBO.s_hBackBufferFBO)
		qglDeleteFramebuffersEXT(1, &s_MSAAFBO.s_hBackBufferFBO);
	s_MSAAFBO.s_hBackBufferFBO = 0;

	if (s_MSAAFBO.s_hBackBufferCB)
		qglDeleteRenderbuffersEXT(1, &s_MSAAFBO.s_hBackBufferCB);
	s_MSAAFBO.s_hBackBufferCB = 0;

	if (s_MSAAFBO.s_hBackBufferDB)
		qglDeleteRenderbuffersEXT(1, &s_MSAAFBO.s_hBackBufferDB);
	s_MSAAFBO.s_hBackBufferDB = 0;

	if (s_MSAAFBO.s_hBackBufferTex)
		qglDeleteTextures(1, &s_MSAAFBO.s_hBackBufferTex);
	s_MSAAFBO.s_hBackBufferTex = 0;

	if (s_BackBufferFBO.s_hBackBufferFBO)
		qglDeleteFramebuffersEXT(1, &s_BackBufferFBO.s_hBackBufferFBO);
	s_BackBufferFBO.s_hBackBufferFBO = 0;

	if (s_BackBufferFBO.s_hBackBufferCB)
		qglDeleteRenderbuffersEXT(1, &s_BackBufferFBO.s_hBackBufferCB);
	s_BackBufferFBO.s_hBackBufferCB = 0;

	if (s_BackBufferFBO.s_hBackBufferDB)
		qglDeleteRenderbuffersEXT(1, &s_BackBufferFBO.s_hBackBufferDB);
	s_BackBufferFBO.s_hBackBufferDB = 0;

	if (s_BackBufferFBO.s_hBackBufferTex)
		qglDeleteTextures(1, &s_BackBufferFBO.s_hBackBufferTex);
	s_BackBufferFBO.s_hBackBufferTex = 0;
}

#ifdef WIN32
// Note: this code is obsolete and won't have any effect.
// The cross-platform OpenGL context creation code supersedes this. - Solokiller
bool bSetupPixelFormat(HDC hdc)
{
	static PIXELFORMATDESCRIPTOR ppfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
		1,							   // version number
		PFD_DRAW_TO_WINDOW			   // support window
		| PFD_SUPPORT_OPENGL	   // support OpenGL
		| PFD_DOUBLEBUFFER,		   // double buffered
		PFD_TYPE_RGBA,				   // RGBA type
		32,							   // 32-bit color depth
		0, 0, 0, 0, 0, 0,			   // color bits ignored
		0,							   // no alpha buffer
		0,							   // shift bit ignored
		0,							   // no accumulation buffer
		0, 0, 0, 0,					   // accum bits ignored
		32,							   // 32-bit z-buffer
		0,							   // no stencil buffer
		0,							   // no auxiliary buffer
		PFD_MAIN_PLANE,				   // main layer
		0,							   // reserved
		0, 0, 0						   // layer masks ignored
	};

	static bool bInitialized = false;

	if (bInitialized)
		return true;

	bInitialized = true;

	if (gfMiniDriver)
	{
		int index = qwglChoosePixelFormat(hdc, &ppfd);

		if (!index)
		{
			MessageBoxA(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
			return false;
		}

		if (!qwglSetPixelFormat(hdc, index, &ppfd))
		{
			MessageBoxA(NULL, "SetPixelFormat failed", "Error", MB_OK);
			return false;
		}

		qwglDescribePixelFormat(hdc, index, sizeof(ppfd), &ppfd);

		return true;
	}

	const int index = ChoosePixelFormat(hdc, &ppfd);

	if (!index)
	{
		MessageBoxA(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return false;
	}

	if (!SetPixelFormat(hdc, index, &ppfd))
	{
		MessageBoxA(NULL, "SetPixelFormat failed", "Error", MB_OK);
		return false;
	}

	DescribePixelFormat(hdc, index, sizeof(ppfd), &ppfd);

	// if not a generic Software driver or a hardware accelerated generic driver, we can use OpenGL.
	if (!(ppfd.dwFlags & PFD_GENERIC_FORMAT) || (ppfd.dwFlags & PFD_GENERIC_ACCELERATED))
		return true;

	return false;
}
#endif


void GL_Config()
{
	Cbuf_InsertText("exec hw/opengl.cfg\n");

	char lowerCase[256];
	Q_strncpy(lowerCase, gl_renderer, ARRAYSIZE(lowerCase) - 1);
	lowerCase[ARRAYSIZE(lowerCase) - 1] = '\0';

	if (Q_strstr(gl_vendor, "3Dfx"))
	{
		gGLHardwareType = 1;
		if (Q_strstr(gl_renderer, "Voodoo(tm)"))
		{
			Cbuf_InsertText("exec hw/3DfxVoodoo1.cfg\n");
		}
		else if (Q_strstr(gl_renderer, "Voodoo^2"))
		{
			Cbuf_InsertText("exec hw/3DfxVoodoo2.cfg\n");
		}
		else
		{
			Cbuf_InsertText("exec hw/3Dfx.cfg\n");
		}
	}
	else if (Q_strstr(gl_vendor, "NVIDIA"))
	{
		if (Q_strstr(gl_renderer, "RIVA 128"))
		{
			gGLHardwareType = 2;
			Cbuf_InsertText("exec hw/riva128.cfg\n");
		}
		else if (Q_strstr(gl_renderer, "TNT"))
		{
			gGLHardwareType = 10;
			Cbuf_InsertText("exec hw/rivaTNT.cfg\n");
		}
		else if (strstr(gl_renderer, "Quadro") ||
			strstr(gl_renderer, "GeForce"))
		{
			gGLHardwareType = 11;
			Cbuf_InsertText("exec hw/geforce.cfg\n");
		}
	}
	else if (Q_strstr(lowerCase, "riva tnt") ||
		Q_strstr(lowerCase, "velocity 4400"))
	{
		Cbuf_InsertText("exec hw/rivaTNT.cfg\n");
		gGLHardwareType = 10;
	}
	else if (Q_strstr(gl_vendor, "PCX2"))
	{
		Cbuf_InsertText("exec hw/PowerVRPCX2.cfg\n");
		gGLHardwareType = 3;
	}
	else if (Q_strstr(gl_vendor, "PowerVR"))
	{
		Cbuf_InsertText("exec hw/PowerVRSG.cfg\n");
		gGLHardwareType = 4;
	}
	else if (Q_strstr(gl_vendor, "V2200"))
	{
		Cbuf_InsertText("exec hw/V2200.cfg\n");
		gGLHardwareType = 5;
	}
	else if (Q_strstr(gl_vendor, "3Dlabs"))
	{
		Cbuf_InsertText("exec hw/3Dlabs.cfg\n");
		gGLHardwareType = 6;
	}
	else if (Q_strstr(gl_vendor, "Matrox"))
	{
		gGLHardwareType = 0;
		Cbuf_InsertText("exec hw/matrox.cfg\n");
	}
	else if (Q_strstr(gl_vendor, "ATI") && (Q_strstr(gl_renderer, "Rage") || Q_strstr(gl_renderer, "RAGE")) && Q_strstr(gl_renderer, "128"))
	{
		gGLHardwareType = 0;
		Cbuf_InsertText("exec hw/ATIRage128.cfg\n");
	}
	else
	{
		gGLHardwareType = 0;
		if (Q_strstr(gl_renderer, "Matrox") &&
			Q_strstr(gl_renderer, "G200"))
		{
			Cbuf_InsertText("exec hw/G200d3d.cfg\n");
		}
		else if (Q_strstr(gl_renderer, "ATI") && (Q_strstr(gl_renderer, "Rage") || Q_strstr(gl_renderer, "RAGE")) && Q_strstr(gl_renderer, "128"))
		{
			Cbuf_InsertText("exec hw/ATIRage128d3d.cfg\n");
		}
		else if (Q_strstr(gl_vendor, "NVIDIA"))
		{
			Cbuf_InsertText("exec hw/nvidiad3d.cfg\n");
		}
	}
}

void CheckTextureExtensions()
{
	if (gl_extensions &&
		(Q_strstr(gl_extensions, "GL_EXT_paletted_texture") &&
			Q_strstr(gl_extensions, "GL_EXT_shared_texture_palette")))
	{
		qglColorTableEXT = reinterpret_cast<decltype(qglColorTableEXT)>(SDL_GL_GetProcAddress("glColorTableEXT"));
		Con_DPrintf("Found paletted texture extension.\n");
	}
	else
	{
		qglColorTableEXT = nullptr;
	}

	if (gl_extensions && Q_strstr(gl_extensions, "GL_EXT_texture_object "))
	{
		qglBindTexture = reinterpret_cast<decltype(qglBindTexture)>(SDL_GL_GetProcAddress("glBindTextureEXT"));
		if (!qglBindTexture)
			Sys_Error("GetProcAddress for BindTextureEXT failed");
	}
}

void CheckMultiTextureExtensions()
{
	if (gl_extensions && Q_strstr(gl_extensions, "GL_ARB_multitexture "))
	{
		Con_DPrintf("ARB Multitexture extensions found.\n");

		qglMTexCoord2fSGIS = reinterpret_cast<decltype(qglMTexCoord2fSGIS)>(SDL_GL_GetProcAddress("glMultiTexCoord2fARB"));
		qglSelectTextureSGIS = reinterpret_cast<decltype(qglSelectTextureSGIS)>(SDL_GL_GetProcAddress("glActiveTextureARB"));

		TEXTURE0_SGIS = GL_TEXTURE0;
		TEXTURE1_SGIS = GL_TEXTURE1;
		TEXTURE2_SGIS = GL_TEXTURE2;

		oldtarget = TEXTURE0_SGIS;
		gl_mtexable = 2;
		GL_SelectTexture(TEXTURE0_SGIS);

		if (Q_strstr(gl_extensions, "GL_ARB_texture_env_combine ") ||
			Q_strstr(gl_extensions, "GL_EXT_texture_env_combine "))
		{
			GLint num;
			qglGetIntegerv(GL_MAX_TEXTURE_UNITS, &num);

			if (num > 2)
			{
				Con_DPrintf("%d texture units.  Detail texture supported.\n", num);
				gl_mtexable = num;
				DT_Initialize();
			}
		}
	}
	else if (gl_extensions && Q_strstr(gl_extensions, "GL_SGIS_multitexture "))
	{
		Con_DPrintf("Multitexture extensions found.\n");

		qglMTexCoord2fSGIS = reinterpret_cast<decltype(qglMTexCoord2fSGIS)>(SDL_GL_GetProcAddress("glMTexCoord2fSGIS"));
		qglSelectTextureSGIS = reinterpret_cast<decltype(qglSelectTextureSGIS)>(SDL_GL_GetProcAddress("glSelectTextureSGIS"));

		TEXTURE0_SGIS = QGL_TEXTURE0_SGIS;
		TEXTURE1_SGIS = QGL_TEXTURE1_SGIS;
		TEXTURE2_SGIS = QGL_TEXTURE2_SGIS;

		oldtarget = TEXTURE0_SGIS;
		gl_mtexable = 2;
		GL_SelectTexture(TEXTURE0_SGIS);
	}
	else
	{
		Con_DPrintf("NO Multitexture extensions found.\n");
	}
}

// TODO: check if this is used on Windows - Solokiller
void CheckArrayExtensions()
{
	auto pszExtensions = reinterpret_cast<const char*>(qglGetString(GL_EXTENSIONS));

	if (pszExtensions)
	{
		while (*pszExtensions)
		{
			if (!Q_strncmp(pszExtensions, "GL_EXT_vertex_array", Q_strlen("GL_EXT_vertex_array")))
				return;

			++pszExtensions;
		}
	}

	Sys_Error("Vertex array extension not present");
}

void CheckATINPatchExtensions()
{
	atismoothing = false;
}