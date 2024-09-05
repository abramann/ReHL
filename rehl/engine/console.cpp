#include "precompiled.h"
#include "console.h"

VAR(char*, con_text, 0x2C837, nullptr);
VVAR(int, con_linewidth, 0x2C7A6, 1);
VVAR(void *, con_notifypos, 0x2C9A6, nullptr);
VVAR(int, con_num_times, 0x2C9B2, 4);
VVAR(float *, con_times, 0x2CC6A, nullptr);
VAR(int, con_totallines, 0x2C8B9);
VVAR(int, con_current, 0x2C8C9, 0);
VAR(int, con_backscroll, 0x2C8BF);
VVAR(cvar_t, con_fastmode, 0x2C9D4, { "con_fastmode" COMMA "1" });
VVAR(cvar_t, con_notifytime, 0x2C9DE, { "con_notifytime" COMMA "4" } );
VVAR(cvar_t, con_color, 0x2C9E8, { "con_color" COMMA "255 180 30" COMMA FCVAR_ARCHIVE });
VAR(cvar_t, con_shifttoggleconsole, 0x2C9F2, { "con_shifttoggleconsole" COMMA "0" });
VVAR(cvar_t, con_mono, 0x2C9FC, { "con_mono" COMMA "0" COMMA FCVAR_ARCHIVE });
VVAR(qboolean, message_type_is_privileged, 0x2C719, true);

bool con_forcedup = false; // because no entities to refresh


void Con_CheckResize()
{
	int charWidth = 8;
	int newwidth = (vid.width / charWidth) - 2;
	int oldwidth = con_linewidth;

	if (newwidth == oldwidth)
		return;

	if (newwidth <= 0)
	{
		con_linewidth = 38;
		con_totallines = 431;
		Q_memset(con_text, ' ', 0x4000);
	}
	else
	{
		int newtotallines = 0x4000 / newwidth;
		con_linewidth = newwidth;

		int oldtotal = con_totallines;

		con_totallines = newtotallines;

		if (newwidth > oldwidth)
			newwidth = oldwidth;

		char tbuf[16384];

		Q_memcpy(tbuf, con_text, 0x4000);
		Q_memset(con_text, ' ', 0x4000);
		
		for (int i = 0; i < newtotallines; i++)
		{
			for (int j = 0; j < newwidth; j++)
			{
				con_text[con_linewidth * (con_totallines + ~i)] = tbuf[j - 0x4000 + oldwidth * ((con_current + oldtotal - i) % oldtotal)];;
			}
		}

		Con_ClearNotify();
	}
	
	con_backscroll = 0;
	con_current = con_totallines - 1;
}

void EXT_FUNC Con_NPrintf(int idx, const char * fmt, ...)
{
	NOT_IMPLEMENTED;
	return void EXT_FUNC();
}

void Con_Shutdown(void)
{
	if (con_times)
		Mem_Free(con_times);
	if (con_notifypos)
		Mem_Free(con_notifypos);

	con_times = 0;
	con_notifypos = 0;
	con_initialized = false;
}

void Con_ClearNotify()
{
	if (!con_times)
		return;

	for (int i = 0; i < con_num_times; i++)
		con_times[i] = 0;

	*g_szNotifyAreaString = '\0';
}

void Con_ToggleConsole_f()
{
	if (VGuiWrap2_IsConsoleVisible())
		VGuiWrap2_HideConsole();
	else
		VGuiWrap2_ShowConsole();
}

void Con_SetTimes_f()
{
	if (Cmd_Argc() != 2)
	{
		Con_Printf("contimes <n>\nShow <n> overlay lines [4-64].\n%i current overlay lines.\n", con_num_times);
	}
	else
	{
		int numtimes = Q_atoi(Cmd_Argv(1));
		int size;
		if (numtimes <= 3)
		{
			numtimes = 4;
			size = 16;
		}
		else if (numtimes <= 64)
		{
			size = sizeof(float) * numtimes;
		}
		else
		{
			size = 256;
			numtimes = size / 4;
		}
		
		if (con_times)
			Mem_Free(con_times);
		if (con_notifypos)
			Mem_Free(con_notifypos);

		con_times = (float*)Mem_Malloc(size);
		con_notifypos = Mem_Malloc(size);

		if (!con_times || !con_notifypos)
			Sys_Error("Couldn't allocate space for %i console overlays.", numtimes);

		con_num_times = numtimes;
	}
}

void Con_HideConsole_f()
{
	VGuiWrap2_HideConsole();
}

void Con_MessageMode_f()
{
	if (!VGuiWrap2_IsInCareerMatch())
	{
		key_dest = key_message;
		message_type_is_privileged = Cmd_CurrentCommandIsPrivileged();
		if (Cmd_Argc() == 2)
		{
			Q_strncpy(message_type, Cmd_Argv(1), 31);
			message_type[31] = 0;
		}
		else
		{
			Q_strcpy(message_type, "say");
		}
	}
}

void Con_MessageMode2_f()
{
	if (!VGuiWrap2_IsInCareerMatch())
	{
		key_dest = key_message;
		message_type_is_privileged = Cmd_CurrentCommandIsPrivileged();
		Q_strcpy(message_type, "say_team");
	}
}

void Con_Clear_f()
{
	if (con_text)
		Q_memset(con_text, ' ', 0x4000);
	
	VGuiWrap2_ClearConsole();
}

void Con_DrawNotify()
{
	NOT_IMPLEMENTED;
}

int Con_IsVisible()
{
	NOT_IMPLEMENTED;
	return 0;
}
