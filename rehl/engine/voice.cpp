#include "precompiled.h"

IMixerControls* g_pMixerControls;
IVoiceRecord* g_pVoiceRecord;

IVoiceTweak g_VoiceTweakAPI =
{
	&VoiceTweak_StartVoiceTweakMode,
	&VoiceTweak_EndVoiceTweakMode,
	&VoiceTweak_SetControlFloat,
	&VoiceTweak_GetControlFloat,
	&VoiceTweak_GetSpeakingVolume
};

bool g_bUsingSteamVoice = true;
bool g_bVoiceAtLeastPartiallyInitted;

bool VoiceSE_Init();

void  VoiceSE_RegisterCvars(void);

cvar_t voice_loopback = { "voice_loopback", "0", NULL,  0.0f, NULL };

cvar_t voice_scale = { "voice_scale", "1", FCVAR_ARCHIVE, 0.0f, NULL };

cvar_t voice_maxgain = { "voice_maxgain", "5", NULL, 0.0f, NULL };

cvar_t voice_avggain = { "voice_avggain", "5", NULL, 0.0f, NULL };

cvar_t voice_fadeouttime = { "voice_fadeouttime", "0.1", NULL, 0.0f, NULL };

cvar_t voice_profile = { "voice_profile", "0", NULL, 0.0f, NULL };

cvar_t voice_showchannels = { "voice_showchannels", "0", NULL, 0.0f, NULL };

cvar_t voice_showincoming = { "voice_showincoming", "0", NULL, 0.0f, NULL };

cvar_t voice_enable = { "voice_enable", "1", FCVAR_ARCHIVE | FCVAR_USERINFO,0.0f, NULL };

cvar_t voice_dsound = { "voice_dsound", "1", NULL, 0.0f, NULL };

cvar_t voice_forcemicrecord = { "voice_forcemicrecord", "1", FCVAR_ARCHIVE, 0.0f, NULL };

cvar_t voice_overdrive = { "voice_overdrive", "2", NULL, 0.0f, NULL };

cvar_t voice_overdrivefadetime = { "voice_overdrivefadetime", "0.4", NULL, 0.0f, NULL };

void Voice_RegisterCvars()
{
	Cvar_RegisterVariable(&voice_loopback);
	Cvar_RegisterVariable(&voice_maxgain);
	Cvar_RegisterVariable(&voice_avggain);
	Cvar_RegisterVariable(&voice_scale);
	Cvar_RegisterVariable(&voice_fadeouttime);
	Cvar_RegisterVariable(&voice_profile);
	Cvar_RegisterVariable(&voice_showchannels);
	Cvar_RegisterVariable(&voice_showincoming);
	Cvar_RegisterVariable(&voice_enable);
	Cvar_RegisterVariable(&voice_dsound);
	Cvar_RegisterVariable(&voice_forcemicrecord);
	VoiceSE_RegisterCvars();
}

bool Voice_Init(const char* pCodecName, int quality)
{
	if (voice_enable.value == 0.0)
		return false;
	
	if (g_bVoiceAtLeastPartiallyInitted)
		Voice_Deinit();

	g_bVoiceAtLeastPartiallyInitted = true;

	if (!VoiceSE_Init())
		return false;

	bool steamUser = SteamUser();

	g_bUsingSteamVoice = steamUser;

	if (steamUser || pCodecName[0] == '\0')
	{
		g_pMixerControls = GetMixerControls();
		float forcemicrecord = voice_forcemicrecord.value;
		
		if (forcemicrecord != 0.0 && g_pMixerControls)
			g_pMixerControls->SelectMicrophoneForWaveInput();

		return true;
	}

	NOT_IMPLEMENTED;

	return true;
}

void Voice_Deinit()
{
	NOT_IMPLEMENTED;
}

int VoiceTweak_StartVoiceTweakMode()
{
	NOT_IMPLEMENTED;
	return false;
}

void VoiceTweak_EndVoiceTweakMode()
{
	NOT_IMPLEMENTED;
}

void VoiceTweak_SetControlFloat(VoiceTweakControl iControl, float value)
{
	NOT_IMPLEMENTED;
}

float VoiceTweak_GetControlFloat(VoiceTweakControl iControl)
{
	NOT_IMPLEMENTED;
	return 0;
}

int VoiceTweak_GetSpeakingVolume()
{
	NOT_IMPLEMENTED;
	return 0;
}


/* WARNING: Removing unreachable block (ram,0x00204b00) */
/* WARNING: Removing unreachable block (ram,0x00204b90) */

int Voice_AddIncomingData(int nChannel, char *pchData, int nCount, int iSequenceNumber)
{
	NOT_IMPLEMENTED;
	return 0;
}

bool VoiceSE_Init()
{
	NOT_IMPLEMENTED;
	return false;
}

void VoiceSE_RegisterCvars()
{
	Cvar_RegisterVariable(&voice_overdrive);
	Cvar_RegisterVariable(&voice_overdrivefadetime);
}