#include "precompiled.h"

#ifdef _WIN32
#include <MMSystem.h>
#endif

class CMixerControls : public  IMixerControls
{
public:

	~CMixerControls();

	void Release() override;

	bool GetValue_Float(Control iControl, float *const value) override;

	bool SetValue_Float(Control iControl, float value) override;

	bool SelectMicrophoneForWaveInput() override;

	const char *GetMixerName() override;

private:
	HMIXEROBJ m_hMixerObj;
	DWORD m_controlID;
	DWORD m_cMultipleItems;

};

CMixerControls::~CMixerControls()
{
}

void CMixerControls::Release()
{
#ifndef _WIN32
	NOT_IMPLEMENTED;
#endif
}

bool CMixerControls::GetValue_Float(Control iControl, float * const value)
{
#ifndef _WIN32
	return false;
#else
	NOT_IMPLEMENTED;
	return false;
#endif
}

bool CMixerControls::SetValue_Float(Control iControl, float value)
{
#ifndef _WIN32
	return false;
#else
	NOT_IMPLEMENTED;
	return false;
#endif
}

bool CMixerControls::SelectMicrophoneForWaveInput()
{
#ifndef _WIN32
	return false;
#else
	
	NOT_IMPLEMENTED;

	/*
	DWORD paDetails[3];
	MIXERCONTROLDETAILS pmxcd;
	
	if (m_controlID == -1)
		return false;

	int size = 4 * m_cMultipleItems + 3;
	//LOBYTE(size) = size & 0xFC;
	//v4 = alloca(v3);

	pmxcd.dwControlID = m_controlID;
	pmxcd.cbStruct = 24;
	pmxcd.cChannels = 1;
	pmxcd.cMultipleItems = m_cMultipleItems;
	pmxcd.cbDetails = 4;
	pmxcd.paDetails = paDetails;
	if (mixerGetControlDetailsA(m_hMixerObj, &pmxcd, MIXERCONTROL_CONTROLF_DISABLED))
		return false;

	if (this[4] == 1879113729)
		memset(paDetails, 0, 4 * m_cMultipleItems);

	v10[this[5]] = 1;

	pmxcd.dwControlID = m_controlID;
	pmxcd.cbStruct = 24;
	pmxcd.cChannels = 1;
	pmxcd.cMultipleItems = m_cMultipleItems;
	pmxcd.cbDetails = 4;
	pmxcd.paDetails = v10;
	if (mixerSetControlDetails(m_hMixerObj, &pmxcd, MIXERCONTROL_CONTROLF_DISABLED))
		return false;
		*/
	return true;
#endif
}

const char * CMixerControls::GetMixerName()
{
#ifndef _WIN32
	return "Linux";
#else
	NOT_IMPLEMENTED;
	return nullptr;
#endif
}

IMixerControls* GetMixerControls()
{
	return new CMixerControls();
}