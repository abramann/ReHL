#pragma once

class IVoiceRecord
{
public:
	~IVoiceRecord() {};

	virtual void Release() = 0;
	virtual bool RecordStart() = 0;
	virtual void RecordStop() = 0;
	virtual void Idle() = 0;
	virtual int GetRecordedData(short *pOut, int nSamples) = 0;
	virtual bool Init(int sampleRate) = 0;
};

IVoiceRecord * CreateVoiceRecord(int sampleRate);
