#include "precompiled.h"
#include "snd_mix.h"


char rgrgvoxword[18432] = { 0 };
char* rgpszrawsentence[CVOXFILESENTENCEMAX] = { 0 };

static int cszrawsentences = 0;
char szsentences[] = "sound/sentences.txt";

void VOX_Init()
{
	return Call_Function<void>(0x902C0);

	Q_memset(rgrgvoxword, 0, 18432);
	VOX_ReadSentenceFile();
}

void VOX_ReadSentenceFile()
{
	for (int i = 0; i < cszrawsentences; i++)
	{
		void* p = rgpszrawsentence[i];
		Mem_Free(p);

	}

	cszrawsentences = 0;

	int nFileLength;
	char* pBuf = (char*)COM_LoadFileForMe(szsentences, &nFileLength);
	if (!pBuf)
	{
		Con_DPrintf("VOX_ReadSentenceFile: Couldn't load %s\n", szsentences);
		return;
	}

	int nSentenceCount = 0;

	for (int i = 0; i < nFileLength; i++)
	{
		char* pBegin = 0;
		char* pszSentenceValue = nullptr;

		if (nSentenceCount > CVOXFILESENTENCEMAX - 1)
		{
			break;
		}
		if (pBuf[i] == '\r' || pBuf[i] == '\n' || pBuf[i] == ' ' || pBuf[i] == '\t')
		{
			continue;
		}

		if (pBuf[i] != '/')
		{
			pBegin = &pBuf[i];
			for (; i < nFileLength; i++)
			{
				if (pBuf[i] == ' ' || pBuf[i] == '\t')
				{
					pBuf[i++] = 0;
					break;
				}
				
			}
			pszSentenceValue = &pBuf[i];
		}
		for (;i < nFileLength; i++)
		{
			if (pBuf[i] == '\n' || pBuf[i] == '\r')
			{
				pBuf[i++] = 0;
				break;
			}
		}			

		if (pBegin)
		{
			int len = strlen(pszSentenceValue) + strlen(pBegin) + 1;
			char* pSentence = (char*)Mem_Malloc(len + 1);
			memcpy(pSentence, pBegin, len);
			pSentence[len] = '\0';
			rgpszrawsentence[nSentenceCount] = pSentence;
			nSentenceCount++;
		}
	}

	cszrawsentences = nSentenceCount;
	Mem_Free(pBuf);
}

void Snd_ReleaseBuffer()
{
	return Call_Function<void>(0x91310);
	NOT_IMPLEMENTED;
}

void Snd_AcquireBuffer()
{
	return Call_Function<void>(0x91340);
	NOT_IMPLEMENTED;
}
