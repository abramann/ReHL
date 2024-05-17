#pragma once

typedef struct screenshake_s
{
	float time;
	float duration;
	float amplitude;
	float frequency;
	float nextShake;
	vec3_t offset;
	float angle;
	vec3_t appliedOffset;
	float appliedAngle;
} screenshake_t;

void V_Init();
bool V_CheckGamma();
void V_ApplyShake(float* origin, float* angles, float factor);
void V_CalcShake(void);
int V_ScreenShake(const char* pszName, int iSize, void* pbuf);
int V_ScreenFade(const char* pszName, int iSize, void* pbuf);
void V_UpdatePalette();
void V_RenderView();
