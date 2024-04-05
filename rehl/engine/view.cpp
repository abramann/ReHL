#include "precompiled.h"

screenshake_t gVShake;


void V_CalcShake()
{
	NOT_IMPLEMENTED;
}

void V_ApplyShake(float* origin, float* angles, float factor)
{
	g_engdstAddrs.V_ApplyShake(&origin, &angles, &factor);

	if (origin)
		VectorMA(origin, factor, gVShake.appliedOffset, origin);

	if (angles)
		angles[2] = gVShake.appliedAngle * factor + angles[2];
}

int V_ScreenShake(const char* pszName, int iSize, void* pbuf)
{
	NOT_IMPLEMENTED;
	return 0;
}

int V_ScreenFade(const char* pszName, int iSize, void* pbuf)
{
	NOT_IMPLEMENTED;
	return 0;
}