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

typedef int BOOL;

// user message
#define MAX_USER_MSG_DATA 192

//moved to com_model.h
//typedef struct cache_user_s
//{
//	void *data;
//} cache_user_t;

typedef int (*pfnUserMsgHook)(const char *, int, void *);

/**
*	the host system specifies the base of the directory tree, the
*	command line parms passed to the program, and the amount of memory
*	available for the program to use
*/

/**
*	true if into command execution
*/

/**
*	not bounded in any way, changed at
*	start of every frame, never reset
*/

/**
*	Loads the server dll if needed.
*/
void Host_InitializeGameDLL();

void Host_ClearSaveDirectory();

// PERFORMANCE INFO
#define MIN_FPS 0.5	  // Host minimum fps value for maxfps.
#define MAX_FPS 100.0 // Upper limit for maxfps.

#define MAX_FRAMETIME 0.25
#define MIN_FRAMETIME 0.001

//
// per-level limits
//
#define MAX_EDICTS 900 // FIXME: ouch! ouch! ouch!
#define MAX_LIGHTSTYLES 64
#define MAX_MODELS 512 // these are sent over the net as bytes
#define MAX_SOUNDS 512 // so they cannot be blindly increased
#define MAX_USER_MSG_DATA 192
#define MAX_EVENTS 1024

#define MAX_USERMSGS 128 // TODO: ???

#define SAVEGAME_COMMENT_LENGTH 39

#define MAX_STYLESTRING 64

#undef LittleLong
#undef LittleShort
#undef LittleFloat
#undef LittleDword
#undef BigLong
#undef BigShort
#undef BigFloat
#undef BigDword

//typedef int SOCKET;
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_FIONBIO(s, m) ioctl(s, FIONBIO, (char*)&m)
#define SOCKET_MSGLEN(s, r) ioctl(s, FIONREAD, (char*)&r)
#define SIN_GET_ADDR(saddr, r) r = (saddr)->s_addr
#define SIN_SET_ADDR(saddr, r) (saddr)->s_addr = (r)
#define SOCKET_CLOSE(s) close(s)
#define SOCKET_AGAIN() (errno == EAGAIN)
//#define SOCKET_ERROR -1

//#define WSAENOPROTOOPT ENOPROTOOPT
