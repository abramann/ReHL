#pragma once

class IMixerControls
{
public:
	enum Control
	{
		MicBoost,
		MicVolume,
		MicMute,
		NumControls,
	};

	virtual ~IMixerControls() {};

	virtual void Release() = 0;

	virtual bool GetValue_Float(Control iControl, float *const value) = 0;

	virtual bool SetValue_Float(Control iControl, float value) = 0;

	virtual bool SelectMicrophoneForWaveInput() = 0;

	virtual const char *GetMixerName() = 0;
};

IMixerControls* GetMixerControls();