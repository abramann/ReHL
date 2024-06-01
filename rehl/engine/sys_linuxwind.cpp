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
#include "hooker.h"

extern cvar_t m_rawinput;
extern bool s_bCursorVisible;

#ifdef SHARED_GAME_DATA
IGame** sp_game = ADDRESS_OF_DATA(IGame**, 0xAC778);
IGame*& game = *sp_game;
extern cvar_t& cl_mousegrab;

#else
volatile CGame g_Game;
IGame *game = (IGame*)&g_Game;
extern cvar_t cl_mousegrab;
#endif

char CSWTCH_79[228] =
{
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','1','2','3','4',
	'5','6','7','8','9','0','\r','\x1B','\x7F','\t',' ','-','=','[',']','\\','\0',';','\'','`',',','.','/','\xAF','\x87','\x88',
	'\x89','\x8A','\x8B','\x8C','\x8D','\x8E','\x8F','\x90','\x91','\x92','\0','\0','\xFF','\x93','\x97','\x96','\x94','\x98','\x95',
	'\x83','\x82','\x81','\x80','\0','\xAC','\xB0','\xAD','\xAE','\xA9','\xA6','\xA7','\xA8','\xA3','\xA4','\xA5','\xA0','\xA1','\xA2',
	'\xAA','\xAB','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
	'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
	'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
	'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
	'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\xAB','\0','\0','\0','\x85','\x86','\x84','\xB1','\x85','\x86','\x84',
	'\xB1',
};

#ifdef SHARED_GAME_DATA
uint32* sp_mouseCode = ADDRESS_OF_DATA(uint32*, 0xB02FF);
uint32& mouseCode = *sp_mouseCode;
#else
uint32 mouseCode;
#endif

void GetWindowNameFromGameDir(char* output, int outputBufferSize);

bool BUsesSDLInput();

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
	//return Call_Method<bool, CGame>(0xB0B90, this);
	if (COM_CheckParm("-noontop"))
		SDL_SetHint("SDL_ALLOW_TOPMOST", "0");

#ifndef _WIN32
	SDL_SetHint("SDL_VIDEO_X11_XRANDR", "1");
	SDL_SetHint("SDL_VIDEO_X11_XVIDMODE", "1");
#endif
	SDL_InitSubSystem(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);

	char gameWindowName[512];
	Q_strcpy(gameWindowName, "Half-Life");

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

void CGame::SleepUntilInput(int sleepTime)
{
	return Call_Method<void, CGame, int>(0xAFF40, this, sleepTime);
#ifdef SWDS
#ifdef _WIN32
	Sleep(time * 1000);
#else // _WIN32
	sleep(time);
#endif // _WIN32
#else
	int deltaX;
	int deltaY;

	SDL_Event ev;

	SDL_PumpEvents();

	if (SDL_WaitEventTimeout(&ev, sleepTime) <= 0)
		return;

	while (true)
	{
		if (ev.type < SDL_TEXTINPUT)
		{
			if (ev.type == SDL_WINDOWEVENT)
			{
				//if (eng->GetState() == 1)
				//	eng->SetQuitting(1);

				switch (ev.window.event)
				{
				case 1:
				case 12:
					AppActivate(true);
					break;

				case 2:
				case 13:
					AppActivate(false);
					break;

				case 4:
					CHECK_REQUIRED;
					if (videomode->IsWindowedMode())
					{
						game->SetWindowXY(ev.window.data1, ev.window.data2);
						videomode->UpdateWindowPosition();
					}
					break;

				case 5:
					game->SetWindowXY(ev.window.data1, ev.window.data2);
					videomode->UpdateWindowPosition();
					break;

				case 14:
					if (eng->GetState() == 1)
						eng->SetQuitting(1);

					SDL_DestroyWindow(m_hSDLWindow);
					m_hSDLWindow = nullptr;
					break;

				default:
					break;
				}
			}
			else
			{
				if (ev.type > SDL_WINDOWEVENT)
				{
					char k = '\0';

					switch (ev.type)
					{
					case SDL_KEYDOWN:
						if ((ev.window.data1 - 4) <= 227)
							k = CSWTCH_79[ev.window.data1 - 4];

						eng->TrapKey_Event(k, true);
						break;

					case SDL_KEYUP:
						if (ev.window.data1 - 4 <= 227)
							k = CSWTCH_79[ev.window.data1 - 4];

						eng->TrapKey_Event(k, false);
						break;

					default:
						break;
					}
				}
			}
		}

		else if (ev.type == SDL_TEXTINPUT)
		{
			if (key_dest == key_message)
			{
				int strLen = 119 - chat_bufferlen;

				char* editText = &ev.edit.text[strlen(ev.edit.text)];

				if (editText - (char *)&ev.jhat.hat <= 119 - chat_bufferlen)
					strLen = editText - (char *)&ev.jhat.hat;

				Q_strncpy(&chat_buffer[chat_bufferlen], ev.edit.text, strLen);

				chat_bufferlen += strLen;

				chat_buffer[chat_bufferlen] = '\0';
			}
		}

		else if (ev.type > SDL_MOUSEBUTTONUP)
		{
			if (ev.type == SDL_MOUSEWHEEL)
			{
				eng->TrapKey_Event((*(short*)(&ev.syswm + 10) > 0) + 239, true);
				eng->TrapKey_Event((*(short*)(&ev.syswm + 10) > 0) + 239, false);
			}
		}

		else if (ev.type >= SDL_MOUSEBUTTONDOWN)
		{
			switch (ev.edit.text[4])
			{
			case 1:
				if (ev.type == SDL_MOUSEBUTTONDOWN)
				{
					mouseCode |= 1;
				}
				else
				{
					mouseCode &= 0xFFFFFFFE;
				}
				break;
			case 2:
				if (ev.type == SDL_MOUSEBUTTONDOWN)
				{
					mouseCode |= 4;
				}
				else
				{
					mouseCode &= 0xFFFFFFFB;
				}
				break;
			case 3:
				if (ev.type == SDL_MOUSEBUTTONDOWN)
				{
					mouseCode |= 2;
				}
				else
				{
					mouseCode &= 0xFFFFFFFD;
				}
				break;
			case 4:
			case 6:
			case 8:
				if (ev.type == SDL_MOUSEBUTTONDOWN)
				{
					mouseCode |= 8;
				}
				else
				{
					mouseCode &= 0xFFFFFFF7;
				}
				break;
			case 5:
			case 7:
			case 9:
				if (ev.type == SDL_MOUSEBUTTONDOWN)
				{
					mouseCode |= 0x10;
				}
				else
				{
					mouseCode &= 0xFFFFFFEF;
				}
				break;
			default:
				break;
			}

			eng->TrapMouse_Event(mouseCode, ev.type == SDL_MOUSEBUTTONDOWN);
		}

		else if (ev.type == SDL_MOUSEMOTION && !(cl_mousegrab.value == 0.0 && (!m_bActiveApp)) && !(m_rawinput.value != 0.0 || !m_bActiveApp))
		{
			if (!g_BaseUISurface.IsCursorVisible() && !s_bCursorVisible && BUsesSDLInput())
			{
				if (m_bExpectSyntheticMouseMotion
					&& ev.motion.x == m_nMouseTargetX && ev.motion.y == m_nMouseTargetY)
				{
					m_bExpectSyntheticMouseMotion = 0;
					if (m_rawinput.value == 0.0)
						SDL_GetRelativeMouseState(&deltaX, &deltaY);

					if (SDL_PollEvent(&ev) <= 0)
						return;

					continue;
				}
				if (!m_bCursorVisible)
				{
					if ((ev.window.data2 < m_nMouseTargetX - m_nWarpDelta || ev.window.data2 > m_nMouseTargetX + m_nWarpDelta)
						|| (!(ev.motion.y >= m_nMouseTargetY - m_nWarpDelta && ev.motion.y <= m_nMouseTargetY + m_nWarpDelta)))
					{
						SDL_WarpMouseInWindow(m_hSDLWindow, m_nMouseTargetX, m_nMouseTargetY);
						m_bExpectSyntheticMouseMotion = 1;
					}
				}
			}
		}

		VGui_CallEngineSurfaceAppHandler(&ev, 0);

		if (SDL_PollEvent(&ev) <= 0)
			return;
	}
#endif
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
	if (x)
		*x = m_x;
	if (y)
		*y = m_y;
	if (w)
		*w = m_width;
	if (h)
		*h = m_height;
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
	m_bCursorVisible = bState;
	if (!bState && cl_mousegrab.value == 0.0)
		SDL_SetWindowGrab(m_hSDLWindow, SDL_FALSE);
}

void CGame::SetActiveApp(bool active)
{
	m_bActiveApp = active;
}

void CGame::AppActivate(bool fActive)
{
	Call_Method<void, CGame, bool>(0xB0960, this,fActive);
	return;
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

bool BUsesSDLInput()
{
	static bool s_bUseRawInput;

	s_bUseRawInput = false;

	if (BIsValveGame() ||
		Q_strstr(g_szfullClientName, "valve/cl_dlls/client") ||
		Q_strstr(g_szfullClientName, "cstrike/cl_dlls/client") ||
		Q_strstr(g_szfullClientName, "tfc/cl_dlls/client") ||
		Q_strstr(g_szfullClientName, "dod/cl_dlls/client") ||
		Q_strstr(g_szfullClientName, "gearbox/cl_dlls/client") ||
		Q_strstr(g_szfullClientName, "bshift/cl_dlls/client"))
	{
		s_bUseRawInput = true;
	}

	return s_bUseRawInput;
}