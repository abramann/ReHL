#include "precompiled.h"
#include "console.h"


#ifdef SHARED_GAME_DATA
char **sp_Con_text = ADDRESS_OF_DATA(char**, 0x2C837);
char *&con_text = *sp_Con_text;

int * sp_con_linewidth = ADDRESS_OF_DATA(int *, 0x2C7A6);
int & con_linewidth = *sp_con_linewidth;

void ** sp_con_notifypos = ADDRESS_OF_DATA(void **, 0x2C9A6);
void *& con_notifypos = *sp_con_notifypos;

int * sp_con_num_times = ADDRESS_OF_DATA(int *, 0x2C9B2);
int & con_num_times = *sp_con_num_times;

float** sp_con_times = ADDRESS_OF_DATA(float**, 0x2CC6A);
float*& con_times = *sp_con_times;

int* sp_con_totallines = ADDRESS_OF_DATA(int*, 0x2C8B9);
int& con_totallines = *sp_con_totallines;

int* sp_con_current = ADDRESS_OF_DATA(int*, 0x2C8C9);
int con_current = *sp_con_current;

int* sp_con_backscroll = ADDRESS_OF_DATA(int*, 0x2C8BF);
int& con_backscroll = *sp_con_backscroll;


cvar_t* sp_con_fastmode = ADDRESS_OF_DATA(cvar_t*, 0x2C9D4);
cvar_t& con_fastmode = *sp_con_fastmode;

cvar_t* sp_con_notifytime = ADDRESS_OF_DATA(cvar_t*, 0x2C9DE);
cvar_t& con_notifytime = *sp_con_notifytime;


cvar_t* sp_con_color = ADDRESS_OF_DATA(cvar_t*, 0x2C9E8);
cvar_t& con_color = *sp_con_color;

cvar_t* sp_con_shifttoggleconsole = ADDRESS_OF_DATA(cvar_t*, 0x2C9F2);
cvar_t& con_shifttoggleconsole = *sp_con_shifttoggleconsole;

cvar_t* sp_con_mono = ADDRESS_OF_DATA(cvar_t*, 0x2C9FC);
cvar_t& con_mono = *sp_con_mono;

qboolean* sp_message_type_is_privileged = ADDRESS_OF_DATA(qboolean*, 0x2C719);
qboolean& message_type_is_privileged = *sp_message_type_is_privileged;
#else
char* con_text;
int con_linewidth = 1;
void *con_notifypos;
int con_num_times = 4;
float *con_times;
int con_totallines;
int con_current = 0;
int con_backscroll;

cvar_t con_fastmode = { "con_fastmode", "1" };
cvar_t con_notifytime = { "con_notifytime", "4" };
cvar_t con_color = { "con_color", "255 180 30", FCVAR_ARCHIVE };
cvar_t con_shifttoggleconsole = { "con_shifttoggleconsole", "0" };
cvar_t con_mono = { "con_mono", "0", FCVAR_ARCHIVE };

qboolean message_type_is_privileged = true;
#endif
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
