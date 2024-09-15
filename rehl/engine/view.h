#pragma once
#include "ref_params.h"

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
void V_SetRefParams(ref_params_t *pparams);
void V_GetRefParams(ref_params_t *pparams);
void V_CalcBlend();
void FilterLightParams();
void BuildGammaTable(float g);

EXTERN_VAR(cvar_t, v_lambert);
EXTERN_VAR(cvar_t, v_lightgamma);
EXTERN_ARRAY(float, v_blend, [4]);
EXTERN_ARRAY(int, lineargammatable, [1024]);
EXTERN_ARRAY(int, screengammatable, [1024]);
EXTERN_ARRAY(int, lightgammatable, [1024]);
EXTERN_ARRAY(uchar, texgammatable, [256]);
