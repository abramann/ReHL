#include "precompiled.h"
#include "render_api.h"
#include "wrapsin.h"


// TODO: Find good name for variables in ProjectPointOnPlane, BuildSurfaceDisplayList, CalcFov

const float PI = 3.141592653589793f;
const float DEFAULT_BLOCKLIGHT = 65280.0f;
const double FAR_PLANE = 16000;
const double NEAR_PLANE = 4;
const int MAX_LIGHTMAP = 324;
const int MAX_DECAL_SURFACES = 512;
const int SAMPLE_SIZE = 16;
const int LIGHTMAP_POLYGONS = 64;
const int MOD_FRAMES = 20;
const int MAX_SIGNED_SHORT = 32767;
const float TURBSCALE = 256.0f / (PI * 2);
const int MAX_CLIP_VERTS = 128; // skybox clip vertices

const cvar_t gl_envmapsize = { "gl_envmapsize", "256" };
const cvar_t gl_flipmtrix = { "gl_flipmatrix", "0" };
const cvar_t gl_flashblend = { "gl_flashblend", "0" };

const int dottexture[8][8] =
{
	{ 0, 1, 1, 0, 0, 0, 0, 0 },
	{ 1, 1, 1, 1, 0, 0, 0, 0 },
	{ 1, 1, 1, 1, 0, 0, 0, 0 },
	{ 0, 1, 1, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
};

const int vec_to_st[6][3] =
{
  { -2, 3, 1 },
  { 2, 3, -1 },
  { 1, 3, 2 },
  { -1, 3, -2 },
  { -2, -1, 3 },
  { -2, 1, -3 }
};

ARRAY(int, st_to_vec, [6][3], 0x506B2);
/*
const int st_to_vec[6][3] =
{
  { 3, -1, 2 },
  { -3, 1, 2 },
  { 1, 3, 2 },
  { -1, -3, 2 },
  { -2, -1, 3 },
  { 2, -1, -3 }
};
*/
VVAR(int, r_framecount, 0x42E55, 0);
VVAR(int, trickframe, 0x45D68, 0);
ARRAY(int, playertextures, [16], 0x46D8B);	// Probably not needed, single xref
VVAR(qboolean, gl_texsort, 0x494F2, true);
VAR(int, r_numparticles, 0x7ABDA);
VVAR(BEAM*, gBeams, 0x7AB53, nullptr);
VVAR(particle_t*, particles, 0x7AC0C, nullptr);
VAR(int, particletexture, 0x46713);
VAR(cl_entity_t, r_worldentity, 0x46DE9);
VAR(int, c_brush_polys, 0x4649F);
VAR(int, c_alias_polys, 0x464A9);
VAR(qboolean, mirror, 0x45A89);
VAR(float, gldepthmin, 0x45DB8);
VAR(float, gldepthmax, 0x45D86);
VVAR(qboolean, g_bUserFogOn, 0x463B8, false);
VVAR(qboolean, isFogEnabled, 0x44FE4, false);
VVAR(mleaf_t *, r_oldviewleaf, 0x456BE, nullptr;);
VVAR(mleaf_t *, r_viewleaf, 0x456C8, nullptr;);
VAR(vec3_t, vpn, 0x26DC6);
VAR(vec3_t, vright, 0x45686);
VAR(vec3_t, vup, 0x45677);
VAR(cshift_t, cshift_water, 0x4EFD4);
VAR(qboolean, r_cache_thrash, 0x4579A);	// Probably not used, single xref
VVAR(int, filterMode, 0x47454, 0);
VVAR(float, filterColorRed, 0x47454, 0);
VVAR(float, filterColorGreen, 0x47454, 0);
VVAR(float, filterColorBlue, 0x47454, 0);
VVAR(float, filterBrightness, 0x47454, 0);
VVAR(int, gl_FilterTexture, 0x3F354, 0;);
ARRAY(int, d_lightstylevalue, [256], 0x42EC9);
VVAR(qboolean, envmap, 0x4592C, false;);
VVAR(mplane_t*, mirror_plane, 0x45A92, nullptr;);
ARRAY(vec4_t, gProjectionMatrix, [4], 0x45C1F);
ARRAY(vec4_t, r_world_matrix, [4], 0x45BAB);
ARRAY(vec4_t, gWorldToScreen, [4], 0x45CB9);
ARRAY(vec4_t, gScreenToWorld, [4], 0x45CB4);
VVAR(cl_entity_t*, currententity, 0x4949D, nullptr;);
VAR(vec3_t, modelorg, 0x4947B);
VAR(colorVec, gWaterColor, 0x4948B);
ARRAY(glpoly_t*, lightmap_polys, [LIGHTMAP_POLYGONS], 0x494E0);
ARRAY(float, skymins, [2][6], 0x4FEEA);
ARRAY(float, skymaxs, [2][6], 0x4FEFE);
VAR(int, gDecalSurfCount, 0x49577);
VARRAY(msurface_t*, gDecalSurfs, [500], 0x47D74 ,{ nullptr });
VAR(int, gRenderMode, 0x89257);
ARRAY(mplane_t, frustum, [4], 0x452DC);
VVAR(int, r_dlightactive, 0x47403, 0);
VARRAY(colorVec, blocklights, [MAX_LIGHTMAP], 0x4761D, { 0 });
VVAR(int, mirrortexturenum, 0x4938D, 0);
VVAR(msurface_t*, skychain, 0x48AD7, nullptr);
VVAR(msurface_t*, waterchain, 0x493C3, nullptr);
VVAR(int, r_dlightframecount, 0x43112, 0);
VVAR(int, skytexturenum, 0x48B55, 0);
VVAR(cl_entity_t*, cl_entities, 0x48AB5, nullptr);
VVAR(int, gLoadSky, 0x4F9C9, 0);
VVAR(int, c_sky, 0x50565, 0)
VARRAY(int, skytexorder, [6], 0x508F3, { 0 COMMA 2 COMMA 1 COMMA 3 COMMA 4 COMMA 5 });
VVAR(qboolean, g_bFogSkybox, 0x5080A, true);
VAR(GLuint, gSkyTexNumber, 0x4FACF);
ARRAY(vec_t, skyclip, [6][3], 0x50037);
VARRAY(cl_entity_t*, cl_visedicts, [512], 0x4488C, nullptr);
VVAR(float, r_blend, 0x483C7, 0);
VAR(vec3_t, r_entorigin, 0x886DF);
VVAR(transObjRef*, transObjects, 0x885F4, nullptr);
VAR(int, numTransObjs, 0x888E0);
VAR(int, maxTransObjs, 0x888D7);

// cvars
VVAR(cvar_t, r_cachestudio, 0x46BD4, { "r_cachestudio" COMMA "1" COMMA 0 COMMA 0.0f COMMA nullptr });
VVAR(cvar_t, r_cullsequencebox, 0x46BDE, { "r_cullsequencebox" COMMA "1" });
VVAR(cvar_t, r_bmodelinterp, 0x46BE8, { "r_bmodelinterp" COMMA "1" });
VVAR(cvar_t, r_norefresh, 0x46BF2, { "r_norefresh" COMMA "0" });
VVAR(cvar_t, r_lightmap, 0x46BFC, { "r_lightmap" COMMA "0" });
VVAR(cvar_t, r_fullbright, 0x46C09, { "r_fullbright" COMMA "0" });
VVAR(cvar_t, r_decals, 0x46C13, { "r_decals" COMMA "4096" });
VVAR(cvar_t, sp_decals, 0x46C1D, { "sp_decals" COMMA "4096" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, mp_decals, 0x46C27, { "mp_decals" COMMA "300" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, r_drawentities, 0x46C40, { "r_drawentities" COMMA "1" });
VVAR(cvar_t, r_drawviewmodel, 0x46C4A, { "r_drawviewmodel" COMMA "1" });
VVAR(cvar_t, r_mirroralpha, 0x46C54, { "r_mirroralpha" COMMA "1" });
VVAR(cvar_t, r_wateralpha, 0x46C5E, { "r_wateralpha" COMMA "1" });
VVAR(cvar_t, r_dynamic, 0x46C68, { "r_dynamic" COMMA "1" });
VVAR(cvar_t, r_novis, 0x46C72, { "r_novis" COMMA "0" });
VVAR(cvar_t, r_speeds, 0x46C7C, { "r_speeds" COMMA "0" });
VVAR(cvar_t, d_spriteskip, 0x46C86, { "d_spriteskip" COMMA "0" });
VVAR(cvar_t, r_wadtextures, 0x46C90, { "r_wadtextures" COMMA "0" COMMA 0 COMMA 0.0f COMMA NULL });
VVAR(cvar_t, r_mmx, 0x46C9A, { "r_mmx" COMMA "0" });
VVAR(cvar_t, r_traceglow, 0x46CA7, { "r_traceglow" COMMA "0" });
VVAR(cvar_t, r_glowshellfreq, 0x46CB1, { "r_glowshellfreq" COMMA "2.2" });
VVAR(cvar_t, gl_clear, 0x46CBB, { "gl_clear"  COMMA "0" });
VVAR(cvar_t, gl_cull, 0x46CC5, { "gl_cull"  COMMA "1" });
VVAR(cvar_t, gl_affinemodels, 0x46CCF, { "gl_affinemodels"  COMMA "0" });
VVAR(cvar_t, gl_dither, 0x46CD9, { "gl_dither"  COMMA "1" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, gl_spriteblend, 0x46CE3,  { "gl_spriteblend"  COMMA "1" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, gl_polyoffset, 0x46CED, { "gl_polyoffset"  COMMA "4" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, gl_lightholes, 0x46CF7, { "gl_lightholes"  COMMA "1" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, gl_keeptjunctions, 0x46D01, { "gl_keeptjunctions"  COMMA "1" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, gl_wateramp, 0x46D0B, { "gl_wateramp" COMMA "0.3" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, gl_overbright, 0x46D15, { "gl_overbright" COMMA "1"  COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, gl_zmax, 0x46D1F, { "gl_zmax" COMMA "4096" });
VVAR(cvar_t, gl_alphamin, 0x46D29, { "gl_alphamin" COMMA "0.25" });
VVAR(cvar_t, gl_flipmatrix, 0x46D33, { "gl_flipmatrix" COMMA "0" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, gl_monolights, 0x46D3D, { "gl_monolights" COMMA "0" COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, ati_npatch, 0x46BC0, { "ati_npatch" COMMA "1.0"  COMMA FCVAR_ARCHIVE });
VVAR(cvar_t, gl_wireframe, 0x46BCA, { "gl_wireframe" COMMA "0" COMMA 40 });
VVAR(cvar_t, gl_fog, 0x46D4A, { "gl_fog" COMMA "0" COMMA FCVAR_ARCHIVE });

void R_PreDrawViewModel()
{
	return Call_Function<void>(0x44E40);
}

void R_DrawViewModel()
{
	return Call_Function<void>(0x44AA0);
}

void R_RenderScene()
{
	if (CL_IsDevOverviewMode())
		CL_SetDevOverView(&r_refdef);

	R_SetupFrame();
	R_SetFrustum();
	R_SetupGL();
	R_MarkLeaves();
	
	if (r_refdef.onlyClientDraws == false)
	{
		if (CL_IsDevOverviewMode())
		{
			qglClearColor(0, 1, 0, 0);
			qglClear(GL_COLOR_BUFFER_BIT);
		}
		R_DrawWorld();
		S_ExtraUpdate();
		R_DrawEntitiesOnList();
	}
	if (g_bUserFogOn)
		R_RenderFinalFog();

	AllowFog(false);
	ClientDLL_DrawNormalTriangles();
	AllowFog(true);
	
	if (g_pcl.waterlevel > 2 && !r_refdef.onlyClientDraws || !g_bUserFogOn)
		qglDisable(GL_FOG);
	
	R_DrawTEntitiesOnList(r_refdef.onlyClientDraws);
	S_ExtraUpdate();
	if (!r_refdef.onlyClientDraws)
	{
		R_RenderDlights();
		GL_DisableMultitexture();
		R_DrawParticles();
	}
}
void R_PolyBlend()
{
	return Call_Function<void>(0x45000);
}

void R_Init()
{
	Cmd_AddCommandWithFlags("timerefresh", R_TimeRefresh_f, 16);
	Cmd_AddCommand("envmap", R_Envmap_f);
	Cmd_AddCommand("pointfile", R_ReadPointFile_f);
	Cmd_AddCommand("gl_dump", GL_Dump_f);
	Cvar_RegisterVariable(&ati_npatch);
	Cvar_RegisterVariable(&gl_wireframe);
	Cvar_RegisterVariable(&r_cachestudio);
	Cvar_RegisterVariable(&r_cullsequencebox);
	Cvar_RegisterVariable(&r_bmodelinterp);
	Cvar_RegisterVariable(&r_norefresh);
	Cvar_RegisterVariable(&r_lightmap);
	Cvar_RegisterVariable(&r_fullbright);
	Cvar_RegisterVariable(&r_decals);
	Cvar_RegisterVariable(&sp_decals);
	Cvar_RegisterVariable(&mp_decals);

	Cvar_SetValue("r_decals", 4096.0);

	Cvar_RegisterVariable(&r_drawentities);
	Cvar_RegisterVariable(&r_drawviewmodel);
	Cvar_RegisterVariable(&r_mirroralpha);
	Cvar_RegisterVariable(&r_wateralpha);
	Cvar_RegisterVariable(&r_dynamic);
	Cvar_RegisterVariable(&r_novis);
	Cvar_RegisterVariable(&r_speeds);
	Cvar_RegisterVariable(&d_spriteskip);
	Cvar_RegisterVariable(&r_wadtextures);
	Cvar_RegisterVariable(&r_mmx);
	Cvar_RegisterVariable(&r_traceglow);
	Cvar_RegisterVariable(&r_glowshellfreq);
	Cvar_RegisterVariable(&gl_clear);
	Cvar_RegisterVariable(&gl_cull);
	Cvar_RegisterVariable(&gl_affinemodels);
	Cvar_RegisterVariable(&gl_dither);
	Cvar_RegisterVariable(&gl_spriteblend);
	Cvar_RegisterVariable(&gl_polyoffset);
	Cvar_RegisterVariable(&gl_lightholes);
	Cvar_RegisterVariable(&gl_keeptjunctions);
	Cvar_RegisterVariable(&gl_wateramp);
	Cvar_RegisterVariable(&gl_overbright);
	Cvar_RegisterVariable(&gl_zmax);
	Cvar_RegisterVariable(&gl_alphamin);
	Cvar_RegisterVariable(&gl_flipmatrix);
	Cvar_RegisterVariable(&gl_monolights);
	Cvar_RegisterVariable(&gl_fog);

	if (gl_mtexable)
	{
		gl_texsort = false;
		Cvar_SetValue("gl_overbright", 0.0);
	}

	R_InitParticles();
	R_InitParticleTexture();
	//R_UploadEmptyTex();	// Not needed?
	//for (int i = 0; i < 16; i++)
	//	playertextures[i] = GL_GenTexture(); // no reference, not needed?
}

void R_PushDlights()
{
	if (gl_flashblend.value != 0.0)
		return;

	r_dlightframecount = r_framecount + 1;

	for (int i = 0; i < ARRAYSIZE(cl_dlights); i++)
	{
		dlight_t& light = cl_dlights[i];
		if (g_pcl.time <= light.die && light.radius != 0.0)
			R_MarkLights(&light, 1 << i, g_pcl.worldmodel->nodes);
	}
}

void R_InitParticles()
{
	int sizeParticles;
	int numParticles;

	int argcParticles = COM_CheckParm("-particles");
	if (argcParticles)
	{
		numParticles = Q_atoi(com_argv[argcParticles + 1]);
		r_numparticles = numParticles;
		if (numParticles > 511)
		{
			sizeParticles = 56 * numParticles;
		}
		else
		{
			r_numparticles = 512;
			sizeParticles = r_numparticles * sizeof(particle_t);
		}
	}
	else
	{
		r_numparticles = 2048;
		sizeParticles = sizeof(particle_t) * r_numparticles;
	}
	particles = (particle_t *)Hunk_AllocName(sizeParticles, "particles");
	gBeams = (BEAM *)Hunk_AllocName(sizeof(BEAM) * 128 , "lightning");
}

void R_InitParticleTexture()
{
	particletexture = GL_GenTexture();

	GL_Bind(particletexture);

	char data[8][8][4] = { 0 };
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				if (k == 3)
					data[i][j][k] = -dottexture[j][i];
				else
					data[i][j][k] = -1;
			}
		}
	}

	int w, h, bpp;
	VideoMode_GetCurrentVideoMode(&w, &h, &bpp);

	if (bpp == 32)
		qglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		qglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA2, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448.0);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 9729.0);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 9729.0);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, gl_ansio.value);
}

void R_UploadEmptyTex()
{
	uchar pPal[768];

	Q_memset(pPal, 0, 768);

	pPal[765] = 255;
	pPal[766] = 0;
	pPal[767] = 255;

	int width = r_notexture_mip->width;
	int height = r_notexture_mip->height;

	r_notexture_mip->gl_texturenum = GL_LoadTexture("**empty**", GLT_SYSTEM, width, height,
		(unsigned char*)&r_notexture_mip[1], 1, 0, (unsigned char*)pPal);
}

void R_TimeRefresh_f(void)
{
	if (g_pcl.worldmodel)
	{
		if (sv_cheats.value == 0.0)
		{
			Con_Printf("sv_cheats not enabled\n");
		}
		else
		{
			qglDrawBuffer(GL_FRONT);
			qglFinish();
			float begintime = Sys_FloatTime();
			for (int i = 0; i < 128; i++)
			{
				r_refdef.viewangles[1] = i * 0.0078125 * 360.0;
				R_RenderView();
			}
			qglFinish();
			float delta = Sys_FloatTime() - begintime;
			Con_Printf("%f seconds (%f fps)\n", delta, (double)(128.0 / (delta)));
			qglDrawBuffer(GL_BACK);
			GL_EndRendering();
		}
	}
	else
	{
		Con_Printf("No map loaded\n");
	}
}

void R_Envmap_f(void)
{
	NOT_IMPLEMENTED;
}

void R_ReadPointFile_f(void)
{
	NOT_IMPLEMENTED;
}

void R_RenderView()
{
	double time1;
	float framerate;
	
	if (r_norefresh.value == 0.0)
	{
		if (r_worldentity.model == nullptr || g_pcl.worldmodel == nullptr)
			Sys_Error("%s: NULL worldmodel", __func__);

		if (r_speeds.value != 0.0)
		{
			qglFinish();
			time1 = Sys_FloatTime();
			c_brush_polys = 0;
			c_alias_polys = 0;
		}

		mirror = false;
	
		R_Clear();

		if (!r_refdef.onlyClientDraws)
			R_PreDrawViewModel();	// Implement

		R_RenderScene(); 
		if (!r_refdef.onlyClientDraws)
		{
			R_DrawViewModel(); // Implement
			R_PolyBlend();	// Implement
		}

		S_ExtraUpdate();

		if (r_speeds.value != 0)
		{
			framerate = g_pcl.time - g_pcl.oldtime;
			float fps = framerate;
			if (framerate > 0.0)
				fps = 1.0 / fps;

			float time2 = Sys_FloatTime();
			Con_Printf("%3ifps %3i ms  %4i wpoly %4i epoly\n",
				fps + 0.5, (time2 - time1) * 1000, c_brush_polys, c_alias_polys);
		}
	}
}

void R_SetStackBase()
{
	// Nothing
}

void R_ForceCVars(qboolean multiplayer)
{ 
	if (multiplayer)
		return;

	if (r_lightmap.value != 0)
		Cvar_DirectSet(&r_lightmap, "0");

	if (gl_clear.value != 0)
		Cvar_DirectSet(&gl_clear, "0");

	if (r_novis.value != 0)
		Cvar_DirectSet(&r_novis, "0");

	if (r_fullbright.value != 0.0)
		Cvar_DirectSet(&r_fullbright, "0");

	if (snd_show.value != 0.0)
		Cvar_DirectSet(&snd_show,"0");

	if (chase_active.value != 0.0)
		Cvar_DirectSet(&chase_active, "0");

	if (v_lambert.value != 1.5)
		Cvar_DirectSet(&v_lambert, "1.5");

	if (gl_monolights.value != 0.0)
	{
		Cvar_DirectSet(&gl_monolights, (char *)"0");
		GL_BuildLightmaps();
	}

	if (gl_wireframe.value != 0.0)
		Cvar_DirectSet(&gl_wireframe, (char *)"0");

	if (r_dynamic.value != 1.0)
		Cvar_DirectSet(&r_dynamic, (char *)"1");

	if (gl_alphamin.value != 0.25)
		Cvar_DirectSet(&gl_alphamin, "0.25");

	if (gl_max_size.value < 255.0)
		Cvar_DirectSet(&gl_max_size, "256");

	float polyoffset = gl_polyoffset.value;
	if(polyoffset < -0.001f)
		Cvar_DirectSet(&gl_polyoffset, "-0.001");
	else if (polyoffset < 0.001f)
		Cvar_DirectSet(&gl_polyoffset, "0.001");
	else if(polyoffset > 25.0f)
		Cvar_DirectSet(&gl_polyoffset, "25");

	if (sv_cheats.value == 0.0 && r_drawentities.value != 1.0)
		Cvar_DirectSet(&r_drawentities, "1");

	if (v_lightgamma.value < 1.8)
	{
		Cvar_DirectSet(&v_lightgamma, "1.8");
		GL_BuildLightmaps();
		D_FlushCaches();
	}
}

void SCR_BeginLoadingPlaque(qboolean reconnect)
{
	NOT_IMPLEMENTED;
	return Call_Function<void, qboolean>(0x4C130, reconnect);
}

void R_InitSky() 
{ 
	gLoadSky = 1;
}

void R_MarkLeaves()
{
	//return Call_Function<void>(0x495C0);

	if (mirror)
		return;
	if (r_oldviewleaf == r_viewleaf && r_novis.value == 0) 
		return;

	uchar solid[4096];
	uchar* vis = nullptr;

	r_visframecount++;
	r_oldviewleaf = r_viewleaf;
	if (r_novis.value == 0)
	{
		vis = Mod_LeafPVS(r_viewleaf, g_pcl.worldmodel);
	}
	else
	{
		Q_memset(solid, 255, (g_pcl.worldmodel->numleafs + 7) / 8);
		vis = solid;
	}
	if (g_pcl.worldmodel->numleafs <= 0)
		return;

	model_t* worldmodel = g_pcl.worldmodel;
	for (int i = 0; i < worldmodel->numleafs; i++)
	{
		if (vis[i / 8] / ((int)pow(2 ,(i & 7))) %2 != 0)
		{
			mleaf_t* leaf = &worldmodel->leafs[i+1];
			do
			{
				if (leaf->visframe == r_visframecount)
					break;
				leaf->visframe = r_visframecount;
				leaf = (mleaf_t*)leaf->parent;
			} while (leaf);
		}
	}
}

void R_InitTextures()
{
	r_notexture_mip = (texture_t *)Hunk_AllocName(sizeof(texture_t) + 16 * 16 + 8 * 8 + 4 * 4 + 2 * 2, "notexture");
	r_notexture_mip->height = 16;
	r_notexture_mip->width = 16;
	r_notexture_mip->offsets[0] = sizeof(texture_t);
	r_notexture_mip->offsets[1] = r_notexture_mip->offsets[0] + 16 * 16;
	r_notexture_mip->offsets[2] = r_notexture_mip->offsets[1] + 8 * 8;
	r_notexture_mip->offsets[3] = r_notexture_mip->offsets[2] + 4 * 4;

	for (int m = 0; m < 4; m++)
	{
		int texSize = 16 >> m;
		unsigned char* dest = (unsigned char *)r_notexture_mip + r_notexture_mip->offsets[m];

		for (int x = 0; x < texSize; x++)
		{
			for (int y = 0; y < texSize; y++, dest++)
			{
				if ((x < (texSize / 2)) == (y < (texSize / 2)))
					*dest = -1;
				else
					*dest = 0;
			}
		}
	}
}

model_t* R_LoadMapSprite(const char* szFilename)
{
	NOT_IMPLEMENTED;
	return nullptr;
}

void R_Clear()
{
	//return Call_Function<void>(0x45CD0);

	R_ForceCVars(g_pcl.maxclients > 1);
	if (r_mirroralpha.value == 1.0)
	{
		if (gl_ztrick.value == 0.0)
		{
			if (gl_clear.value == 0.0)
				qglClear(GL_DEPTH_BUFFER_BIT);
			else
				qglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			gldepthmin = 0.0;
			gldepthmax = 1.0;
			glDepthFunc(GL_LEQUAL);
		}
		else
		{
			if (gl_clear.value != 0.0)
				qglClear(GL_COLOR_BUFFER_BIT);
			if ((++trickframe & 1) != 0)
			{
				gldepthmin = 0.0f;
				gldepthmax = 5.0f;
				qglDepthFunc(GL_LEQUAL);
			}
			else
			{
				gldepthmin = 1.0f;
				gldepthmax = 0.5f;
				qglDepthFunc(GL_GEQUAL);
			}
		}
	}
	else
	{
		if (gl_clear.value == 0.0)
			qglClear(GL_DEPTH_BUFFER_BIT);
		else
			qglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gldepthmin = 0.0f;
		gldepthmax = 0.5f;
		qglDepthFunc(GL_LEQUAL);
	}
	qglDepthRange(gldepthmin, gldepthmax);
}

void R_SetupFrame()
{
	//return Call_Function<void>(0x45640);

	R_ForceCVars(g_pcl.maxclients > 1);
	R_CheckVariables();
	R_AnimateLight();

	r_framecount++;

	VectorCopy(r_refdef.vieworg, r_origin);

	r_oldviewleaf = r_viewleaf;
	AngleVectors(r_refdef.viewangles, vpn, vright, vup);
	r_viewleaf = Mod_PointInLeaf(r_origin, g_pcl.worldmodel);
	if (g_pcl.waterlevel > 2 && !r_refdef.onlyClientDraws)
	{
		float fogColor[4] = { cshift_water.destcolor[0] / 255.0f,
								cshift_water.destcolor[1] / 255.0f,
								cshift_water.destcolor[2] / 255.0f,
								1.0f };
		GLfloat forward = 1536 - 4.0f * cshift_water.percent;

		qglFogi(GL_FOG_MODE, GL_LINEAR);
		qglFogfv(GL_FOG_COLOR, fogColor);
		qglFogf(GL_FOG_START, 0);
		qglFogf(GL_FOG_END, forward);
		qglEnable(GL_FOG);
	}
	
	V_CalcBlend();
	c_brush_polys = 0;
	r_cache_thrash = false;
	c_alias_polys = 0;
}

void R_SetFrustum()
{
	//return Call_Function<void>(0x45270);

	float fov_x = 90.0;	
	float fov = CalcFov(fov_x, glwidth, glheight);

	float xa = 90 - (scr_fov_value / 2);
	float xb = 90 - (fov / 2);

	RotatePointAroundVector(&frustum[0].normal, vup, vpn, -xa);
	RotatePointAroundVector(&frustum[1].normal,vup, vpn, xa);
	RotatePointAroundVector(&frustum[2].normal, vright,vpn, xb);
	RotatePointAroundVector(&frustum[3].normal, vright, vpn, -xb);
	for (int i = 0; i < ARRAYSIZE(frustum); i++)
	{
		frustum[i].dist = _DotProduct(frustum[i].normal, r_origin);
		frustum[i].signbits = SignbitsForPlane(&frustum[i]);
		frustum[i].type = 5;
	}
}

void R_SetupGL()
{
	//return Call_Function<void>(0x458D0);

	qglMatrixMode(GL_PROJECTION);
	qglLoadIdentity();

	int x = r_refdef.vrect.x;
	int x2 = r_refdef.vrect.width + r_refdef.vrect.x;
	int y2 = glheight - r_refdef.vrect.y;
	int y = glheight - r_refdef.vrect.height - r_refdef.vrect.y;
	if (r_refdef.vrect.x > 0)
		x = r_refdef.vrect.x - 1;
	if (x2 < glwidth)
		++x2;
	if (y < 0)
		--y;
	if (y2 < glheight)
		++y2;

	int vpWidth = x2 - x;
	int vpHeight = y2 - y;
	if (envmap)
	{
		y = 0;
		x = 0;
		vpWidth = vpHeight = gl_envmapsize.value;	// const
		glwidth = glheight = gl_envmapsize.value;
	}

	qglViewport(x + glx, y + gly, vpWidth, vpHeight);

	float width = r_refdef.vrect.width;
	float height = r_refdef.vrect.height;
	float screenaspect = width / height;

	float fov = CalcFov(scr_fov_value, width, height);

	float viewfield = tan(fov * PI / 360.0f) * 4;

	if (r_refdef.onlyClientDraws)
	{
		glFrustum(screenaspect * -viewfield, viewfield * screenaspect, -viewfield, viewfield, NEAR_PLANE, FAR_PLANE);
	}
	else  if (CL_IsDevOverviewMode())
	{
		float zoom = gDevOverview.zoom;
		float clipplane = 4096.0 / zoom;
		screenaspect = 4096.0 / (zoom * screenaspect);
		qglOrtho(
			clipplane,
			clipplane,
			screenaspect,
			screenaspect,
			FAR_PLANE - gDevOverview.z_min,
			FAR_PLANE - gDevOverview.z_max);
	}
	else
	{
		glFrustum(screenaspect * -viewfield, viewfield * screenaspect, -viewfield, viewfield, NEAR_PLANE, movevars.zmax);
	}

	if (mirror)
	{
		if (mirror_plane->normal[2] == 0.0)
			qglScalef(-1, 1, 1);
		else
			qglScalef(1, -1, 1);

		qglCullFace(GL_BACK);
	}
	else
	{
		qglCullFace(GL_FRONT);
	}
	qglGetFloatv(GL_PROJECTION_MATRIX, (float*)gProjectionMatrix);

	qglMatrixMode(GL_MODELVIEW);
	qglLoadIdentity();
	qglRotatef(-90.0, 1.0, 0.0, 0.0);
	qglRotatef(90.0, 0.0, 0.0, 1.0);
	qglRotatef(-r_refdef.viewangles[2], 1.0, 0.0, 0.0);
	qglRotatef(-r_refdef.viewangles[0], 0.0, 1.0, 0.0);
	qglRotatef(-r_refdef.viewangles[1], 0.0, 0.0, 1.0);

	qglTranslatef(-r_refdef.vieworg[0], -r_refdef.vieworg[1], -r_refdef.vieworg[2]);
	qglGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)r_world_matrix);
	if (gl_cull.value == 0.0)
		qglDisable(GL_CULL_FACE);
	else
		qglEnable(GL_CULL_FACE);

	qglDisable(GL_BLEND);
	qglDisable(GL_ALPHA_TEST);
	qglEnable(GL_DEPTH_TEST);

	if (gl_flipmatrix.value == 0.0)
	{		
		for (int i = 0; i < 4; i++)
		{
			vec4_t& r_worldmat = r_world_matrix[i];
			vec4_t& world2scren = gWorldToScreen[i];
			for (int j = 0; j < 4; j++)
			{
				vec4_t& projmat = gProjectionMatrix[j];
				gWorldToScreen[i][j] = 0;

				for (int k = 0; k < 4; k++)
				{
					gWorldToScreen[i][j] += r_worldmat[k] * projmat[k];
				}
			}
		}
	}
	else
	{
		// Never executed code since gl_flipmatrix never changed
		NOT_IMPLEMENTED;
	}

	InvertMatrix(gScreenToWorld, gWorldToScreen);
}

void R_ClearSkyBox()
{
	for (int i = 0; i < 6; i++)
	{
		skymins[0][i] = skymins[1][i] = 9999.0;
		skymaxs[0][i] = skymaxs[1][i] = -9999.0;
	}
}

void R_RecursiveWorldNode(mnode_t* node)
{	
	//return Call_Function<void, mnode_t*>(0x49220, node);

	int side;

	for (mnode_t* n = node; n->contents != CONTENT_SOLID && n->visframe == r_visframecount; n = n->children[side])
	{
		if (R_CullBox(n->minmaxs, &n->minmaxs[3]))
			break;

		if (n->contents < 0)
		{
			mleaf_t* pleaf = (mleaf_t*)n;

			msurface_t** mark = pleaf->firstmarksurface;
			int c = pleaf->nummarksurfaces;

			if (c)
			{
				do
				{
					(*mark)->visframe = r_framecount;
					mark++;
				} while (--c);
			}

			// deal with model fragments in this leaf
			if (pleaf->efrags)
				R_StoreEfrags(&pleaf->efrags);
			return;
		}
		mplane_t* plane = n->plane;
		byte type = plane->type;
		
		double dot;
		if (type == 1)
		{
			dot = modelorg[1] - plane->dist;
		}
		else if (type)
		{
			if (type == 2)
				dot = modelorg[2] - plane->dist;
			else
				dot = _DotProduct(modelorg,plane->normal) - plane->dist;
		}
		else
		{
			dot = modelorg[0] - plane->dist;
		}
		R_RecursiveWorldNode(n->children[dot < 0]);
		int numsurfaces = n->numsurfaces;
		dot = (double)dot;
		if (n->numsurfaces)
		{
			side = 0;
			msurface_t* surf = &g_pcl.worldmodel->surfaces[n->firstsurface];
			if (dot >= -0.01)
			{
				side = dot >= 0 ? 1 : 0;
			}
			for (int i = 0; i < numsurfaces; i++, surf++)
			{
				if (surf->visframe != r_framecount)
					continue;

				int flags = surf->flags;
				if ((flags & 0x80u) != 0 || ((flags & 2) != 0) == dot < 0)
				{
					if (gl_texsort)
					{
						mtexinfo_t* texinfo = surf->texinfo;
						texture_t* texture = texinfo->texture;
						if (mirror == false || texture != g_pcl.worldmodel->textures[mirrortexturenum])
						{
							surf->texturechain = texture->texturechain;
							texinfo->texture->texturechain = surf;
						}
					}
					else if ((flags & 4) != 0)
					{
						surf->texturechain = skychain;
						skychain = surf;
					}
					else if ((flags & 0x10) != 0)
					{
						surf->texturechain = waterchain;
						waterchain = surf;
					}
					else
					{
						R_DrawSequentialPoly(surf, 0);
					}
				}
			}
		}
		else
		{
			side = dot >= 0;
		}
	}
}

qboolean R_CullBox(vec_t* mins, vec_t* maxs)
{
	for (int i = 0; i < 4; i++)
	{
		if (BoxOnPlaneSide(mins, maxs,&frustum[i]) == 2)
			return true;
	}
	return false;
}

void R_StoreEfrags(efrag_t** ppefrag)
{
	efrag_t* efrag;

	for (; *ppefrag; ppefrag = &efrag->leafnext)
	{
		efrag = *ppefrag;

		cl_entity_s* ent = efrag->entity;
		if (ent->model->type > (unsigned int)mod_alias)
			Sys_Error("%s Bad entity type %d\n", __func__, ent->model->type);
		if (ent->visframe != r_framecount && cl_numvisedicts <= 511)
		{
			if (ClientDLL_AddEntity(ET_FRAGMENTED, efrag->entity))
				ent->visframe = r_framecount;
		}
	}
}

void R_BlendLightmaps()
{
	//return Call_Function<void>(0x48500);

	if (filterMode || r_fullbright.value != 0.0 || !gl_texsort)
		return;	

	qglDepthMask(GL_ZERO);
	if (gl_lightmap_format == GL_LUMINANCE)
	{
		glBlendFunc(0, GL_ONE_MINUS_SRC_COLOR);
	}
	else if (gl_lightmap_format != GL_INTENSITY && gl_lightmap_format == GL_RGBA && gl_monolights.value == 0.0)
	{
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448.0);
		if (gl_overbright.value == 0)
		{
			qglBlendFunc(0, GL_SRC_COLOR);
		}
		else
		{
			qglBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
			qglColor4f(0.66, 0.66, 0.66, 1.0);
		}
	}
	else
	{
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448.0);
		qglColor4f(0, 0, 0, 1.0);
		qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	if (r_lightmap.value == 0.0)
		qglEnable(GL_BLEND);

	for (int i = 0; i < LIGHTMAP_POLYGONS; i++)
	{
		glpoly_t* p = lightmap_polys[i];
		if (p)
		{
			GL_Bind(lightmap_textures[i]);
			qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 9729.0);
			qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 9729.0);
			if (lightmap_modified[i])
			{
				lightmap_modified[i] = false;
				qglTexSubImage2D(GL_TEXTURE_2D, 0, 0,
					0, 128, 128,
					gl_lightmap_format, GL_UNSIGNALED,
					&lightmaps[0x4000 * lightmap_bytes * i]);
			}
			for (; p != nullptr; p = p->chain)
			{
				int flags = p->flags;
				if ((flags & 0x80u) == 0)
				{
					if ((flags & 0x10) != 0)
					{
						for (glpoly_t* poly = p; poly != nullptr; poly = poly->next)
						{
 							qglBegin(GL_POLYGON);
							qglColor3f(1.0, 1.0, 1.0);

							for (int j = 0; j < poly->numverts; j++)
							{
								float* verts = (float*)poly->verts[j];
								qglTexCoord2f(verts[5], verts[6]);
								float tempVert[3];
								tempVert[0] = verts[0];
								tempVert[1] = verts[1];
								float wateramp = gl_wateramp.value;
								float nv = turbsin[(int)(g_pcl.time * 160.0 + verts[0] + verts[1])] * wateramp + verts[2];
								nv += wateramp * 0.8 * turbsin[(int)(g_pcl.time * 171.0 + verts[0] * 5.0 - verts[1])];
								tempVert[2] = nv;
								qglVertex3fv(tempVert);
							}
							qglEnd();
						}
					}
					else
					{
						qglBegin(GL_POLYGON);
						for (int j = 0; j < p->numverts; j++)
						{
							float* verts = p->verts[j];

							qglTexCoord2f(verts[5], verts[6]);
							qglVertex3fv(verts);
						}
						qglEnd();
					}
				}
				else
				{
					DrawGLWaterPolyLightmap(p);
				}
			}
		}
	}
	qglDisable(GL_BLEND);
	if (gl_lightmap_format == GL_LUMINANCE)
	{
		qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if (gl_lightmap_format == GL_INTENSITY || gl_monolights.value != 0.0)
	{
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 7681);
		qglColor4f(1.0, 1.0, 1.0, 1.0);
	}
	qglDepthMask(GL_ONE);
}

void R_DrawSequentialPoly(msurface_t* s, int face)
{
	//return Call_Function<void, msurface_t*, int>(0x47900, s, face);

	glpoly_t* polygs = s->polys;
	float (*vert)[7] = polygs->verts;
	texture_t* texanimation;
	if (currententity->curstate.rendermode == 1)
	{
		GL_DisableMultitexture();
		qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		qglEnable(GL_BLEND);
		int texnum = R_TextureAnimation(s)->gl_texturenum;
		GL_Bind(texnum);
		qglDisable(GL_TEXTURE_2D);
		DrawGLSolidPoly(s->polys); // Implement
		GL_EnableMultitexture();
	}
	else if ((s->flags & 0x94) != 0)
	{
		if ((s->flags & 0x10) != 0)
		{
			GL_DisableMultitexture();
			int texnum = s->texinfo->texture->gl_texturenum;
			GL_Bind(texnum);
			EmitWaterPolys(s, face);
		}
	}
	else
	{
		if (!filterMode)
			R_RenderDynamicLightmaps(s);

		int rendermode;
		if (gl_mtexable && ((rendermode = currententity->curstate.rendermode) == kRenderNormal || rendermode == kRenderTransAlpha))
		{
			texanimation = R_TextureAnimation(s);
			GL_SelectTexture(TEXTURE0_SGIS);
			GL_Bind(texanimation->gl_texturenum);
			qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448);

			if (currententity->curstate.rendermode == kRenderFxPulseSlow)
				qglDisable(GL_TEXTURE_2D);

			GL_EnableMultitexture();

			if (filterMode)
				GL_Bind(gl_FilterTexture);
			else
				GL_Bind(lightmap_textures[s->lightmaptexturenum]);

			int lmtexnum = s->lightmaptexturenum;
			if (lightmap_modified[lmtexnum])
			{
				lightmap_modified[lmtexnum] = false;

				glRect_t* lmrect = &lightmap_rectchange[lmtexnum];

				GLvoid* pixels = &lightmaps[lightmap_bytes * ((lightmap_rectchange[lmtexnum].t + (lmtexnum << 7)) << 7)];
				qglTexSubImage2D(GL_TEXTURE_2D, 0, 0,
					lightmap_rectchange[lmtexnum].t, 128,
					lightmap_rectchange[lmtexnum].h, gl_lightmap_format,
					GL_UNSIGNED_BYTE, pixels);

				lmrect->l = lmrect->t = 128;
				lmrect->h = lmrect->w = 0;
			}

			qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448);

			int renderstate = DT_SetRenderState(texanimation->gl_texturenum);

			qglBegin(GL_POLYGON);
			
			for (int i = 0; i < polygs->numverts; i++)
			{
				qglMTexCoord2fSGIS(TEXTURE0_SGIS, vert[i][3], vert[i][4]);
				qglMTexCoord2fSGIS(TEXTURE1_SGIS, vert[i][5], vert[i][6]);
				if (renderstate)
				{
					float u;
					if ((s->flags & 0x20) != 0)
						u = ScrollOffset(s, currententity) + vert[i][3];
					else
						u = vert[i][3];

					float v = vert[i][5];
					DT_SetTextureCoordinates(u, v);
				}
				qglVertex3fv(vert[i]);
			}
			
			qglEnd();
			if (renderstate)
				DT_ClearRenderState();

			if (gl_texsort == false && s->pdecals)
			{
				gDecalSurfs[gDecalSurfCount] = s;
				if (++gDecalSurfCount > MAX_DECAL_SURFACES - 1)
					Sys_Error("%s: Too many decal surfaces!\n", __func__);

				if (!currententity->curstate.rendermode)
					R_DrawDecals(true);

				if (gl_wireframe.value > 0)
				{
					qglDisable(GL_TEXTURE_2D);
					qglColor3f(1.0, 1.0, 1.0);

					if (gl_wireframe.value == 2.0)
						qglDisable(GL_DEPTH_TEST);

					qglBegin(GL_POINT_BIT);

					for (int i = 0; i < polygs->numverts; i++)
						qglVertex3fv(vert[i]);

					qglEnd();
					qglEnable(GL_TEXTURE_2D);
					if (gl_wireframe.value == 2.0)
						qglEnable(GL_DEPTH_TEST);
				}
			}
			// return
		}
		else
		{
			GL_DisableMultitexture();
			texanimation = R_TextureAnimation(s);
			GL_Bind(texanimation->gl_texturenum);
			if ((s->flags & 0x20) != 0)
			{
				DrawGLPolyScroll(s, currententity);
			}
			else
			{
				qglBegin(GL_POLYGON);

				for (int i = 0; i < polygs->numverts; i++)
				{
					qglTexCoord2f(vert[i][3], vert[i][4]);
					qglVertex3fv(vert[i]);
				}

				qglEnd();
			}

			if (!gl_texsort && s->pdecals)
			{
				gDecalSurfs[gDecalSurfCount] = s;
				if (++gDecalSurfCount > MAX_DECAL_SURFACES - 1)
					Sys_Error("%s: Too many decal surfaces!\n", __func__);

				R_DrawDecals(false);

				if (gl_wireframe.value != 0.0)
				{
					qglDisable(GL_TEXTURE_2D);
					qglColor3f(1.0, 1.0, 1.0);

					if (gl_wireframe.value == 2.0)
						qglDisable(GL_DEPTH_TEST);

					qglBegin(GL_POINT_BIT);

					for (int i = 0; i < polygs->numverts; i++)
						qglVertex3fv(vert[i]);

					qglEnd();
					qglEnable(GL_TEXTURE_2D);
					if (gl_wireframe.value == 2.0)
						qglEnable(GL_DEPTH_TEST);
				}
				if (!currententity->curstate.rendermode)
				{
					if (filterMode)
						GL_Bind(gl_FilterTexture);
					else
						GL_Bind(lightmap_textures[s->lightmaptexturenum]);

					qglEnable(GL_BLEND);
					qglBegin(GL_POLYGON);

					for (int i = 0; i < polygs->numverts; i++)
					{
						qglTexCoord2f(vert[i][5], vert[i][6]);
						qglVertex3fv(vert[i]);
					}

					qglEnd();
					qglDisable(GL_BLEND);
				}
			}
		}
	}
}

void DrawTextureChains()
{
	int extraupdtcount = 100;

	currententity = cl_entities;

	if (!gl_texsort)
	{
		GL_DisableMultitexture();
		if (skychain)
		{
			R_DrawSkyChain(skychain);
			skychain = nullptr;
		}
		if (waterchain)
		{
			R_DrawWaterChain(waterchain);
			waterchain = nullptr;
		}
	}

	model_t* model = g_pcl.worldmodel;
	for (int i = 0; i < model->numtextures; i++)
	{
		texture_t* texture = model->textures[i];
		if (texture == nullptr)
			continue;

		msurface_t* chain = texture->texturechain;
		if (chain == nullptr)
			continue;

		if (i == skytexturenum)
		{
			R_DrawSkyChain(chain);

			texture->texturechain = nullptr;
			;
			if (extraupdtcount-- <= 0)
			{
				S_ExtraUpdate();
				extraupdtcount = 100;
			}
		}
		if (i != mirrortexturenum || r_mirroralpha.value == 1.0)
		{
			if ((chain->flags & 0x10) != 0 && r_wateralpha.value != 1.0)
				continue;
			do
			{
				R_RenderBrushPoly(chain);
				chain = chain->texturechain;
			} while (chain);

			texture->texturechain = 0;
			if (extraupdtcount-- <= 0)
			{
				S_ExtraUpdate();
				extraupdtcount = 100;
			}
		}
		R_MirrorChain(texture->texturechain);
	}
}

void R_RenderBrushPoly(msurface_t* fa)
{
	TO_IMPLEMENT;
	return Call_Function<void, msurface_t*>(0x488B0, fa);
}

void R_MirrorChain(msurface_t* s)
{
	if (!mirror)
	{
		mirror = true;
		mirror_plane = s->plane;
	}
}

void DrawGLSolidPoly(glpoly_t* p)
{
	TO_IMPLEMENT;
	return Call_Function<void, glpoly_t*>(0x483C0, p);
}

void R_DrawSkyChain(msurface_t* s)
{
	//	reyurn Call_Function<void, msurface_t*>(0x5054E, s);
	if (CL_IsDevOverviewMode())
		return;

	vec_t verts[64][3];

	c_sky = 0;
	for (msurface_t* surf = s; surf; surf = surf->texturechain)
	{
		for (glpoly_t* poly = surf->polys; poly; poly = poly->next)
		{
			for (int i = 0; i < poly->numverts; i++)
			{
				float* polyverts = poly->verts[i];
				VectorSubtract(polyverts, r_origin, verts[i]);
			}
			ClipSkyPolygon(poly->numverts, (vec_t*)verts, 0);
		}
	}
	R_DrawSkyBox();	// Implement
}

void R_DrawWaterChain(msurface_t* pChain)
{
	//return Call_Function<void, msurface_t*>(0x48A60, pChain);
	
	if (pChain == nullptr)
		return;

	qglColor4ub(255, 255, 255, 255);

	for(; pChain != nullptr; pChain = pChain->texturechain)
	{
		int texnum = pChain->texinfo->texture->gl_texturenum;
		GL_Bind(texnum);
		EmitWaterPolys(pChain, 0);
	}
}

void R_DrawSkyBox()
{
	TO_IMPLEMENT;
	return Call_Function<void>(0x507AE);
}

void MakeSkyVec(float s, float t, int axis)
{
	//return Call_Function<void, float, float, int>(0x5061E, s, t, axis);
	
	int	j, k, farclip;
	vec3_t	v, b;

	farclip = movevars.zmax;

	b[0] = s * (farclip >> 1);
	b[1] = t * (farclip >> 1);
	b[2] = (farclip >> 1);

	for (j = 0; j < 3; j++)
	{
		k = st_to_vec[axis][j];
		v[j] = (k < 0) ? -b[-k - 1] : b[k - 1];
		v[j] += r_origin[j];
	}

	// avoid bilerp seam
	s = (s + 1.0f) * 0.5f;
	t = (t + 1.0f) * 0.5f;

	if (s < 1.0f / 512.0f)
		s = 1.0f / 512.0f;
	else if (s > 511.0f / 512.0f)
		s = 511.0f / 512.0f;
	if (t < 1.0f / 512.0f)
		t = 1.0f / 512.0f;
	else if (t > 511.0f / 512.0f)
		t = 511.0f / 512.0f;

	t = 1.0f - t;

	qglTexCoord2f(s, t);
	qglVertex3fv(v);
}

void ClipSkyPolygon(int nump, vec_t* vecs, int stage)
{
	//return Call_Function<void, int, vec_t*, int>(0x4FFE2, nump, vecs, stage);

	const float* norm;
	float* v, d, e;
	qboolean front, back;
	float	dists[MAX_CLIP_VERTS + 1];
	int	sides[MAX_CLIP_VERTS + 1];
	vec3_t newv[2][MAX_CLIP_VERTS + 1];
	int	newc[2];
	int	i, j;

	if (nump > MAX_CLIP_VERTS)
		Sys_Error("%s: MAX_CLIP_VERTS\n", __func__);
loc1:
	if (stage == 6)
	{
		// fully clipped, so draw it
		DrawSkyPolygon(nump, vecs);
		return;
	}

	front = back = false;
	norm = skyclip[stage];
	for (i = 0, v = vecs; i < nump; i++, v += 3)
	{
		d = _DotProduct(v, norm);
		if (d > ON_EPSILON)
		{
			front = true;
			sides[i] = SIDE_FRONT;
		}
		else if (d < -ON_EPSILON)
		{
			back = true;
			sides[i] = SIDE_BACK;
		}
		else
		{
			sides[i] = SIDE_ON;
		}
		dists[i] = d;
	}

	if (!front || !back)
	{
		// not clipped
		stage++;
		goto loc1;
	}

	// clip it
	sides[i] = sides[0];
	dists[i] = dists[0];
	VectorCopy(vecs, (vecs + (i * 3)));
	newc[0] = newc[1] = 0;

	for (i = 0, v = vecs; i < nump; i++, v += 3)
	{
		switch (sides[i])
		{
		case SIDE_FRONT:
			VectorCopy(v, newv[0][newc[0]]);
			newc[0]++;
			break;
		case SIDE_BACK:
			VectorCopy(v, newv[1][newc[1]]);
			newc[1]++;
			break;
		case SIDE_ON:
			VectorCopy(v, newv[0][newc[0]]);
			newc[0]++;
			VectorCopy(v, newv[1][newc[1]]);
			newc[1]++;
			break;
		}

		if (sides[i] == SIDE_ON || sides[i + 1] == SIDE_ON || sides[i + 1] == sides[i])
			continue;

		d = dists[i] / (dists[i] - dists[i + 1]);
		for (j = 0; j < 3; j++)
		{
			e = v[j] + d * (v[j + 3] - v[j]);
			newv[0][newc[0]][j] = e;
			newv[1][newc[1]][j] = e;
		}
		newc[0]++;
		newc[1]++;
	}

	// continue
	ClipSkyPolygon(newc[0], newv[0][0], stage + 1);
	ClipSkyPolygon(newc[1], newv[1][0], stage + 1);
}

void DrawSkyPolygon(int nump, vec3_t vecs)
{
	//return Call_Function<void, int, vec_t*>(0x4FDA2, nump, vecs);

	int	i, j, axis;
	float	s, t, dv, * vp;
	vec3_t	v, av;

	c_sky++;
	// decide which face it maps to
	VectorCopy(vec3_origin, v);

	for (i = 0, vp = vecs; i < nump; i++, vp += 3)
		VectorAdd(vp, v, v);

	av[0] = fabs(v[0]);
	av[1] = fabs(v[1]);
	av[2] = fabs(v[2]);

	if (av[0] > av[1] && av[0] > av[2])
		axis = (v[0] < 0) ? 1 : 0;
	else if (av[1] > av[2] && av[1] > av[0])
		axis = (v[1] < 0) ? 3 : 2;
	else axis = (v[2] < 0) ? 5 : 4;
	
	// project new texture coords
	for (i = 0; i < nump; i++, vecs += 3)
	{
		j = vec_to_st[axis][2];
		dv = (j > 0) ? vecs[j - 1] : -vecs[-j - 1];

		if (dv == 0.0f) continue;

		j = vec_to_st[axis][0];
		s = (j < 0) ? -vecs[-j - 1] / dv : vecs[j - 1] / dv;

		j = vec_to_st[axis][1];
		t = (j < 0) ? -vecs[-j - 1] / dv : vecs[j - 1] / dv;

		if (s < skymins[0][axis]) skymins[0][axis] = s;
		if (t < skymins[1][axis]) skymins[1][axis] = t;
		if (s > skymaxs[0][axis]) skymaxs[0][axis] = s;
		if (t > skymaxs[1][axis]) skymaxs[1][axis] = t;
	}
}

void AddTEntity(cl_entity_t* pEnt)
{
	//return Call_Function<void, cl_entity_t*>(0x888D0, pEnt);

	model_s* model;
	vec3_t v;
	float distance; 

	if (numTransObjs >= maxTransObjs)
		Sys_Error("%s: Too many objects", __func__);

	model = pEnt->model;
	if (model && model->type == mod_brush && pEnt->curstate.rendermode == 4)
	{
		distance = 1000000000;
	}
	else
	{
		VectorAdd(model->maxs, model->mins, v);
		VectorScale(v, 0.5, v);
		VectorAdd(pEnt->origin, v, v);
		VectorSubtract(r_origin, v, v);
		distance = _DotProduct(v, v);
	}

	int i = numTransObjs;
	transObjRef* pObj = &transObjects[i - 1];

	for (i = numTransObjs; i > 0; i--)
	{
		if (distance < pObj->distance)
			break;

		pObj[1].distance = pObj->distance;
		pObj[1].pEnt = pObj->pEnt;
		pObj--;
	}

	numTransObjs++;
	transObjects[i].pEnt = pEnt;
	transObjects[i].distance = distance;	
}

void R_DrawBrushModel(cl_entity_t* e)
{
	TO_IMPLEMENT;
	return Call_Function<void, cl_entity_t*>(0x48D30, e);
}

float* R_GetAttachmentPoint(int entity, int attachment)
{
	if (attachment > 0)
		return &cl_entities[entity].angles[3 * attachment];
	return cl_entities[entity].origin;
}

void R_DrawSpriteModel(cl_entity_t* e)
{
	//return Call_Function<void, cl_entity_t*>(0x43D70, e);

	float scale;
	colorVec color;
	vec3_t point;
	vec3_t forward;
	vec3_t right;
	vec3_t up;

	msprite_t* pSprite = (msprite_t*)e->model->cache.data;
	mspriteframe_t* pSpriteFrame = R_GetSpriteFrame(pSprite, e->curstate.frame);
	if (!pSpriteFrame)
	{
		Sys_Warning("%s: couldn't get sprite frame for %s\n", __func__, e->model->name);
		return;
	}

	scale = e->curstate.scale;
	if (scale <= 0)
		scale = 1;

	int rendermode = e->curstate.rendermode;
	if (rendermode == kRenderNormal)
		r_blend = 1.0;

	R_SpriteColor(&color, e, r_blend * 255);	// Implement

	if (gl_spriteblend.value == 0.0 && rendermode == kRenderNormal)
	{
		GL_TEXTURE_MAG_FILTER;
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448.0);
		qglColor4ub(color.r, color.g, color.b, 255);
		qglDisable(GL_BLEND);
	}
	else
	{
		switch (rendermode)
		{
		case kRenderTransColor:
			qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			qglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 6406);
			qglColor4ub(color.r, color.g, color.b, 255 * r_blend);
			break;

		case kRenderTransAdd:
			qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448);
			qglBlendFunc(GL_ONE, GL_ONE);
			qglColor4ub(color.r, color.g, color.b, 255);
			qglDepthMask(GL_ZERO);
			break;
		case kRenderGlow:
			qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448);
			qglBlendFunc(GL_ONE, GL_ONE);
			qglColor4ub(color.r, color.g, color.b, 255);
			qglDisable(GL_DEPTH_TEST);
			qglDepthMask(GL_ZERO);
			break;
		case kRenderTransAlpha:
			qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448);
			qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			qglColor4ub(color.r, color.g, color.b, 255 * r_blend);
			qglDepthMask(GL_ZERO);
			break;

		default:
			qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448);
			qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			qglColor4ub(color.r, color.g, color.b, 255 * r_blend);
		}
		qglEnable(GL_BLEND);
	}

	R_GetSpriteAxes(e, pSprite->type, forward, right, up);

	GL_DisableMultitexture();

	GL_Bind(pSpriteFrame->gl_texturenum);

	qglEnable(GL_ALPHA_TEST);
	qglBegin(GL_QUADS);
	qglTexCoord2f(0.0, 1.0);

	VectorMA(r_entorigin, scale * pSpriteFrame->down, up, point);
	VectorMA(point, scale * pSpriteFrame->left, right, point);
	qglVertex3fv(point);

	qglTexCoord2f(0, 0);

	VectorMA(r_entorigin, scale * pSpriteFrame->up, up, point);
	VectorMA(point, scale * pSpriteFrame->left, right, point);
	qglVertex3fv(point);

	qglTexCoord2f(1, 0);

	VectorMA(r_entorigin, scale * pSpriteFrame->up, up, point);
	VectorMA(point, scale * pSpriteFrame->right, right, point);
	qglVertex3fv(point);

	qglTexCoord2f(1, 1);

	VectorMA(r_entorigin, scale * pSpriteFrame->down, up, point);
	VectorMA(point, scale * pSpriteFrame->right, right, point);
	qglVertex3fv(point);

	qglEnd();
	qglDisable(GL_ALPHA_TEST);
	qglDepthMask(GL_ONE);
	if (e->curstate.rendermode || gl_spriteblend.value != 0)
	{
		qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 7681);
		qglDisable(GL_BLEND);
		qglEnable(GL_DEPTH_TEST);
	}
}

void R_SpriteColor(colorVec* pColor, cl_entity_t* pEntity, int alpha)
{
	//return Call_Function<void, colorVec*, cl_entity_t*, int>(0x26F90, pColor, pEntity, alpha);

	int rendermode = pEntity->curstate.rendermode;
	uint a = 255;
	if (rendermode == kRenderTransAdd || rendermode == kRenderGlow)
		a = alpha;

	if (pEntity->curstate.rendercolor.r || pEntity->curstate.rendercolor.g || pEntity->curstate.rendercolor.b)
	{
		pColor->r = (a * pEntity->curstate.rendercolor.r) >> 8;
		pColor->g = (a * pEntity->curstate.rendercolor.g) >> 8;
		pColor->b = (a * pEntity->curstate.rendercolor.b) >> 8;

		if (filterMode)
		{
			pColor->r *= filterColorRed;
			pColor->g *= filterColorGreen;
			pColor->b *= filterColorBlue;
		}
	}
	else
	{
		pColor->r = (255 * a) >> 8;
		pColor->g = (255 * a) >> 8;
		pColor->b = (255 * a) >> 8;
	}

}

void R_GetSpriteAxes(cl_entity_t* pEntity, int type, vec_t* forward, vec_t* right, vec_t* up)
{
	TO_IMPLEMENT;
	return Call_Function<void, cl_entity_t*, int, vec_t*, vec_t*, vec_t*>(0x26D20,
		pEntity, type, forward, right, up);
}

texture_t* R_TextureAnimation(msurface_t* s)
{
	//return Call_Function<texture_t*, msurface_t*>(0x47750, s);
	
	static int rtable[MOD_FRAMES][MOD_FRAMES] = { 0 };
	
	texture_t* base = s->texinfo->texture;
	if (rtable[0][0] == 0)
	{
		for (int i = 0; i < MOD_FRAMES; i++)
			for (int j = 0; j < MOD_FRAMES; j++)
				rtable[i][j] = RandomLong(0, MAX_SIGNED_SHORT);
	}
	if (currententity->curstate.frame != 0.0 && base->alternate_anims)
		base = base->alternate_anims;

	int anim_total = base->anim_total;
	if (!anim_total)
		return base;

	int reletive;
	int i = 0;
	if (base->name[0] == '-')
	{
		int	tx = ((s->texturemins[0] + (base->width << 16)) / base->width) % MOD_FRAMES;
		int	ty = ((s->texturemins[1] + (base->height << 16)) / base->height) % MOD_FRAMES;
		reletive = rtable[tx][ty] % base->anim_total;
	}
	else
	{
		float speed = 10;
		reletive = (int)(g_pcl.time * speed) % anim_total;
	}
	while (base->anim_min > reletive || base->anim_max <= reletive)
	{
		base = base->anim_next;
		if (base == nullptr)
			Sys_Error("%s: broken cycle", __func__);
		if (++i > 100)
			Sys_Error("%s: infinite cycle", __func__);
	}
	return base;
}

void EmitWaterPolys(msurface_t* fa, int direction)
{
	float tempVert[3];

	uchar* pPal = fa->texinfo->texture->pPal;

	glpoly_t* poly = fa->polys;

	float scale = currententity->curstate.scale;

	D_SetFadeColor(pPal[9], pPal[10], pPal[11], pPal[12]);

	if (r_refdef.vieworg[2] <= poly->verts[0][2])
		scale = -scale;

	for (; poly != nullptr; poly = poly->next)
	{
		qglBegin(GL_POLYGON);

		int numverts = poly->numverts;
		float* verts;
		if (direction)
			verts = poly->verts[numverts - 1];
		else
			verts = (float*)poly->verts;
	
		for (int i = 0; i < numverts; i++)
		{
			tempVert[0] = *verts;
			tempVert[1] = verts[1];
			float time = g_pcl.time;
			float nv = turbsin[(uchar)(time * 160.0 + verts[0] + verts[1])] + 8.0;
			nv += (turbsin[(uchar)(time * 171.0 + verts[0] * 5.0 - verts[1])] + 8.0) * 0.8;

			tempVert[2] = nv * scale + verts[2];

			float t = (verts[4] + turbsin[(uchar)((time + 0.125 * verts[3]) * TURBSCALE)])
				* 0.015625;
			float s = (turbsin[(uchar)((0.125 * verts[4] + time) * TURBSCALE)] + verts[3])
				* 0.015625;

			qglTexCoord2f(s, t);

			qglVertex3fv(tempVert);
			if (direction)
				verts -= 7;
			else
				verts += 7;
		}
		qglEnd();
	}
	EmitWireFrameWaterPolys(fa, direction);
}

void EmitWireFrameWaterPolys(msurface_t* fa, int direction)
{
	//return Call_Function<void, msurface_t*, int>(0x4F002, fa, direction);
	
	if (gl_wireframe.value == 0)
		return;

	float tempVert[3];
	
	float scale;
	if (r_refdef.vieworg[2] <= fa->polys->verts[0][2])
		scale = -currententity->curstate.scale;
	else
		scale = currententity->curstate.scale;
	
	if (gl_wireframe.value == 2)
		qglDisable(GL_DEPTH_TEST);

	qglColor3f(1.0, 1.0, 1.0);
	for (glpoly_t* poly = fa->polys; poly != nullptr; poly = poly->next)
	{
		qglBegin(GL_POINT_BIT);
		int numverts = poly->numverts;
		float* verts;
		if (direction)
			verts = poly->verts[numverts - 1];
		else
			verts = (float*)poly->verts;

		for (int i = 0; i < numverts; i++)
		{
			tempVert[0] = verts[0];
			tempVert[1] = verts[1];
			float nv = turbsin[(uchar)(g_pcl.time * 160.0 + verts[0] + verts[1])] + 8;
			nv += (turbsin[(uchar)(g_pcl.time * 171.0 + verts[0] * 5.0 - verts[1])] + 8) * 0.8f;
			tempVert[2] = nv * scale + verts[2];
			qglVertex3fv(tempVert);
			if (direction)
				verts -= 7;
			else
				verts += 7;
		}
		qglEnd();
	}
	if (gl_wireframe.value == 2)
		qglEnable(GL_DEPTH_TEST);
}

void D_SetFadeColor(int r, int g, int b, int fog)
{
	gWaterColor.r = r;
	gWaterColor.g = g;
	gWaterColor.b = b;

	cshift_water.destcolor[0] = r;
	cshift_water.destcolor[1] = g;
	cshift_water.destcolor[2] = b;
	cshift_water.percent = fog;
}

void R_RenderDynamicLightmaps(msurface_t* fa)
{
	//return Call_Function<void, msurface_t*>(0x47F00, fa);

	c_brush_polys++;
	if ((fa->flags & 0x14) != 0)
		return;

	int lmtexnum = fa->lightmaptexturenum;
	glpoly_t* polys = fa->polys;
	uint* cached_light = (uint*)fa->cached_light;
	polys->chain = lightmap_polys[lmtexnum];
	lightmap_polys[lmtexnum] = polys;
	
	uchar* styles = fa->styles;
	for (int i = 0; i < 5; i++)
	{
		if (i == 4 || styles[i] == 255)
		{
			if (fa->dlightframe != r_framecount && !fa->cached_dlight)
				return;
			break;
		}
		if (d_lightstylevalue[styles[i]] != cached_light[i])
			break;
	}
	if (r_dynamic.value == 0.0)
		return;

	int light_s = fa->light_t;
	glRect_t* lmrect = &lightmap_rectchange[lmtexnum];
	lightmap_modified[lmtexnum] = 1;

	int h = lmrect->h;
	int t = lmrect->t;
	int w = lmrect->w;
	int l = lmrect->l;

	if (light_s < t)
	{
		if (h)
			h = lmrect->h += lmrect->t + - light_s;
		lmrect->t = t = light_s;
	}

	if (light_s < lmrect->l)
	{
		if (w > 0)
			w = lmrect->w += l + - light_s;
		lmrect->l = l = light_s;
	}

	int smax = (fa->extents[0]  / 16) + 1;
	int tmax = (fa->extents[1] / 16) + 1;
	int light_t = fa->light_t;
	if (l + w < smax + light_s)
		lmrect->w = smax + light_s - lmrect->l;
	if (t + h < tmax + light_t)
		lmrect->h = tmax + light_t - t;

	R_BuildLightMap(fa, &lightmaps[lightmap_bytes * (fa->light_s + ((fa->light_t + (lmtexnum << 7)) << 7))],
		lightmap_bytes << 7);
}

void R_DrawDecals(qboolean bMultitexture)
{
	//return Call_Function<void, qboolean>(0x4B890, bMultitexture);

	if (gDecalSurfCount <= 0)
		return;

	qglEnable(GL_BLEND);
	qglEnable(GL_ALPHA_TEST);
	qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	qglDepthMask(GL_ZERO);
	float polyoffset = gl_polyoffset.value;
	if (polyoffset != 0.0)
	{
		qglEnable(GL_POLYGON_OFFSET_FILL);
		float polyoffset = gl_polyoffset.value;
		if (gl_ztrick.value == 0.0 || gldepthmin < 0.5)
		{
			qglPolygonOffset(-1.0, -polyoffset);
		}
		else
		{
			qglPolygonOffset(1.0, polyoffset);
		}
	}

	for (int i = 0; i < gDecalSurfCount; i++)
	{
		msurface_t* surf = gDecalSurfs[i];
		if (surf->pdecals == nullptr)
			continue;

		for (decal_t* decal = surf->pdecals; decal != nullptr; decal = decal->pnext)
		{
			texture_t* ptexture = Draw_DecalTexture(decal->texture); // Check
			float* verts = nullptr;
			int vertCount = 0;
			if ((decal->flags & 0x40) == 0)
			{
				verts = R_DecalVertsClip(0, decal, surf, ptexture, &vertCount); // Implement
				if (vertCount > 0 && bMultitexture)
				{
					R_DecalVertsLight(verts, surf, vertCount); // Implement
				}
				if (vertCount == 0)
					continue;
			}
			else
			{
				verts = R_DecalVertsNoclip(decal, surf, ptexture, bMultitexture); // Implement
				vertCount = 4;
			}
			if (bMultitexture)
				R_DecalMPoly(verts, ptexture, surf, vertCount); // Implement
			else
				R_DecalPoly(verts, ptexture, surf, vertCount); // Implement
		}
	}

	if (polyoffset != 0.0)
		qglDisable(GL_POLYGON_OFFSET_FILL);

	qglDisable(GL_ALPHA_TEST);
	qglDisable(GL_BLEND);
	qglDepthMask(GL_ONE);
	gDecalSurfCount = 0;
}

void R_DrawTEntitiesOnList(qboolean clientOnly)
{
	return Call_Function<void, qboolean>(0x88AF0, clientOnly);
}

void R_RenderDlights()
{
	//return Call_Function<void>(0x430F0);

	if (gl_flashblend.value == 0.0)
		return;

	NOT_TESTED; // Never executed code since gl_flashblend never changed
	r_dlightframecount = r_framecount + 1;
	qglDepthMask(GL_ZERO);
	qglDisable(GL_TEXTURE_2D);
	qglShadeModel(GL_SMOOTH);
	qglEnable(GL_BLEND);
	qglBlendFunc(GL_ONE, GL_ONE);
	for (int i = 0; i < ARRAYSIZE(cl_dlights); i++)
	{
		dlight_t& dlight = cl_dlights[i];
		if (g_pcl.time <= dlight.die && dlight.radius > 0)
			R_RenderDlight(&dlight);

	}
	qglColor3f(1.0, 1.0, 1.0);
	qglDisable(GL_BLEND);
	qglEnable(GL_TEXTURE_2D);
	qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	qglDepthMask(GL_ONE);
}

void R_MarkLights(dlight_t* light, int bit, mnode_t* node)
{
	return Call_Function<void, dlight_t*, int, mnode_t*>(0x431D0, light, bit, node);
}

void R_DrawParticles()
{
	TO_IMPLEMENT;
	return Call_Function<void>(0x7CB20);
}

void R_BuildLightMap(msurface_t* psurf, uchar* dest, int stride)
{
	//return Call_Function<void, msurface_s*, uchar*, int>(0x47390, psurf, dest, stride);
	
	int smax;
	int tmax;
	color24* samples;
	int size;
	int lightscale;

	if (gl_overbright.value == 0.0 || !gl_texsort)
		lightscale = pow(2.0, 1.0 / v_lightgamma.value) * 256.0 + 0.5;
	else
		lightscale = 256;

	smax = (psurf->extents[0] / 16) + 1;
	tmax = (psurf->extents[1] / 16) + 1;
	samples = psurf->samples;
	size = smax * tmax;
	psurf->dlightbits = psurf->cached_dlight = r_dlightactive & psurf->dlightbits;
	if (smax * tmax > MAX_LIGHTMAP)
		Sys_Error("Error: lightmap for texture% s too large(% d x % d = % d luxels); cannot exceed% d\n",
			psurf->texinfo->texture->name, smax, tmax, size, MAX_LIGHTMAP);

	if (filterMode)
	{
		for (int i = 0; i < size; i++)
		{
			blocklights[i].r = filterColorRed * 65280.0 * filterBrightness;
			blocklights[i].g = filterColorGreen * 65280.0 * filterBrightness;
			blocklights[i].b = filterBrightness * 65280.0 * filterColorBlue;
		}
	}
	else if (r_fullbright.value == 0.0 && g_pcl.worldmodel->lightdata)
	{
		for (int i = 0; i < size; i++)
		{
			blocklights[i].r = blocklights[i].g = blocklights[i].b = 0;
		}

		if (samples)
		{
			for (int i = 0; i < 4; i++)
			{
				uchar style = psurf->styles[i];
				if (style == 255)
					break;

				int light = psurf->cached_light[i] = d_lightstylevalue[style];

				for (int j = 0; j < size; j++)
				{
					blocklights[j].r += light * samples[j + (i * size)].r;
					blocklights[j].g += light * samples[j + (i * size)].g;
					blocklights[j].b += light * samples[j + (i * size)].b;
				}
			}
		}
		if (psurf->dlightframe == r_framecount)
		{
			R_AddDynamicLights(psurf);
		}
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			blocklights[i].r = blocklights[i].g = blocklights[i].b = DEFAULT_BLOCKLIGHT;
		}
	}
	if (gl_lightmap_format == GL_RGBA)
	{
		uchar* lightmap = dest;
		for (int i = 0; i < tmax; i++, lightmap += stride - 4 * smax)
		{
			for (int j = 0; j < smax; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					int iLightgamma = (lightscale * *(&blocklights[(i * smax) + j].r + k)) / 16384;
					if (iLightgamma > 1023)
						iLightgamma = 1023;
					lightmap[k] = lightgammatable[iLightgamma] / 4;
				}

				if (gl_monolights.value == 0.0)
				{
					lightmap[3] = 255;
				}
				else
				{
					lightmap[3] = ~((149 * lightmap[1] + 29 * lightmap[2] + 76 * lightmap[0]) / 256);
					lightmap[0] = lightmap[1] = lightmap[2] = 255;
				}
				lightmap += 4;
			}
		}
	}
	else
	{
		NOT_TESTED; // Never executed code
		if (gl_lightmap_format != GL_LUMINANCE && gl_lightmap_format != GL_INTENSITY)
			Sys_Error("Bad lightmap format");

		uchar* lightmap = dest;
		int j;
		for (int i = 0; i < tmax; i++, lightmap += stride)
		{
			for (j = 0; j < smax; j++)
			{
				int red = blocklights[(i * smax) + j].r;
				uchar v = red / 256;
				if (v > 255)
					v = 255;
				lightmap[j] = 255 - v;
			}
		}
	}
}

void R_AddDynamicLights(msurface_t* surf)
{
	//return Call_Function<void, msurface_t*>(0x47110, surf);

	mtexinfo_t* texinfo = surf->texinfo;
	int smax = (surf->extents[0] / SAMPLE_SIZE) + 1;
	int tmax = (surf->extents[1] / SAMPLE_SIZE) + 1;

	for (int lnum = 0; lnum < 32; ++lnum)
	{
		if (((1 << lnum) & surf->dlightbits) != 0)
		{
			vec3_t origin_l;
			VectorSubtract(cl_dlights[lnum].origin, currententity->origin, origin_l);

			float radius = cl_dlights[lnum].radius;
			float dist = abs(_DotProduct(origin_l, surf->plane->normal) - surf->plane->dist);
			float minlight = cl_dlights[lnum].minlight;
			float rad = radius - dist;

			if (rad < minlight)
				continue;

			minlight = rad - minlight;

			float tl = _DotProduct(origin_l, texinfo->vecs[0]) + texinfo->vecs[0][3] - surf->texturemins[0];
			float sl = _DotProduct(origin_l, texinfo->vecs[1]) + texinfo->vecs[1][3] - surf->texturemins[1];

			for (int i = 0; i < tmax; i++)
			{
				int sd = abs(sl - (i * SAMPLE_SIZE));

				for (int j = 0; j < smax; j++)
				{
					int td = abs(tl - (j * SAMPLE_SIZE));
					double dist;
					if (td <= sd)
						dist = sd + (td / 2);
					else
						dist = td + (sd / 2);

					if (dist < minlight)
					{
						int blindex = (i * smax) + j;
						blocklights[blindex].r += ((int((rad - dist) * 256 * cl_dlights[lnum].color.r))) / 256;
						blocklights[blindex].g += ((int((rad - dist) * 256 * cl_dlights[lnum].color.g))) / 256;
						blocklights[blindex].b += ((int((rad - dist) * 256 * cl_dlights[lnum].color.b))) / 256;
					}

				}
			}
		}
	}
}

void R_RenderDlight(dlight_t* light)
{
	// called by R_RenderDlights which is nvever executed 
	return Call_Function<void, dlight_t*>(0x42F90, light);
}

void DrawGLWaterPolyLightmap(glpoly_t* p)
{
	NOT_TESTED;

	vec3_t nv;

	qglBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < p->numverts; i++)
	{
		float* verts = p->verts[i];
		qglTexCoord2f(verts[5], verts[6]);

		nv[0] = sin(0.05 * verts[1] + realtime) * 8.0 * sin(0.05 * verts[2] + realtime) + verts[0];
		nv[1] = sin(verts[0] * 0.05 + realtime) * 8.0 * sin(0.05 * verts[2] + realtime) + verts[1];
		nv[2] = verts[2];
		qglVertex3fv(nv);
	}
	qglEnd();
}

void R_DrawWorld()
{
	//return Call_Function<void>(0x49440);

	cl_entity_t ent;
	Q_memset(&ent, 0, sizeof(cl_entity_t));
	currententity = &ent;
	ent.model = g_pcl.worldmodel;

	VectorCopy(r_refdef.vieworg, modelorg);
	
	currenttexture = -1;
	color24& rendercolor = ent.curstate.rendercolor;
	rendercolor.r = gWaterColor.r;
	rendercolor.g = gWaterColor.g;
	rendercolor.b = gWaterColor.b;

	qglColor3f(1.0, 1.0, 1.0);
	Q_memset(lightmap_polys, 0, 256);
	R_ClearSkyBox();
	if (!gl_texsort)
	{
		switch (gl_lightmap_format)
		{
		case GL_LUMINANCE:
			qglBlendFunc(0, GL_ONE_MINUS_SRC_COLOR);
			break;
		case GL_INTENSITY:
			qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448.0);
			qglColor4f(0.0, 0.0, 0.0, 1.0);
			qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
			break;
		case GL_RGBA:
			qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448.0);
			break;
		}
	}

	gDecalSurfCount = 0;
	R_RecursiveWorldNode(g_pcl.worldmodel->nodes);
	gDecalSurfCount = 0;
	DrawTextureChains();
	S_ExtraUpdate();
	if (!CL_IsDevOverviewMode())
	{
		R_DrawDecals(false);
		R_BlendLightmaps();
	}
}

void DrawGLPolyScroll(msurface_t* psurface, cl_entity_t* pEntity)
{
	//return Call_Function<void, msurface_t*, cl_entity_t*>(0x48350, psurface,pEntity);
	
	NOT_TESTED;
	float sOffset = ScrollOffset(psurface, pEntity);
	glpoly_t* poly = psurface->polys;
	qglBegin(GL_POLYGON);
	for (int i = 0; i < poly->numverts; i++)
	{
		auto verts = poly->verts[i];
		qglTexCoord2f(sOffset + verts[3], verts[4]);
		qglVertex3fv(verts);
	}
	qglEnd();
}

void R_DrawEntitiesOnList()
{	
	//return Call_Function<void>(0x44860);

	cl_entity_t* e;

	for (int i = 0; i < cl_numvisedicts; ++i)
	{
		currententity = e = cl_visedicts[i];
		if (e->curstate.rendermode != kRenderNormal)
		{
			AddTEntity(e);
		}
		else
		{
			modtype_t type = e->model->type;
			if (type == mod_studio)
			{

				if (e->player)
				{
					return;
				}
				else if (e->curstate.movetype == 12)
				{

					if (cl_numvisedicts <= 0)
						continue;

					e = nullptr;

					for (int k = 0; k < cl_numvisedicts; k++)
					{
						if (cl_visedicts[k]->index == e->curstate.aiment)
						{
							e = cl_visedicts[k];
							break;
						}
					}
					if (e == nullptr)
						continue;

					currententity = e;
					if (currententity->player)
						return;
					else
						pStudioAPI->StudioDrawModel(0);

					currententity = cl_visedicts[i];
					pStudioAPI->StudioDrawModel(3);
				}
				else
				{
					pStudioAPI->StudioDrawModel(3);
				}

			}
			else if (type == mod_brush)
			{
				R_DrawBrushModel(e);
			}
		}
	}

	r_blend = 1.0;
	for (int i = 0; i < cl_numvisedicts; i++)
	{
		currententity = e = cl_visedicts[i];
		if (e->curstate.rendermode == kRenderNormal && e->model->type == mod_sprite)
		{
			if (e->curstate.body)
			{
				vec_t* attach = R_GetAttachmentPoint(e->curstate.skin, e->curstate.body);
				VectorCopy(attach, r_entorigin);
			}
			else
			{
				VectorCopy(e->origin, r_entorigin);
			}

			R_DrawSpriteModel(e);
		}
	}
}

void R_CheckVariables()
{
	static int oldFilterMode = filterMode;
	static float oldFilterColorRed = filterColorRed;
	static float oldFilterColorGreen = filterColorGreen;
	static float oldFilterColorBlue = filterColorBlue;
	static float oldFilterBrightness = filterBrightness;

	if (filterMode != oldFilterMode
		|| filterColorRed != oldFilterColorRed
		|| filterColorGreen != oldFilterColorGreen
		|| filterColorBlue != oldFilterColorBlue
		|| filterBrightness != oldFilterBrightness)
	{
		oldFilterMode = filterMode;
		oldFilterColorRed = filterColorRed;
		oldFilterColorGreen = filterColorGreen;
		oldFilterColorBlue = filterColorBlue;
		oldFilterBrightness = filterBrightness;

		GL_LoadFilterTexture(filterColorRed, filterColorGreen, filterColorBlue, filterBrightness);
	}
}

void R_AnimateLight()
{	
	//Call_Function<void>(0x42eb0);

	for (int i = 0; i < ARRAYSIZE(d_lightstylevalue); i++)
	{
		int len = cl_lightstyle[i].length;
		if (len > 0)
		{
			d_lightstylevalue[i] = 22 * (cl_lightstyle[i].map[(int)(10 * g_pcl.time) % len] - 97);
		}
		else
		{
			d_lightstylevalue[i] = 256;
		}
	}
}

void GL_LoadFilterTexture(float r, float g, float b, float brightness)
{
	uchar* texbuf = (uchar*)Mem_Malloc(192);
	for (int i = 0; i < 192;)
	{
		texbuf[i++] = r * brightness * 255;
		texbuf[i++] = g * brightness * 255;
		texbuf[i++] = b * brightness * 255;
	}
	
	if (gl_FilterTexture)
	{
		GL_Bind(gl_FilterTexture);
		qglTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 8, 8, GL_RGB, GL_UNSIGNED_BYTE, texbuf);
	}
	else
	{
		gl_FilterTexture = GL_GenTexture();
		GL_Bind(gl_FilterTexture);
		qglTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGB, GL_UNSIGNED_BYTE, texbuf);
		qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, gl_ansio.value);

	}
	Mem_Free(texbuf);
}

void AllowFog(qboolean allowed)
{
	//return Call_Function<void, int>(0x44FB0, allowed);
	if (allowed)
	{
		if (isFogEnabled)
			qglEnable(GL_FOG);
	}
	else
	{
		isFogEnabled = qglIsEnabled(GL_FOG);
		if (isFogEnabled)
			qglDisable(GL_FOG);
	}
}

float CalcFov(float fov_x, float width, float height)
{
	float v1; 
	if (fov_x < 1.0 || fov_x > 179.0)
		v1 = 1;
	else
		v1 = tan(fov_x / 360.0 * PI);

	float v2 = width / v1;
	float x = height / v2;
	return atanf(x) * 360.0f / PI;
}

float ScrollOffset(msurface_t* psurface, cl_entity_t* pEntity)
{
	//return Call_Function<float, msurface_t*, cl_entity_t*>(0x482A0, psurface, pEntity);

	NOT_TESTED;

	float sOffset = (pEntity->curstate.rendercolor.b + (pEntity->curstate.rendercolor.g << 8)) * 0.0625;
	
	byte r = pEntity->curstate.rendercolor.r;
	if (r <= 0)
		sOffset = -sOffset;

	float sOffseta = 1.0 / (double)(unsigned int)psurface->texinfo->texture->width * sOffset * g_pcl.time;
	
	int power = (sOffseta < 0) ? -1 : 1;

	return fmod(sOffseta, power);
}

void R_ConcatRotations(vec3_t* in1, vec3_t* in2, vec3_t* out)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vec3_t v2 = { in2[0][j], in2[1][j], in2[2][j] };
			out[i][j] = _DotProduct(in1[i], v2);
		}
	}
}

void ProjectPointOnPlane(vec_t* dst, const vec_t* p, const vec_t* normal)
{
	float v5 = 1.0f / _DotProduct(normal, normal);
	float dist = _DotProduct(normal, p) * v5;
	for (int i = 0; i < 3; i++)
		dst[i] = p[i] - (normal[i] * dist * v5);
}

void PerpendicularVector(vec_t* dst, const vec_t* src)
{
	vec3_t tempvec = { 0 };

	for (int i = 0; i < 3; i++)
	{
		if (src[i] <= src[0] && src[i] <= src[1] && src[i] <= src[2])
		{
			tempvec[i] = 1;
			break;
		}
	}

	ProjectPointOnPlane(dst, tempvec, src);
	VectorNormalize(dst);
}

void RotatePointAroundVector(vec3_t* dst, const vec_t* dir, const vec_t* point, float degrees)
{
	//return Call_Function<void, const vec_t*, const vec_t*, const vec_t*, float>(0x43A10, dst, dir, point, degrees);
	
	vec3_t m[3];
	float im[3][3];
	float zrot[3][3];
	float tmpmat[3][3];
	float rot[3][3];
	vec3_t vr;
	vec3_t vup;
	vec3_t vf;
	vec_t p[9] = { 0 };
	VectorCopy(dir, vf);
	PerpendicularVector(vr, dir);
	CrossProduct(vr, vf, vup);
	for (int i = 0; i < 3; i++)
	{
		m[i][0] = vr[i];
		m[i][1] = vup[i];
		m[i][2] = vf[i];
	}
	Q_memcpy(im, m, sizeof(vec3_t[3]));
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j == i)
				continue;

			im[i][j] = m[j][i];
		}
	}

	Q_memset(zrot, 0, sizeof(zrot));
	zrot[2][2] = 1;
	float angle = degrees * PI / 180;
	float s = sin(angle), c = cos(angle);
	zrot[0][0] = zrot[1][1] = c;
	zrot[0][1] = s;
	zrot[1][0] = -s;

	R_ConcatRotations(m, zrot, tmpmat);
	R_ConcatRotations(tmpmat, im, rot);
	for (int i = 0; i < 3; i++)
		(*dst)[i] = _DotProduct(rot[i], point);
}

int SignbitsForPlane(mplane_t* out)
{
	int sign = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (out->normal[i] < 0)
			sign |= 1 << i;
	}
	return sign;
}