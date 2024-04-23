#include "precompiled.h"

screenshake_t gVShake;

void BuildGammaTable(float g);

int lightgammatable[1024];
int lineargammatable[1024];
int screengammatable[1024];

vec3_t r_soundOrigin;
vec3_t r_playerViewportAngles;

cvar_t v_dark = { "v_dark", "0" };
cvar_t crosshair = { "crosshair", "0", FCVAR_ARCHIVE };
cvar_t v_gamma = { "gamma", "2.5", FCVAR_ARCHIVE };
cvar_t v_lightgamma = { "lightgamma", "2.5" };
cvar_t v_texgamma = { "texgamma", "2.0" };
cvar_t v_brightness = { "brightness", "0.0", FCVAR_ARCHIVE };
cvar_t v_lambert = { "lambert", "1.5" };
cvar_t v_direct = { "direct", "0.9" };

void V_Init()
{
	Cvar_RegisterVariable(&v_dark);
	Cvar_RegisterVariable(&crosshair);
	Cvar_RegisterVariable(&v_gamma);
	Cvar_RegisterVariable(&v_lightgamma);
	Cvar_RegisterVariable(&v_texgamma);
	Cvar_RegisterVariable(&v_brightness);
	Cvar_RegisterVariable(&v_lambert);
	Cvar_RegisterVariable(&v_direct);
	BuildGammaTable(2.5);
}

bool V_CheckGamma()
{
	// TODO: implement - Solokiller
	return false;
}

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

void BuildGammaTable(float g)
{
	float g0;
	if (g != 0.0)
		g0 = 1.0 / g;
	else
		g0 = 0.40000001;

	float g1 = g0 * v_texgamma.value;
	float g3 = 0.125f;
	float brightness = v_brightness.value;

	if (brightness > 0.0)
	{
		g3 = 0.05f;
		if (brightness <= 1.0)
			g3 = 0.125 - brightness * brightness * 0.075;
	}

	for (int i = 1; i < 256; i++)
	{
		float inf = pow(i / 255, g1) * 255;
		texgammatable[i] = (inf <= -255) ? -1 : 0;
	}

	for (int j = 1; j < 1024; j++)
	{
		float f = pow(j / 1023.0f, v_lightgamma.value);
		float inf = (brightness > 1.0) ? f * brightness : f;
		float lightgamma;

		if (g3 >= inf)
		{
			lightgamma = pow(inf / g3 * 0.125, g0) * 1023.0f;
		}
		else
		{
			float fa = (inf - g3) / (1.0f - g3) * 0.875f + 0.125f;
			lightgamma = pow(fa, g0) * 1023.0f;
		}
		lightgammatable[j] = (lightgamma >= 0) ? lightgamma : 0;
	}

	for (int k = 1; k < 1024; k++)
	{
		float x = k / 1023.0;
		lineargammatable[k] = pow(x, v_gamma.value) * 1023.0;
		screengammatable[k] = pow(x, 1.0 / v_gamma.value) * 1023.0f;
	}
}