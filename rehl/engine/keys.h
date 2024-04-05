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

#include "FileSystem.h"
#include <SDL.h>

#define K_TAB 9 
#define K_ENTER 13 
#define K_ESCAPE 27
#define K_SPACE 32 
#define K_BACKSPACE 127 
#define K_UPARROW 128 
#define K_DOWNARROW 129 
#define K_LEFTARROW 130 
#define K_RIGHTARROW 131 
#define K_ALT 132 
#define K_CTRL 133 
#define K_SHIFT 134 
#define K_F1 135 
#define K_F2 136 
#define K_F3 137 
#define K_F4 138 
#define K_F5 139 
#define K_F6 140 
#define K_F7 141 
#define K_F8 142 
#define K_F9 143 
#define K_F10 144 
#define K_F11 145 
#define K_F12 146 
#define K_INS 147 
#define K_DEL 148
#define K_PGDN 149
#define K_PGUP 150 
#define K_HOME 151 
#define K_END 152 
#define K_MOUSE1 241 
#define K_MOUSE2 242 
#define K_MOUSE3 243 
#define K_MOUSE4 244 
#define K_MOUSE5 245 
#define K_JOY1 203 
#define K_JOY2 204 
#define K_JOY3 205 
#define K_JOY4 206 
#define K_AUX1 207 
#define K_AUX2 208 
#define K_AUX3 209 
#define K_AUX4 210 
#define K_AUX5 211 
#define K_AUX6 212 
#define K_AUX7 213 
#define K_AUX8 214 
#define K_AUX9 215 
#define K_AUX10 216 
#define K_AUX11 217 
#define K_AUX12 218 
#define K_AUX13 219 
#define K_AUX14 220 
#define K_AUX15 221 
#define K_AUX16 222
#define K_AUX17 223
#define K_AUX18 224
#define K_AUX19 225
#define K_AUX20 226
#define K_AUX21 227
#define K_AUX22 228
#define K_AUX23 229
#define K_AUX24 230
#define K_AUX25 231
#define K_AUX26 232
#define K_AUX27 233
#define K_AUX28 234
#define K_AUX29 235
#define K_AUX30 236
#define K_AUX31 237
#define K_AUX32 238
#define K_KP_HOME 160
#define K_KP_UPARROW 161
#define K_KP_PGUP 162
#define K_KP_LEFTARROW 163
#define K_KP_5 164
#define K_KP_RIGHTARROW 165
#define K_KP_END 166
#define K_KP_DOWNARROW 167
#define K_KP_PGDN 168
#define K_KP_ENTER 169
#define K_KP_INS 170
#define K_KP_DEL 171
#define K_KP_SLASH 172
#define K_KP_MINUS 173
#define K_KP_PLUS 174
#define K_CAPSLOCK 175
#define K_MWHEELUP 240
#define K_MWHEELDOWN 239
#define K_PAUSE 255
#define K_SEMICOLON 59 
#define K_KP_MUL 176 
#define K_WIN 177 

typedef enum {
	key_game = 0,
	key_message = 1,
	key_menu = 2,
} keydest_t;

const size_t MAX_CHAT_BUFFER = 120;

extern keydest_t key_dest;
extern char* keybindings[256];
extern int key_repeats[256];

extern int toggleconsole_key;

extern char chat_buffer[MAX_CHAT_BUFFER];
extern int chat_bufferlen;
extern char message_type[32];

extern SDL_Keycode GetSDLKeycodeFromEngineKey(int iKey);
extern int GetEngineKeyFromSDLScancode(SDL_Scancode code);

extern void Key_Event(int key, bool down);
extern void Key_Init();
extern void Key_Shutdown();
extern bool CheckForCommand();
extern void CompleteCommand();
extern int Key_CountBindings();
extern void Key_WriteBindings(FileHandle_t f);
extern int Key_StringToKeynum(const char* str);
extern const char* Key_KeynumToString(int keynum);
extern void Key_SetBinding(int keynum, const char* binding);
extern const char* Key_NameForBinding(const char* pBinding);
extern void Key_EventStub(int key, bool down);
extern void Key_ClearStates();
extern const char* Key_BindingForKey(int keynum);