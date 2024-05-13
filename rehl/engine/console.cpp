#include "precompiled.h"
#include "console.h"


bool con_forcedup = false; // because no entities to refresh

cvar_t con_fastmode = { "con_fastmode", "1" };
cvar_t con_notifytime = { "con_notifytime", "4" };
cvar_t con_color = { "con_color", "255 180 30", FCVAR_ARCHIVE };
cvar_t con_mono = { "con_mono", "0", FCVAR_ARCHIVE };

int con_linewidth = 1;
int con_num_times = 4;
int con_totallines;
void *con_notifypos;
char *con_text;
int con_current = 0;
char g_szNotifyAreaString[256];
int con_backscroll;

cvar_t con_shifttoggleconsole = { "con_shifttoggleconsole", "0" };

float *con_times;

void Con_CheckResize()
{
	int newwidth = (vid.width >> 3) - 2;
	int oldwidth = con_linewidth;

	if (newwidth == oldwidth)
		return;

	if (newwidth <= 0)
	{
		con_linewidth = 38;
		con_totallines = 431;
		Q_memset(con_text, 32, 0x4000);
	}
	else
	{
		int newtotal = 0x4000 / newwidth;
		con_linewidth = newwidth;
		if (newtotal > con_totallines)
			newtotal = con_totallines;

		int oldtotal = con_totallines;

		con_totallines = newtotal;

		int numlines = newtotal;
		if (newwidth > oldwidth)
			newwidth = oldwidth;

		char tbuf[16384];

		Q_memcpy(tbuf, con_text, 0x4000);
		Q_memset(con_text, 32, 0x4000);
		
		for (int i = 0; i < numlines; i++)
		{
			for (int j = 0; j < newwidth; j++)
			{
				con_text[con_linewidth * (con_totallines + ~i)] = tbuf[j - 0x4000 + oldwidth * ((con_current + oldtotal - i) % oldtotal)];;
			}
		}
		
		if (con_times)
		{
			for (int i = 0; i < con_num_times; i++)
			{
				con_times[i] = 0;
			}
			*g_szNotifyAreaString = '\0';
		}
	}
	
	con_backscroll = 0;
	con_current = con_totallines - 1;
}

void Con_ToggleConsole_f()
{
	NOT_IMPLEMENTED;
}

void Con_SetTimes_f()
{
	NOT_IMPLEMENTED;
}

void Con_HideConsole_f()
{
	NOT_IMPLEMENTED;
}

void Con_MessageMode_f()
{
	NOT_IMPLEMENTED;
}

void Con_MessageMode2_f()
{
	NOT_IMPLEMENTED;
}

void Con_Clear_f()
{
	NOT_IMPLEMENTED;
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
