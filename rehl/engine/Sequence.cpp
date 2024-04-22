#include "precompiled.h"


char g_sequenceParseFileName[256];
bool  g_sequenceParseFileIsGlobal;

sequenceCommandLine_s g_fileScopeDefaults;
sequenceCommandLine_s* g_blockScopeDefaults;

sequenceEntry_s* g_sequenceList;

int g_lineNum;
char* g_scan;
char* g_lineScan;

sequenceEntry_s* SequenceGet(const char* fileName, const char* entryName)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

void Sequence_ParseBuffer(byte *buffer, int bufferSize);

void Sequence_StripComments(char *buffer, int *pBufSize);

void Sequence_ResetDefaults(sequenceCommandLine_s *destination, sequenceCommandLine_s *source);

char Sequence_ParseModifier(sequenceCommandLine_s *defaults);

char Sequence_ParseEntry();

void Sequence_GetNameValueString(char *token, int tokenLen);

qboolean Sequence_ParseSentenceLine();

qboolean Sequence_ExpandGosubsForEntry(sequenceEntry_s *entry);

void Sequence_WriteDefaults(sequenceCommandLine_s *source, sequenceCommandLine_s *destination);

void Sequence_BakeDefaults(sequenceCommandLine_s *destination, sequenceCommandLine_s *source);

void Sequence_ParseFile(const char * fileName, qboolean isGlobal)
{
	char filePathName[256];
	Q_strncpy(g_sequenceParseFileName, fileName, 255);

	g_sequenceParseFileName[255] = 0;
	g_sequenceParseFileIsGlobal = isGlobal;

	snprintf(filePathName, 255, "sequences/%s.seq", fileName);

	int bufSize;
	auto buf = COM_LoadTempFile(filePathName, &bufSize);

	if (buf)
		Sequence_ParseBuffer(buf, bufSize);
	else
		Con_DPrintf("WARNING: failed to locate sequence file %s\n", fileName);
}

sentenceEntry_s* SequencePickSentence(const char* groupName, int pickMethod, int* picked)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

void Sequence_Init()
{
	Sequence_ParseFile("global", true);
}

void Sequence_ParseBuffer(byte * buffer, int bufferSize)
{
	NOT_IMPLEMENTED;
}

void Sequence_StripComments(char *buffer, int *pBufSize)
{
	NOT_IMPLEMENTED;
}

void Sequence_ResetDefaults(sequenceCommandLine_s *destination, sequenceCommandLine_s *source)
{
	NOT_IMPLEMENTED;
}

char Sequence_ParseModifier(sequenceCommandLine_s *defaults)
{
	NOT_IMPLEMENTED;
	return 0;
}

char Sequence_ParseEntry()
{
	NOT_IMPLEMENTED;
	return 0;
}

void Sequence_GetNameValueString(char * token, int tokenLen)
{
	NOT_IMPLEMENTED;
}

qboolean Sequence_ParseSentenceLine()
{
	NOT_IMPLEMENTED;
	return qboolean();
}

qboolean Sequence_ExpandGosubsForEntry(sequenceEntry_s * entry)
{
	NOT_IMPLEMENTED;
	return qboolean();
}

void Sequence_WriteDefaults(sequenceCommandLine_s * source, sequenceCommandLine_s * destination)
{
	NOT_IMPLEMENTED;
}

void Sequence_BakeDefaults(sequenceCommandLine_s * destination, sequenceCommandLine_s * source)
{
	NOT_IMPLEMENTED;
}
