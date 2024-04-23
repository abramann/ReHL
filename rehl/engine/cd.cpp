#include "precompiled.h"
#include "cd.h"
#include "cd_internal.h"

static int s_careerAudioPaused = 0;

static CCDAudio g_CDAudio;

ICDAudio* cdaudio = &g_CDAudio;

CCDAudio* GetInteralCDAudio(void)
{
	return &g_CDAudio;
}

void CD_Command_f(void)
{
	NOT_IMPLEMENTED;
}

void MP3_Command_f(void)
{

}

void CareerAudio_Command_f(void)
{
	s_careerAudioPaused = 0;
}

void PrimeMusicStream(char * filename, int looping)
{
	NOT_IMPLEMENTED;
}

CCDAudio::CCDAudio(void)
{
	// TODO: implement - ScriptedSnark
}

CCDAudio::~CCDAudio(void)
{
}

void CCDAudio::ResetCDTimes(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::_Stop(int, int)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::_Pause(int, int)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::_Eject(int, int)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::_CloseDoor(int, int)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::_GetAudioDiskInfo(int, int)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::_Play(int track, int looping)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::_PrimeTrack(int track, int looping)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::_Resume(int, int)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::_SwitchToLauncher(int, int)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::_SwitchToEngine(int, int)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::CD_f(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::_CDUpdate(int, int)
{
	// TODO: implement - ScriptedSnark
}

int CCDAudio::Init(void)
{
	// TODO: implement - ScriptedSnark
	return 0;
}

void CCDAudio::_Init(int, int)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::Shutdown(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::_CDReset(int, int)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::Reset(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::Stop(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::Pause(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::Eject(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::CloseDoor(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::GetAudioDiskInfo(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::Play(int track, bool looping)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::PlayFile(const char* filename, bool looping)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::PrimeTrack(char* filename, int looping)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::Resume(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::SwitchToLauncher(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::SwitchToEngine(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::FadeOut(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::Frame(void)
{
	// TODO: implement - ScriptedSnark
}

void CDAudio_Init(void)
{
	thread->Init();
}

void CDAudio_Shutdown(void)
{
	thread->Shutdown();
}

void CDAudio_Play(int track, int looping)
{
	// TODO: implement - ScriptedSnark
}

void CDAudio_Pause(void)
{
	// TODO: implement - ScriptedSnark
}

void CDAudio_Resume(void)
{
	// TODO: implement - ScriptedSnark
}

void MP3_Resume_Audio(void)
{
	// TODO: implement - ScriptedSnark
}

void MP3_Suspend_Audio(void)
{
	// TODO: implement - ScriptedSnark
}

bool CCDAudio::MP3_PlayTrack(int trackNum, bool looping)
{
	// TODO: implement - ScriptedSnark
	return false;
}

bool CCDAudio::MP3_PlayTrack(const char* filename, bool looping)
{
	// TODO: implement - ScriptedSnark
	return false;
}

void CCDAudio::MP3_PlayTrackFinalize(int trackNum, bool looping)
{
	// TODO: implement - ScriptedSnark
}

bool CCDAudio::MP3_Init(void)
{
	// TODO: implement - ScriptedSnark
	return false;
}

void CCDAudio::MP3_Shutdown(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::MP3_Resume_Audio(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::MP3_Suspend_Audio(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::MP3_ReleaseDriver(void)
{
	NOT_IMPLEMENTED;

	/*
	if (MP3digitalDriver)
	{
		AIL_close_digital_driver(MP3digitalDriver);
		MP3digitalDriver = nullptr; // 0
	}
	*/
}

float CCDAudio::MP3_SetVolume(float NewVol)
{
	// TODO: implement - ScriptedSnark
	return 1;
}

bool CCDAudio::MP3_InitStream(int trackNum, bool looping)
{
	// TODO: implement - ScriptedSnark
	return false;
}

bool CCDAudio::MP3_InitStream(const char* filename, bool looping)
{
	// TODO: implement - ScriptedSnark
	return false;
}

void CCDAudio::MP3_StartStream(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::MP3_StopStream(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::MP3_Loop(void)
{
	// TODO: implement - ScriptedSnark
}

void CCDAudio::MP3_SetPause(bool OnOff)
{
	// TODO: implement - ScriptedSnark
}