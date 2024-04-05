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

#pragma once

#include "maintypes.h"
#include "igame.h"
#include <SDL.h>


class CGame : public IGame
{
public:
	CGame();
	virtual ~CGame();

	NOBODY virtual bool Init(void *pvInstance);
	virtual bool Shutdown();
	virtual bool CreateGameWindow();
	virtual void SleepUntilInput(int time);
	virtual SDL_Window* GetMainWindow();
	virtual SDL_Window** GetMainWindowAddress();
	virtual void SetWindowXY(int x, int y);
	virtual void SetWindowSize(int w, int h);
	virtual void GetWindowRect(int *x, int *y, int *w, int *h);
	virtual bool IsActiveApp();
	virtual bool IsMultiplayer();
	NOBODY virtual void PlayStartupVideos();
	NOBODY virtual void PlayAVIAndWait(const char *aviFile);
	virtual void SetCursorVisible(bool bState);

	void SetActiveApp(bool active);
	void AppActivate(bool fActive);

private:
	bool m_bActiveApp = true;

	SDL_Window* m_hSDLWindow = nullptr;
	SDL_GLContext m_hSDLGLContext = nullptr;

	bool m_bExpectSyntheticMouseMotion = false;
	int m_nMouseTargetX = 0;
	int m_nMouseTargetY = 0;

	int m_nWarpDelta = 0;

	bool m_bCursorVisible = true;

	int m_x = 0;
	int m_y = 0;

	int m_width = 0;
	int m_height = 0;

	bool m_bMultiplayer = false;
};