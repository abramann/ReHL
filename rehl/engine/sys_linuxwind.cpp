/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    In addition, as a special exception, the author gives permission to
*    link the code of this program with the Half-Life Game Engine ("HL
*    Engine") and Modified Game Libraries ("MODs") developed by Valve,
*    L.L.C ("Valve").  You must obey the GNU General Public License in all
*    respects for all of the code used other than the HL Engine and MODs
*    from Valve.  If you modify this file, you may extend this exception
*    to your version of the file, but you are not obligated to do so.  If
*    you do not wish to do so, delete this exception statement from your
*    version.
*
*/

#include "precompiled.h"
#include "gl_vidnt.h"
#include "renderer.h"


CGame g_Game;
IGame *game = &g_Game;

void GetWindowNameFromGameDir(char* output, int outputBufferSize);

CGame::CGame()
{
	m_bActiveApp = true;
}

CGame::~CGame()
{
}

bool CGame::Init(void *pvInstance)
{
	return true;
}

bool CGame::Shutdown()
{
	return true;
}

bool CGame::CreateGameWindow()
{
	if (COM_CheckParm("-noontop"))
		SDL_SetHint("SDL_ALLOW_TOPMOST", "0");

	SDL_SetHint("SDL_VIDEO_X11_XRANDR", "1");
	SDL_SetHint("SDL_VIDEO_X11_XVIDMODE", "1");

	SDL_InitSubSystem(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);

	char gameWindowName[512];
	strcpy(gameWindowName, "Half-Life");

	// Check if liblist overrides the window title.
	GetWindowNameFromGameDir(gameWindowName, sizeof(gameWindowName));

	// Added SDL_WINDOW_OPENGL. - Solokiller
	Uint32 uiFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL;

	if (COM_CheckParm("-noborder"))
		uiFlags |= SDL_WINDOW_BORDERLESS;

	m_hSDLWindow = SDL_CreateWindow(gameWindowName, 0, 0, 640, 480, uiFlags);

	if (!m_hSDLWindow)
	{
		// Try 16 bit color depth.
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 3);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 3);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 3);
		m_hSDLWindow = SDL_CreateWindow(gameWindowName, 0, 0, 640, 480, uiFlags);

		if (!m_hSDLWindow)
			Sys_Error("Failed to create SDL Window");
	}

	byte* buffer = (byte*)malloc(512 * 512);
	int width, height;
	if (LoadTGA("game.tga", buffer, 512 * 512, &width, &height))
	{
		// Flip the image vertically.
		byte* bufferline = (byte*)malloc(4 * width);

		for (int i = 0; i < height / 2; ++i)
		{
			memcpy(bufferline, &buffer[width * i * 4], width * 4);
			memcpy(&buffer[width * i * 4], &buffer[width * (height - i - 1) * 4], width * 4);
			memcpy(&buffer[width * (height - i - 1) * 4], bufferline, width * 4);
		}

		free(bufferline);

		SDL_Surface* pSurface = SDL_CreateRGBSurfaceFrom(buffer, width, height, 32, 4 * width, 0xFF, 0xFF << 8, 0xFF << 16, 0xFF << 24);

		if (pSurface)
		{
			SDL_SetWindowIcon(m_hSDLWindow, pSurface);
			SDL_FreeSurface(pSurface);
		}
	}

	free(buffer);

	SDL_ShowWindow(m_hSDLWindow);

	m_hSDLGLContext = SDL_GL_CreateContext(m_hSDLWindow);

	if (!m_hSDLWindow || !m_hSDLGLContext)
		Sys_Error("Failed to create SDL Window");

	int r, g, b, a, depth;

	if (SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &r))
	{
		r = 0;
		Con_Printf("Failed to get GL RED size (%s)\n", SDL_GetError());
	}
	if (SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &g))
	{
		g = 0;
		Con_Printf("Failed to get GL GREEN size (%s)\n", SDL_GetError());
	}
	if (SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &b))
	{
		b = 0;
		Con_Printf("Failed to get GL BLUE size (%s)\n", SDL_GetError());
	}
	if (SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &a))
	{
		a = 0;
		Con_Printf("Failed to get GL ALPHA size (%s)\n", SDL_GetError());
	}
	if (SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &depth))
	{
		depth = 0;
		Con_Printf("Failed to get GL DEPTH size (%s)\n", SDL_GetError());
	}
	Con_Printf("GL_SIZES:  r:%d g:%d b:%d a:%d depth:%d\n", r, g, b, a, depth);
	if (r <= 4 || g <= 4 || b <= 4 || depth <= 15 || gl_renderer && Q_strstr(gl_renderer, "GDI Generic"))
		Sys_Error("Failed to create SDL Window, unsupported video mode. A 16-bit color depth desktop is required and a supported GL driver.\n");

	m_nMouseTargetX = 320;
	m_nMouseTargetY = 240;
	m_nWarpDelta = 213;

	return true;
}

void CGame::SleepUntilInput(int time)
{
	NOT_IMPLEMENTED_IGNORE;
#ifdef _WIN32
	Sleep(time * 1000);
#else // _WIN32
	sleep(time);
#endif // _WIN32
}

SDL_Window* CGame::GetMainWindow()
{
	return m_hSDLWindow;
}

SDL_Window** CGame::GetMainWindowAddress()
{
	return &m_hSDLWindow;
}

void CGame::SetWindowXY(int x, int y)
{
	m_x = x;
	m_y = y;
}

void CGame::SetWindowSize(int w, int h)
{
	m_width = w;
	m_height = h;

	SDL_SetWindowSize(m_hSDLWindow, w, h);
	m_nMouseTargetX = m_width / 2;
	m_nMouseTargetY = m_height / 2;

	int iMinWarp = 200;

	if ((m_height / 3) >= iMinWarp)
		iMinWarp = m_height / 3;

	m_nWarpDelta = iMinWarp;
}

void CGame::GetWindowRect(int *x, int *y, int *w, int *h)
{
	if (x) *x = 0;
	if (y) *y = 0;
	if (w) *w = 0;
	if (h) *h = 0;
}

bool CGame::IsActiveApp()
{
	return m_bActiveApp;
}

bool CGame::IsMultiplayer()
{
	return m_bMultiplayer;
}

void CGame::PlayStartupVideos()
{
}

void CGame::PlayAVIAndWait(const char *aviFile)
{
}

void CGame::SetCursorVisible(bool bState)
{
	NOT_IMPLEMENTED;
}

void CGame::SetActiveApp(bool active)
{
	m_bActiveApp = active;
}

void CGame::AppActivate(bool fActive)
{
	NOT_IMPLEMENTED;
}

void GetWindowNameFromGameDir(char* output, int outputBufferSize)
{
	FileHandle_t hLiblist = FS_Open("liblist.gam", "rt");

	if (hLiblist)
	{
		char line[512];

		while (!FS_EndOfFile(hLiblist))
		{
			*line = '\0';
			FS_ReadLine(line, sizeof(line) - 1, hLiblist);

			if (!Q_strnicmp(line, "game", 4))
			{
				auto pszStart = strchr(line, '"');

				if (pszStart)
				{
					auto pszEnd = strchr(pszStart + 1, '"');

					if (pszEnd)
					{
						const int uiLength = pszEnd - pszStart;

						if ((uiLength - 1) < outputBufferSize)
						{
							strncpy(output, pszStart + 1, uiLength - 1);
							output[uiLength - 1] = '\0';
						}
					}
				}
				break;
			}
		}

		FS_Close(hLiblist);
	}
}