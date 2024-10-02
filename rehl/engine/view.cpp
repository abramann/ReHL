#include "precompiled.h"
#include "cl_main.h"
#include "chase.h"


const int MAX_TEX_GAMMA = 256;
const int MAX_LIGHT_GAMMA = 1024;

typedef struct
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
} Shake_t;

float oldgammavalue_25911;
float oldlightgamma_25912;
float oldtexgamma_25913;
float oldbrightness_25914;

vec3_t r_soundOrigin;
vec3_t r_playerViewportAngles;

ARRAY(int, lineargammatable, [1024], 0xC197A);
ARRAY(int, screengammatable, [1024], 0xC1999);
ARRAY(int, lightgammatable, [1024], 0xC1919);
ARRAY(uchar, texgammatable, [256], 0xC1847);
ARRAY(float, v_blend, [4], 0xC1A53);
VAR(screenshake_t, gVShake, 0xC2857);

VVAR(cvar_t, v_texgamma, 0xC2819, { "texgamma" COMMA "2.0" });
VVAR(cvar_t, v_brightness, 0xC164F, { "brightness" COMMA "0.0" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, v_gamma, 0xC2805, { "gamma" COMMA "2.5" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, v_lightgamma, 0xC160F, { "lightgamma" COMMA "2.5" });
VVAR(cvar_t, v_dark, 0xC27F1, { "v_dark" COMMA "0" });
VVAR(cvar_t, crosshair, 0xC27FB, { "crosshair" COMMA "0" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, v_lambert, 0xC282D, { "lambert" COMMA "1.5" });
VVAR(cvar_t, v_direct, 0xC2837, { "direct" COMMA "0.9" });

vec3_t forward;
vec3_t right;
vec3_t up;


uchar ramps[3][256];

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
	FilterLightParams();
	if (v_gamma.value == oldgammavalue_25911
		&& v_lightgamma.value == oldlightgamma_25912
		&& v_texgamma.value == oldtexgamma_25913)
	{
		if (v_brightness.value == oldbrightness_25914)
			return false;
	}

	BuildGammaTable(v_gamma.value);

	oldgammavalue_25911 = v_gamma.value;
	oldlightgamma_25912 = v_lightgamma.value;
	oldtexgamma_25913 = v_texgamma.value;
	oldbrightness_25914 = v_brightness.value;

	D_FlushCaches();

	vid.recalc_refdef = 1;

	return true;
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
	struct shake_s
	{
		unsigned short amplitude, duration, frequency;
	}; 

	shake_s* shake = (shake_s*)pbuf;

	gVShake.duration = shake->duration * 0.00024414062;
	gVShake.time = gVShake.duration + g_pcl.time;

	float amplitude = 0.00024414062 * shake->amplitude;

	if (amplitude > gVShake.amplitude)
		gVShake.amplitude = amplitude;

	gVShake.nextShake = 0;
	gVShake.frequency = shake->frequency * 0.00390625;
	return 1;
}

int V_ScreenFade(const char* pszName, int iSize, void* pbuf)
{
	struct fade_s
	{
		unsigned short fadeend, fadereset, flags;
		uchar fader;
		uchar fadeg;
		uchar fadeb;
		uchar fadealpha;
	};

	fade_s* fade = (fade_s*)pbuf;
	if (fade == nullptr)
		return 1;

	float rate = 4096.0;

	if ((fade->flags & 8) != 0)
		rate = 256.0;
	
	g_pcl.sf.fader = fade->fader;// *((BYTE*)pbuf + 6);
	g_pcl.sf.fadeSpeed = 0.0;
	g_pcl.sf.fadeg = fade->fadeg;// *((BYTE*)pbuf + 7);
	g_pcl.sf.fadeb = fade->fadeb;// *((BYTE*)pbuf + 8);
	g_pcl.sf.fadealpha = fade->fadealpha;// *((BYTE*)pbuf + 9);
	g_pcl.sf.fadeFlags = fade->flags; 
	g_pcl.sf.fadeEnd = fade->fadeend / rate;
	g_pcl.sf.fadeReset = fade->fadereset / rate;
	if (fade->fadeend == 0)
		return 1;
	if ((fade->flags & 1) == 0)
	{
		if (g_pcl.sf.fadeEnd != 0.0)
		{
			g_pcl.sf.fadeSpeed = (float)fade->fadealpha / g_pcl.sf.fadeEnd;
		}
		g_pcl.sf.fadeReset = g_pcl.sf.fadeReset + g_pcl.time;
		g_pcl.sf.fadeEnd = g_pcl.sf.fadeReset + g_pcl.sf.fadeEnd;
		return 1;
	}
	if (g_pcl.sf.fadeEnd != 0)
	{
		g_pcl.sf.fadeSpeed = -(fade->fadealpha / g_pcl.sf.fadeEnd);
	}
	g_pcl.sf.fadeEnd = g_pcl.sf.fadeEnd + g_pcl.time;
	g_pcl.sf.fadeTotalEnd = g_pcl.sf.fadeEnd;
	g_pcl.sf.fadeReset = g_pcl.sf.fadeReset + g_pcl.sf.fadeEnd;
	return 1;
}

void BuildGammaTable(float g)
{
	if (g == 0)
		g = 2.5f;

	float g0 = 1.0 / g;

	float g1 = g0 * v_texgamma.value;
	float g3 = 0.125f;
	float brightness = v_brightness.value;

	if (brightness > 0.0)
	{
		g3 = 0.05f;
		if (brightness <= 1.0)
			g3 = 0.125f - pow(brightness, 2) * 0.075f;
	}

	for (int i = 0; i < MAX_TEX_GAMMA; i++)
	{
		float gamma = pow(((double)i*1.0) / 255.0, g1) * 255.0;
		if (gamma > 255)
			gamma = -1;
		else if (gamma < 0)
			gamma = 0;
		texgammatable[i] = gamma;
	}

	for (int j = 0; j < MAX_LIGHT_GAMMA; j++)
	{
		float f = pow(((double)j*1.0) / 1026.0, v_lightgamma.value);
		float inf = (brightness > 1.0) ? f * brightness : f;

		float lightgamma;
		if (g3 >= inf)
			lightgamma = pow(inf / g3 * 0.125, g0) * (MAX_LIGHT_GAMMA - 1.0f);
		else
		{
			float fa = (inf - g3) / (1.0f - g3) * 0.875f + 0.125f;
			lightgamma = pow(fa, g0) *(MAX_LIGHT_GAMMA - 1.0f);
		}
		lightgammatable[j] = (lightgamma < 0) ? 0 : lightgamma;
	}

	for (int k = 0; k < MAX_LIGHT_GAMMA; k++)
	{
		double x = (double)(k*1.0) / 1023.0;
		lineargammatable[k] = pow(x, v_gamma.value) * 1023.0;
		screengammatable[k] = pow(x, 1.0 / v_gamma.value) * 1023.0;
	}
}

void FilterLightParams()
{
	if (g_bIsCStrike)
	{
		Cvar_DirectSet(&v_lightgamma, "2.5");
		Cvar_DirectSet(&v_texgamma, "2.0");
	}
	if (Host_GetMaxClients() > 1 && v_brightness.value > 2.0)
		Cvar_DirectSet(&v_brightness, "2.0");

	if (v_gamma.value < 1.8)
	{
		Cvar_DirectSet(&v_gamma, "1.8");
	}
	else if (v_gamma.value > 3.0)
	{
		Cvar_DirectSet(&v_gamma, "3.0");
	}
	if (v_texgamma.value < 1.8)
	{
		Cvar_DirectSet(&v_texgamma, "1.8");
	}
	else if (v_texgamma.value > 3.0)
	{
		Cvar_DirectSet(&v_texgamma, "3.0");
	}
	if (v_lightgamma.value >= 1.8)
	{
		if (v_lightgamma.value > 3.0)
			Cvar_DirectSet(&v_lightgamma, "3.0");
	}
	if (v_brightness.value > 100)
	{
		Cvar_DirectSet(&v_brightness, "100.0");
	}
	else if(v_brightness.value < 0)
	{
		Cvar_DirectSet(&v_brightness, "0");
	}
}

void V_UpdatePalette()
{
	FilterLightParams();

	if (v_gamma.value != oldgammavalue_25911
		|| v_lightgamma.value != oldlightgamma_25912
		|| v_texgamma.value != oldtexgamma_25913
		|| v_brightness.value != oldbrightness_25914)
	{
		BuildGammaTable(v_gamma.value);
		
		oldgammavalue_25911 = v_gamma.value;
		oldlightgamma_25912 = v_lightgamma.value;
		oldtexgamma_25913 = v_texgamma.value;
		oldbrightness_25914 = v_brightness.value;

		D_FlushCaches();

		vid.recalc_refdef = 1;

		v_blend[0] = 0.0;
		v_blend[1] = 0.0;
		v_blend[2] = 0.0;
		v_blend[3] = 0.0;

		for (int i = 0; i < 256; i++)
		{
			ramps[0][i] = texgammatable[i];;
			ramps[1][i] = texgammatable[i];;
			ramps[2][i] = texgammatable[i];;
		}
	}
}

void V_RenderView()
{
	VectorClear(r_soundOrigin);
	VectorClear(r_playerViewportAngles);

	if (con_forcedup || g_pcls.state != ca_active || g_pcls.signon != 2)
		return;

	ref_params_s angles;

	g_pcl.viewent.curstate.frame = 0.0;
	g_pcl.viewent.model = CL_GetModelByIndex(g_pcl.stats[2]);
	g_pcl.viewent.curstate.modelindex = g_pcl.stats[2];
	g_pcl.viewent.curstate.colormap = 0;
	g_pcl.viewent.index = g_pcl.playernum + 1;

	V_SetRefParams(&angles);

	for (int viewnum = 0; angles.nextView; viewnum++)
	{
		if (viewnum == 0)
		{
			if (g_pcls.demoplayback || (CL_SetDemoViewInfo(&angles, g_pcl.viewent.origin, g_pcl.stats[2]), g_pcls.demoplayback))
			{
				if (!g_pcls.spectator)
				{
					CL_GetDemoViewInfo(&angles, g_pcl.viewent.origin, &g_pcl.stats[2]);

					vec3_t clearview;
					VectorCopy(angles.viewangles, clearview);
					clearview[2] *= -1;

					VectorCopy(clearview, g_pcl.viewent.curstate.angles);
					VectorCopy(clearview, g_pcl.viewent.angles);
					VectorCopy(clearview, g_pcl.viewent.latched.prevangles);
					VectorCopy(clearview, g_pcl.viewent.latched.prevangles);

					angles.nextView = 0;
				}
			}
		}

		V_GetRefParams(&angles);
		if (angles.intermission)
		{
			g_pcl.viewent.model = 0;
		}
		else if (!angles.paused && chase_active.value != 0.0)
		{
			Chase_Update();
		}
		
		R_PushDlights();
		if (!viewnum && r_refdef.onlyClientDraws)
		{
			qglClearColor(0.0, 0.0, 0.0, 0.0);
			qglClear(GL_COLOR_BUFFER_BIT);
		}
	}
}

void V_SetRefParams(ref_params_t *pparams)
{
	CHECK_REQUIRED;
	//values forward, up, right are used with no initializing
	
	Q_memset(pparams, 0, 232);
	VectorCopy(r_refdef.vieworg, pparams->vieworg);
	VectorCopy(r_refdef.viewangles, pparams->viewangles);
	VectorCopy(forward, pparams->forward);
	VectorCopy(right, pparams->right);
	VectorCopy(up, pparams->up);
	VectorCopy(g_pcl.simvel, pparams->simvel);
	VectorCopy(g_pcl.simorg, pparams->simorg);
	VectorCopy(g_pcl.viewheight, pparams->viewheight);
	VectorCopy(g_pcl.viewangles, pparams->cl_viewangles);
	VectorCopy(g_pcl.crosshairangle, pparams->crosshairangle);
	VectorCopy(g_pcl.punchangle, pparams->punchangle);
	VectorCopy(g_pcl.crosshairangle, pparams->crosshairangle);

	pparams->time = g_pcl.time;
	pparams->frametime = host_frametime;
	pparams->intermission = g_pcl.intermission != 0;
	pparams->paused = g_pcl.paused != false;
	pparams->spectator = g_pcls.spectator != false;
	pparams->onground = g_pcl.onground != -1;
	pparams->waterlevel = g_pcl.waterlevel;
	pparams->health = g_pcl.stats[0];
	pparams->viewsize = scr_viewsize.value;
	pparams->maxclients = g_pcl.maxclients;
	pparams->viewentity = g_pcl.viewentity;
	pparams->playernum = g_pcl.playernum;
	pparams->max_entities = g_pcl.max_edicts;
	pparams->cmd = &g_pcl.cmd;
	pparams->demoplayback = g_pcls.demoplayback;
	pparams->movevars = &movevars;
	pparams->hardware = 1;
	pparams->smoothing = g_pcl.pushmsec;
	pparams->viewport[0] = 0;
	pparams->viewport[1] = 0;
	pparams->viewport[2] = vid.width;
	pparams->viewport[3] = vid.height;
	pparams->nextView = 0;
	pparams->onlyClientDraw = 0;
}

void V_GetRefParams(ref_params_t *pparams)
{
	VectorCopy(pparams->vieworg, r_refdef.vieworg);
	VectorCopy(pparams->viewangles, r_refdef.viewangles);
	VectorCopy(pparams->forward, forward);
	VectorCopy(pparams->right, right);
	VectorCopy(pparams->up, up);
	VectorCopy(pparams->simvel, g_pcl.simvel);
	VectorCopy(pparams->simorg, g_pcl.simorg);
	VectorCopy(pparams->cl_viewangles, g_pcl.viewangles);
	VectorCopy(pparams->crosshairangle, g_pcl.crosshairangle);
	VectorCopy(pparams->punchangle, g_pcl.punchangle);
	VectorCopy(pparams->viewheight, g_pcl.viewheight);

	r_refdef.vrect.x = pparams->viewport[0];
	r_refdef.vrect.y = pparams->viewport[1];
	r_refdef.vrect.width = pparams->viewport[2];
	r_refdef.vrect.height = pparams->viewport[3];

	r_refdef.onlyClientDraws = pparams->onlyClientDraw;
}

void V_CalcBlend()
{
	for (int i = 0; i < ARRAYSIZE(v_blend); i++)
		v_blend[i] = 0;
}

int V_FadeAlpha()
{
	TO_IMPLEMENT;
	return Call_Function<int>(0xC2700);
}
