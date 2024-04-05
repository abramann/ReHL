#include "precompiled.h"
#include "snd_mix.h"


char rgrgvoxword[18432];

void VOX_Init()
{
	Q_memset(rgrgvoxword, 0, 18432);
	VOX_ReadSentenceFile();
}

void VOX_ReadSentenceFile()
{
	NOT_IMPLEMENTED_IGNORE;
}