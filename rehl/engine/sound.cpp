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

void S_Init()
{
	char *fmt;

	Con_DPrintf("Sound Initialization\n");
	VOX_Init();
	if (!COM_CheckParm("-nosound"))
	{
		NOT_IMPLEMENTED;

		/*
		if (COM_CheckParm("-simsound"))
			fakedma = true;
		Cmd_AddCommand("play", S_Play);
		Cmd_AddCommand("playvol", S_PlayVol);
		Cmd_AddCommand("speak", S_Say);
		Cmd_AddCommand((char *)off_2831A0, S_Say_Reliable);
		Cmd_AddCommand("stopsound", S_StopAllSoundsC);
		Cmd_AddCommand("soundlist", S_SoundList);
		Cmd_AddCommand("soundinfo", S_SoundInfo_f);
		Cvar_RegisterVariable(&s_show);
		Cvar_RegisterVariable(&nosound);
		Cvar_RegisterVariable(&volume);
		Cvar_RegisterVariable(&suitvolume);
		Cvar_RegisterVariable(&hisound);
		Cvar_RegisterVariable(&loadas8bit);
		Cvar_RegisterVariable(&bgmvolume);
		Cvar_RegisterVariable(&MP3Volume);
		Cvar_RegisterVariable(&MP3FadeTime);
		Cvar_RegisterVariable(&ambient_level);
		Cvar_RegisterVariable(&ambient_fade);
		Cvar_RegisterVariable(&snd_noextraupdate);
		Cvar_RegisterVariable(&snd_show);
		Cvar_RegisterVariable(&snd_mixahead);
		Cvar_RegisterVariable(&speak_enable);
		if (host_parms.memsize <= (int)&unk_7FFFFF)
		{
			Cvar_Set("loadas8bit", (char *)"1");
			Con_DPrintf("loading all sounds as 8bit\n");
		}
		snd_initialized = true;
		if (fakedma || SNDDMA_Init())
		{
			sound_started = 1;
		}
		else
		{
			Con_Printf("S_Startup: SNDDMA_Init failed.\n");
			sound_started = 0;
		}
		SND_InitScaletable();
		known_sfx = (sfx_t *)Hunk_AllocName(73728, (char *)"sfx_t");
		num_sfx = 0;
		if (fakedma)
		{
			v0 = (volatile dma_t *)Hunk_AllocName(44, (char *)off_2831DF);
			v0->splitbuffer = false;
			shm = v0;
			v0->samplebits = 16;
			v0->speed = 22050;
			v0->channels = 2;
			v0->samples = 0x8000;
			v0->samplepos = 0;
			v0->soundalive = true;
			v0->gamealive = true;
			v1 = shm;
			shm->submission_chunk = 1;
			v1->buffer = (unsigned __int8 *)Hunk_AllocName(0x10000, "shmbuf");
			Con_DPrintf("Sound sampling rate: %i\n", shm->speed);
		}
		if (sound_started)
			S_StopAllSounds_0((qboolean)fmt);
		SX_Init();
		Wavstream_Init();
		*/
	}
}

void S_AmbientOff() { NOT_IMPLEMENTED; }

void S_AmbientOn() { NOT_IMPLEMENTED; }

void S_Shutdown() { NOT_IMPLEMENTED; }

void S_TouchSound(char *sample) { NOT_IMPLEMENTED; }

void S_ClearBuffer() { NOT_IMPLEMENTED; }

void S_StartStaticSound(int entnum, int entchannel, sfx_t *sfx, vec_t *origin, float vol, float attenuation, int flags, int pitch) { NOT_IMPLEMENTED; }

void S_StartDynamicSound(int entnum, int entchannel, sfx_t *sfx, vec_t *origin, float fvol, float attenuation, int flags, int pitch) { NOT_IMPLEMENTED; }

void S_StopSound(int entnum, int entchannel) { NOT_IMPLEMENTED; }

sfx_t *S_PrecacheSound(char *sample) { return NULL; }

void S_ClearPrecache() { NOT_IMPLEMENTED; }

void S_Update(vec_t *origin, vec_t *v_forward, vec_t *v_right, vec_t *v_up) { NOT_IMPLEMENTED; }

void S_StopAllSounds(qboolean clear) { NOT_IMPLEMENTED; }

void S_BeginPrecaching() { NOT_IMPLEMENTED; }

void S_EndPrecaching() { NOT_IMPLEMENTED; }

void S_ExtraUpdate() 
{ 
	return Call_Function<void>(0x8CC80);
}

void S_LocalSound(char *s) { NOT_IMPLEMENTED; }

void S_BlockSound() { NOT_IMPLEMENTED; }

void S_PrintStats() { NOT_IMPLEMENTED; }

qboolean Voice_RecordStart(const char *pUncompressedFile, const char *pDecompressedFile, const char *pMicInputFile) { return FALSE; }

qboolean Voice_IsRecording() { return FALSE; }

void Voice_Idle(float frametime) { NOT_IMPLEMENTED; }

qboolean Voice_RecordStop() { return TRUE; }
