#include "precompiled.h"
#include "cl_main.h"
#include "chase.h"

const int MAX_TEX_GAMMA = 256;
const int MAX_LIGHT_GAMMA = 1024;

screenshake_t gVShake;

float oldgammavalue_25911;
float oldlightgamma_25912;
float oldtexgamma_25913;
float oldbrightness_25914;

vec3_t r_soundOrigin;
vec3_t r_playerViewportAngles;


#ifdef SHARED_GAME_DATA
cvar_t * sp_v_texgamma = ADDRESS_OF_DATA(cvar_t *, 0xC2819);
cvar_t & v_texgamma = *sp_v_texgamma;

cvar_t* sp_v_brightness = ADDRESS_OF_DATA(cvar_t*, 0xC164F);
cvar_t& v_brightness = *sp_v_brightness;

uchar(*sp_texgammatable)[256] = ADDRESS_OF_DATA(uchar(*)[256], 0xC1847);
uchar(&texgammatable)[256] = *sp_texgammatable;

int * lineargammatable = ADDRESS_OF_DATA(int *, 0xC197A);
int* screengammatable = ADDRESS_OF_DATA(int*, 0xC1999);
int* lightgammatable = ADDRESS_OF_DATA(int*, 0xC1919);

cvar_t * sp_v_gamma = ADDRESS_OF_DATA(cvar_t *, 0xC2805);
cvar_t & v_gamma = *sp_v_gamma;

cvar_t * sp_v_lightgamma = ADDRESS_OF_DATA(cvar_t *, 0xC160F);
cvar_t & v_lightgamma = *sp_v_lightgamma;

cvar_t * sp_v_dark = ADDRESS_OF_DATA(cvar_t *, 0xC27F1);
cvar_t & v_dark = *sp_v_dark;

cvar_t * sp_crosshair = ADDRESS_OF_DATA(cvar_t *, 0xC27FB);
cvar_t & crosshair = *sp_crosshair;

cvar_t * sp_v_lambert = ADDRESS_OF_DATA(cvar_t *, 0xC282D);
cvar_t & v_lambert = *sp_v_lambert;

cvar_t * sp_v_direct = ADDRESS_OF_DATA(cvar_t *, 0xC2837);
cvar_t & v_direct = *sp_v_direct;

#else
cvar_t v_texgamma = { "texgamma", "2.0" };
cvar_t v_brightness = { "brightness", "0.0", FCVAR_ARCHIVE };
cvar_t v_lambert = { "v_lambert", "1.5" };
unsigned char texgammatable[256];
int lineargammatable[1024]
int screengammatable[1024];
int lightgammatable[1024];
cvar_t v_gamma = { "gamma", "2.5", FCVAR_ARCHIVE };
cvar_t v_lightgamma = { "lightgamma", "2.5" };
cvar_t v_dark = { "v_dark", "0" };
cvar_t crosshair = { "crosshair", "0", FCVAR_ARCHIVE };
cvar_t v_lambert = { "lambert", "1.5" };
cvar_t v_direct = { "direct", "0.9" };
#endif

vec3_t forward;
vec3_t right;
vec3_t up;

float v_blend[4];

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

	m1.viewent.curstate.frame = 0.0;
	m1.viewent.model = CL_GetModelByIndex(m1.stats[2]);
	m1.viewent.curstate.modelindex = m1.stats[2];
	m1.viewent.curstate.colormap = 0;
	m1.viewent.index = m1.playernum + 1;

	V_SetRefParams(&angles);

	for (int viewnum = 0; angles.nextView; viewnum++)
	{
		if (viewnum == 0)
		{
			if (g_pcls.demoplayback || (CL_SetDemoViewInfo(&angles, m1.viewent.origin, m1.stats[2]), g_pcls.demoplayback))
			{
				if (!g_pcls.spectator)
				{
					CL_GetDemoViewInfo(&angles, m1.viewent.origin, &m1.stats[2]);

					vec3_t clearview;
					VectorCopy(angles.viewangles, clearview);
					clearview[2] *= -1;

					VectorCopy(clearview, m1.viewent.curstate.angles);
					VectorCopy(clearview, m1.viewent.angles);
					VectorCopy(clearview, m1.viewent.latched.prevangles);
					VectorCopy(clearview, m1.viewent.latched.prevangles);

					angles.nextView = 0;
				}
			}
		}

		V_GetRefParams(&angles);
		if (angles.intermission)
		{
			m1.viewent.model = 0;
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
	VectorCopy(m1.simvel, pparams->simvel);
	VectorCopy(m1.simorg, pparams->simorg);
	VectorCopy(m1.viewheight, pparams->viewheight);
	VectorCopy(m1.viewangles, pparams->cl_viewangles);
	VectorCopy(m1.crosshairangle, pparams->crosshairangle);
	VectorCopy(m1.punchangle, pparams->punchangle);
	VectorCopy(m1.crosshairangle, pparams->crosshairangle);

	pparams->time = m1.time;
	pparams->frametime = host_frametime;
	pparams->intermission = m1.intermission != 0;
	pparams->paused = m1.paused != false;
	pparams->spectator = g_pcls.spectator != false;
	pparams->onground = m1.onground != -1;
	pparams->waterlevel = m1.waterlevel;
	pparams->health = m1.stats[0];
	pparams->viewsize = scr_viewsize.value;
	pparams->maxclients = m1.maxclients;
	pparams->viewentity = m1.viewentity;
	pparams->playernum = m1.playernum;
	pparams->max_entities = m1.max_edicts;
	pparams->cmd = &m1.cmd;
	pparams->demoplayback = g_pcls.demoplayback;
	pparams->movevars = &movevars;
	pparams->hardware = 1;
	pparams->smoothing = m1.pushmsec;
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
	VectorCopy(pparams->simvel, m1.simvel);
	VectorCopy(pparams->simorg, m1.simorg);
	VectorCopy(pparams->cl_viewangles, m1.viewangles);
	VectorCopy(pparams->crosshairangle, m1.crosshairangle);
	VectorCopy(pparams->punchangle, m1.punchangle);
	VectorCopy(pparams->viewheight, m1.viewheight);

	r_refdef.vrect.x = pparams->viewport[0];
	r_refdef.vrect.y = pparams->viewport[1];
	r_refdef.vrect.width = pparams->viewport[2];
	r_refdef.vrect.height = pparams->viewport[3];

	r_refdef.onlyClientDraws = pparams->onlyClientDraw;
}