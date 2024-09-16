#include "precompiled.h"
#include "render_api.h"


// TODO: Find good name for variables in R_AddDynamicLights, ProjectPointOnPlane, BuildSurfaceDisplayList

const float PI = 3.141592653589793f;
const double FAR_PLANE = 16000.0;
const double NEAR_PLANE = 4;
const int MAX_LIGHTMAP = 324;
const int MAX_DECAL_SURFACES = 512;
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

VVAR(int, r_framecount, 0x42E55, 0);
VVAR(int, trickframe_25783, 0x45D68, 0);
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
VAR(vec3_t, vpn, 0x4568B);
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
ARRAY(glpoly_t*, lightmap_polys, [64], 0x494E0);
ARRAY(float, skymins, [2][6], 0x505FC);
ARRAY(float, skymaxs, [2][6], 0x5060E);
VAR(int, gDecalSurfCount, 0x49577);
VARRAY(msurface_t*, gDecalSurfs, [500], 0x47D74 ,{ nullptr });
VVAR(int, gRenderMode, 0x89257);
ARRAY(mplane_t, frustum, [4], 0x452DC);
VVAR(int, r_dlightactive, 0x47403, 0);
VARRAY(colorVec, blocklights, [MAX_LIGHTMAP], 0x4761D, { 0 });
VVAR(int, cl_numvisedicts, 0xBBB3, 0);
VVAR(int, mirrortexturenum, 0x4938D, 0);
VVAR(msurface_t*, skychain, 0x48AD7, nullptr);
VVAR(msurface_t*, waterchain, 0x493C3, nullptr);
VVAR(int, r_dlightframecount, 0x43112, 0);

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
		CL_SetDevOverView(&r_refdef);	// Implement

	R_SetupFrame();
	R_SetFrustum();
	R_SetupGL();
	R_MarkLeaves();
	if (r_refdef.onlyClientDraws == false)
	{
		if (CL_IsDevOverviewMode())
		{
			qglClearColor(0.0, 1.0, 0.0, 0.0);
			qglClear(GL_COLOR_BUFFER_BIT);
		}
		R_DrawWorld();	// Under implement
		S_ExtraUpdate();
		R_DrawEntitiesOnList();	// Implement
	}
	if (g_bUserFogOn)
		R_RenderFinalFog();

	isFogEnabled = qglIsEnabled(GL_FOG);
	if (r_refdef.onlyClientDraws == false)
	{
		if (CL_IsDevOverviewMode())
		{
			qglClearColor(0, 1, 0, 0);
			qglClear(GL_DEPTH_BUFFER_BIT6_QCOM);
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

	if (g_pcl.waterlevel > 2 && r_refdef.onlyClientDraws == false || !g_bUserFogOn)
		qglDisable(GL_FOG);

	R_DrawTEntitiesOnList(r_refdef.onlyClientDraws);	// Implement
	S_ExtraUpdate();
	if (r_refdef.onlyClientDraws == false)
	{
		R_RenderDlights();
		GL_DisableMultitexture();
		R_DrawParticles();	// Implement
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
	for (int i = 0; i < 16; i++)
		playertextures[i] = GL_GenTexture(); // same?
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
			sizeParticles = 28672;
		}
	}
	else
	{
		r_numparticles = 2048;
		sizeParticles = 114688;
	}
	particles = (particle_t *)Hunk_AllocName(sizeParticles, (char *)"particles");
	gBeams = (BEAM *)Hunk_AllocName(15872, "lightning");
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
		if (!r_worldentity.model || !g_pcl.worldmodel)
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

		if (r_refdef.onlyClientDraws == false)
			R_PreDrawViewModel();	// Implement

		R_RenderScene(); 
		if (r_refdef.onlyClientDraws == false)
		{
			R_DrawViewModel(); // Implement
			R_PolyBlend();	// Implement
		}

		S_ExtraUpdate();

		if (r_speeds.value != 0.0)
		{
			framerate = g_pcl.time - g_pcl.oldtime;
			float fps = framerate;
			if (framerate > 0.0)
				fps = 1.0 / fps;

			float time2 = Sys_FloatTime();
			Con_Printf(
				"%3ifps %3i ms  %4i wpoly %4i epoly\n",
				fps + 0.5,
				(time2 - time1) * 1000,
				c_brush_polys,
				c_alias_polys);
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
}

void R_InitSky() 
{ 
	NOT_IMPLEMENTED;
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
			if ((++trickframe_25783 & 1) != 0)
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
	float xa;
	float xb;
	float fov_x = 90.0;	
	float fov = CalcFov(&fov_x, glwidth, glheight);

	xa = 90 - (scr_fov_value / 2);
	xb = 90 - (fov / 2);

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

#define LODWORD(x) (*((unsigned int*)&(x)))
/*
void R_SetupGL()
{
	int y; // eax
	int v1; // edx
	int v2; // ecx
	GLsizei v3; // esi
	GLsizei value; // ebx
	long double width; // fst7
	long double height; // fst6
	long double v7; // fst5
	long double v8; // fst5
	long double v9; // fst7
	__m128 v10; // xmm5
	__m128 v11; // xmm4
	__m128 v12; // xmm3
	__m128 v13; // xmm1
	__m128 v14; // xmm2
	__m128 v15; // xmm6
	__m128 v16; // xmm5
	__m128 v17; // xmm4
	__m128 v18; // xmm0
	__m128 v19; // xmm0
	__m128 v20; // xmm4
	__m128 v21; // xmm3
	__m128 v22; // xmm2
	__m128 v23; // xmm1
	__m128 v24; // xmm2
	long double zoom; // fst7
	long double v26; // fst7
	__m128 v27; // xmm2
	__m128 v28; // xmm3
	__m128 v29; // xmm4
	__m128 v30; // xmm5
	__m128 v31; // xmm2
	__m128 v32; // xmm0
	__m128 v33; // xmm2
	__m128 v34; // xmm1
	__m128 v35; // xmm5
	__m128 v36; // xmm7
	__m128 v37; // xmm4
	float v38; // [esp+0h] [ebp-8Ch]
	GLfloat v39; // [esp+0h] [ebp-8Ch]
	GLfloat v40; // [esp+0h] [ebp-8Ch]
	GLfloat v41; // [esp+0h] [ebp-8Ch]
	GLfloat v42; // [esp+0h] [ebp-8Ch]
	GLfloat v43; // [esp+4h] [ebp-88h]
	GLfloat v44; // [esp+8h] [ebp-84h]
	float v45; // [esp+30h] [ebp-5Ch]
	float v46; // [esp+50h] [ebp-3Ch]
	float screenaspect; // [esp+60h] [ebp-2Ch]
	float screenaspecta; // [esp+60h] [ebp-2Ch]
	__m128 screenaspectb; // [esp+60h] [ebp-2Ch]
	float v50; // [esp+74h] [ebp-18h]
	float v51; // [esp+7Ch] [ebp-10h]
	float v52; // [esp+7Ch] [ebp-10h]

	qglMatrixMode(0x1701u);
	qglLoadIdentity();
	y = r_refdef.vrect.y;
	v1 = r_refdef.vrect.x - (r_refdef.vrect.x > 0);
	v2 = ((unsigned int)(glheight - (r_refdef.vrect.height + y)) < 0x80000000)
		+ glheight
		- (r_refdef.vrect.height
			+ y)
		- 1;
	if (envmap)
	{
		value = (int)gl_envmapsize.value;
		v2 = 0;
		v1 = 0;
		glheight = value;
		v3 = value;
		glwidth = value;
	}
	else
	{
		v3 = (r_refdef.vrect.x + r_refdef.vrect.width < glwidth) + r_refdef.vrect.x + r_refdef.vrect.width - v1;
		value = (glheight > glheight - y)
			+ glheight
			- (((unsigned int)(glheight - (r_refdef.vrect.height + y)) < 0x80000000)
				+ glheight
				- r_refdef.vrect.height
				- 1);
	}
	qglViewport(glx + v1, gly + v2, v3, value);
	width = (long double)r_refdef.vrect.width;
	height = (long double)r_refdef.vrect.height;
	screenaspect = width / height;
	v7 = scr_fov_value;
	if (scr_fov_value < 1.0)
	{
		v8 = 0.9999999999999999;
	}
	else if (v7 <= 179.0)
	{
		v46 = height;
		v45 = width;
		height = v46;
		v8 = tan(v7 / 360.0 * 3.141592653589793);
		width = v45;
	}
	else
	{
		v8 = 0.9999999999999999;
	}
	v51 = width / v8;
	v38 = height / v51;
	v52 = atanf(LODWORD(v38)) * 360.0 / 3.141592653589793;
	if (r_refdef.onlyClientDraws)
	{
		v9 = tan(v52 * 3.141592653589793 / 360.0) * 4.0;
		qglFrustum(screenaspect * -v9, v9 * screenaspect, -v9, v9, 4.0, 16000.0);
	}
	else
	{
		if (CL_IsDevOverviewMode())
		{
			zoom = gDevOverview.zoom;
			v50 = 4096.0 / zoom;
			screenaspecta = 4096.0 / (zoom * screenaspect);
			qglOrtho(
				(double)-(int)v50,
				(double)(int)v50,
				(double)-(int)screenaspecta,
				(double)(int)screenaspecta,
				16000.0 - gDevOverview.z_min,
				16000.0 - gDevOverview.z_max);
			if (mirror)
				goto LABEL_22;
		LABEL_21:
			qglCullFace(0x404u);
			goto LABEL_13;
		}
		v26 = tan(v52 * (double)3.141592653589793 / (float)360.0) * 4.0;
		qglFrustum(screenaspect * -v26, v26 * screenaspect, -v26, v26, 4.0, movevars.zmax);
	}
	if (mirror == false)
		goto LABEL_21;
LABEL_22:
	if (mirror_plane->normal[2] == 0.0)
		qglScalef(-1.0, 1.0, 1.0);
	else
		qglScalef(1.0, -1.0, 1.0);
	qglCullFace(0x405u);
LABEL_13:
	qglGetFloatv(0xBA7u,(GLfloat*) gProjectionMatrix);
	qglMatrixMode(0x1700u);
	qglLoadIdentity();
	qglRotatef(-90.0, 1.0, 0.0, 0.0);
	qglRotatef(90.0, 0.0, 0.0, 1.0);
	v39 = -r_refdef.viewangles[2];
	qglRotatef(v39, 1.0, 0.0, 0.0);
	v40 = -r_refdef.viewangles[0];
	qglRotatef(v40, 0.0, 1.0, 0.0);
	v41 = -r_refdef.viewangles[1];
	qglRotatef(v41, 0.0, 0.0, 1.0);
	v44 = -r_refdef.vieworg[2];
	v43 = -r_refdef.vieworg[1];
	v42 = -r_refdef.vieworg[0];
	qglTranslatef(v42, v43, v44);
	qglGetFloatv(0xBA6u,(GLfloat*) r_world_matrix);
}
*/
void R_SetupGL()
{
	//return Call_Function<void>(0x458D0);

	qglMatrixMode(GL_PROJECTION);
	qglLoadIdentity();

	int x = r_refdef.vrect.x;
	int v1 = r_refdef.vrect.width + r_refdef.vrect.x;
	int v2 = glheight - r_refdef.vrect.y;
	int y = glheight - r_refdef.vrect.height - r_refdef.vrect.y;
	if (r_refdef.vrect.x > 0)
		x = r_refdef.vrect.x - 1;
	if (v1 < glwidth)
		++v1;
	if (y < 0)
		--y;
	if (v2 < glheight)
		++v2;

	int vpWidth = v1 - x;
	int vpHeight = v2 - y;
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

	float fov = CalcFov(&scr_fov_value, width, height);

	auto v = atanf(1.0f / screenaspect) * 360.0 / PI;
	float viewfield = tan(v * PI / 360.0f) * 4;

	if (r_refdef.onlyClientDraws)
	{
		glFrustum(screenaspect * -viewfield, viewfield * screenaspect, -viewfield, viewfield, NEAR_PLANE, FAR_PLANE);
	}
	else  if (CL_IsDevOverviewMode())
	{
		NOT_TESTED;
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

	qglMatrixMode(0x1700u);
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
			vec4_t& projmat = gProjectionMatrix[0];
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					world2scren[j] = r_worldmat[k] * projmat[j];
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

/*
void R_RecursiveWorldNode(mnode_t* node)
{
	mnode_t* node_; // edi
	mplane_t* plane; // eax
	long double v3; // fst7
	__int16 v4; // fps
	bool v5; // c0
	char v6; // c2
	bool v7; // c3
	int v8; // ebx
	msurface_t* surf; // esi
	__int16 v10; // fps
	bool v11; // c0
	char v12; // c2
	bool v13; // c3
	unsigned int v14; // ecx
	texture_t* v15; // eax
	msurface_s* v16; // ecx
	int firstsurface_; // ecx
	mnode_s* v20; // eax
	int v21; // edx
	int* v22; // esi
	double v23; // [esp+Ch] [ebp-8h]
	mnode_t* nodea; // [esp+1Ch] [ebp+8h]

	node_ = node;

	if (node->contents == CONTENTS_SOLID)
		return; // hit a solid leaf

	while (1)
	{
		if (node->visframe != r_visframecount)
			return;

		if (R_CullBox(node_->minmaxs, &node_->minmaxs[3]))
			return;

		if (node_->contents < 0)
		{
			firstsurface_ = *(DWORD*)&node_->firstsurface;
			v20 = node_->children[1];
			if (firstsurface_)
			{
				v21 = r_framecount;
				do
				{
					v22 = (int*)v20->contents;
					v20 = (mnode_s*)((char*)v20 + 4);
					--firstsurface_;
					*v22 = v21;
				} while (firstsurface_);
			}
			if (node_->children[0])
				R_StoreEfrags((efrag_t**)node_->children);
			return;
		}

		plane = node_->plane;
		if (plane->type)
		{
			if (plane->type == 1)
			{
				v3 = modelorg[1];
			}
			else
			{
				v3 = modelorg[2];
				if (plane->type != 2)
					v3 = v3 * plane->normal[2] + modelorg[1] * plane->normal[1] + modelorg[0] * plane->normal[0];
			}
		}
		else
		{
			v3 = modelorg[0];
		}
		v23 = v3 - plane->dist;
		v5 = v23 < 0.0;
		v6 = 0;
		v7 = v23 == 0.0;
		v8 = v3 < 0.0;
		R_RecursiveWorldNode(node_->children[v8]);
		if (node_->numsurfaces)
		{
			surf = &g_pcl.worldmodel->surfaces[node_->firstsurface];
			if (v23 >= -0.01)
			{
				v11 = v23 < 0.01;
				v12 = 0;
				v13 = v23 == 0.01;
				if (v11 == 0)
					v8 = 0;
			}
			else
			{
				v8 = 2;
			}
			nodea = (mnode_t*)node_->numsurfaces;
			do
			{
				if (surf->visframe == r_framecount)
				{
					v14 = surf->flags;
					if ((v14 & 0x80u) != 0 || v23 < 0.0 == ((v14 >> 1) & 1))
					{
						if (gl_texsort)
						{
							if (mirror == false || surf->texinfo->texture != g_pcl.worldmodel->textures[mirrortexturenum])
							{
								v15 = surf->texinfo->texture;
								v16 = v15->texturechain;
								v15->texturechain = surf;
								surf->texturechain = v16;
							}
						}
						else if ((v14 & 4) != 0)
						{
							surf->texturechain = skychain;
							skychain = surf;
						}
						else if ((v14 & 0x10) != 0)
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
				++surf;
				nodea = (mnode_t*)((char*)nodea - 1);
			} while (nodea);
		}
		node_ = node_->children[v8 == 0];
		if (node_->contents == -2)
			return;
	}
}
*/

void __cdecl R_RecursiveWorldNode(mnode_t* node)
{	
	return Call_Function<void, mnode_t*>(0x49220, node);

	mnode_t* n; // ebx
	int side; // ebp
	qboolean v3; // eax
	mplane_t* v4; // edx
	byte v5; // al
	long double v6; // fst7
	int v7; // edi
	long double v8; // fst7
	msurface_t* surf; // esi
	int v10; // ecx
	texture_t* v11; // eax
	int v12; // esi
	mnode_s* v13; // edi
	DWORD* v14; // edx
	__int32 v15; // eax
	DWORD* v16; // edx
	BOOL v17; // [esp+28h] [ebp-24h]
	mtexinfo_t* v18; // [esp+2Ch] [ebp-20h]

	for (n = node; n->contents != -2 && n->visframe == r_visframecount; n = n->children[side])
	{
		v3 = R_CullBox(n->minmaxs, &n->minmaxs[3]);
		if (v3)
			break;
		if (n->contents < 0)
		{
			v12 = *(DWORD*)&n->firstsurface;
			v13 = n->children[1];
			if (v12)
			{
				do
				{
					v14 = (DWORD*)*(&v13->contents + v3);
					v15 = v3 + 1;
					*v14 = r_framecount;
					if (v15 == v12)
						break;
					v16 = (DWORD*)*(&v13->contents + v15);
					v3 = v15 + 1;
					*v16 = r_framecount;
				} while (v3 != v12);
			}
			if (n->children[0])
				R_StoreEfrags((efrag_t**)n->children);
			return;
		}
		v4 = n->plane;
		v5 = v4->type;
		if (v5 == 1)
		{
			v6 = modelorg[1] - v4->dist;
		}
		else if (v5)
		{
			if (v5 == 2)
				v6 = modelorg[2] - v4->dist;
			else
				v6 = modelorg[0] * v4->normal[0] + modelorg[1] * v4->normal[1] + modelorg[2] * v4->normal[2] - v4->dist;
		}
		else
		{
			v6 = modelorg[0] - v4->dist;
		}
		v17 = v6 < 0.0;
		R_RecursiveWorldNode(n->children[v17]);
		v7 = n->numsurfaces;
		v8 = (double)v6;
		if (n->numsurfaces)
		{
			side = 0;
			surf = &g_pcl.worldmodel->surfaces[n->firstsurface];
			if (v8 >= -0.01)
			{
				side = !v17;
				if (v8 > 0.01)
					side = 1;
			}
			do
			{
				if (surf->visframe == r_framecount)
				{
					v10 = surf->flags;
					if ((v10 & 0x80u) != 0 || ((v10 & 2) != 0) == v8 < 0.0)
					{
						if (gl_texsort)
						{
							v18 = surf->texinfo;
							v11 = v18->texture;
							if (mirror == false || v11 != g_pcl.worldmodel->textures[mirrortexturenum])
							{
								surf->texturechain = v11->texturechain;
								v18->texture->texturechain = surf;
							}
						}
						else if ((v10 & 4) != 0)
						{
							surf->texturechain = skychain;
							skychain = surf;
						}
						else if ((v10 & 0x10) != 0)
						{
							surf->texturechain = waterchain;
							waterchain = surf;
						}
						else
						{
							R_DrawSequentialPoly(surf, 0);
							v8 = (double)v8;
						}
					}
				}
				++surf;
				--v7;
			} while (v7);
		}
		else
		{
			side = !v17;
		}
	}
}

void ww(mnode_t* node)
{
	mnode_t* node_; // edi
	mplane_t* plane; // eax
	long double v3; // fst7
	__int16 v4; // fps
	bool v5; // c0
	char v6; // c2
	bool v7; // c3
	int v8; // ebx
	msurface_t* surf; // esi
	__int16 v10; // fps
	bool v11; // c0
	char v12; // c2
	bool v13; // c3
	unsigned int v14; // ecx
	texture_t* v15; // eax
	msurface_s* v16; // ecx
	int firstsurface_; // ecx
	mnode_s* v20; // eax
	int v21; // edx
	int* v22; // esi
	double v23; // [esp+Ch] [ebp-8h]
	mnode_t* nodea; // [esp+1Ch] [ebp+8h]

	node_ = node;

	if (node->contents == CONTENTS_SOLID)
		return; // hit a solid leaf

	int side;
	while (1)
	{
		if (node_->visframe != r_visframecount)
			return;

		if (R_CullBox(node_->minmaxs, &node_->minmaxs[3]))
			return;

		/*
		if (node_->contents < 0)
		{
			mleaf_t* pleaf = (mleaf_t*)node;

			msurface_t** mark = pleaf->firstmarksurface;

			for (int i = 0; i < pleaf->nummarksurfaces; i++)
			{
				mark[i]->visframe = r_framecount;
			}

			if (pleaf->efrags)
				R_StoreEfrags(&pleaf->efrags);
			break;
		}*/
		if (node_->contents < 0)
		{
			firstsurface_ = *(DWORD*)&node_->firstsurface;
			v20 = node_->children[1];
			if (firstsurface_)
			{
				v21 = r_framecount;
				do
				{
					v22 = (int*)v20->contents;
					v20 = (mnode_s*)((char*)v20 + 4);
					--firstsurface_;
					*v22 = v21;
				} while (firstsurface_);
			}
			if (node_->children[0])
				R_StoreEfrags((efrag_t**)node_->children);
			return;
		}

		plane = node_->plane;
		if (plane->type)
		{
			if (plane->type == 1)
			{
				v3 = modelorg[1];
			}
			else
			{
				v3 = modelorg[2];
				if (plane->type != 2)
					v3 = v3 * plane->normal[2] + modelorg[1] * plane->normal[1] + modelorg[0] * plane->normal[0];
			}
		}
		else
		{
			v3 = modelorg[0];
		}
		v23 = v3 - plane->dist;
		v5 = v23 < 0.0;
		v6 = 0;
		v7 = v23 == 0.0;
		v8 = v3 < 0.0;
		R_RecursiveWorldNode(node_->children[v8]);
		if (node_->numsurfaces)
		{
			surf = &g_pcl.worldmodel->surfaces[node_->firstsurface];
			if (v23 >= -0.01)
			{
				v11 = v23 < 0.01;
				v12 = 0;
				v13 = v23 == 0.01;
				if (v11 == 0)
					v8 = 0;
			}
			else
			{
				v8 = 2;
			}
			nodea = (mnode_t*)node_->numsurfaces;
			do
			{
				if (surf->visframe == r_framecount)
				{
					v14 = surf->flags;
					if ((v14 & 0x80u) != 0 || v23 < 0.0 == ((v14 >> 1) & 1))
					{
						if (gl_texsort)
						{
							if (mirror == false || surf->texinfo->texture != g_pcl.worldmodel->textures[mirrortexturenum])
							{
								v15 = surf->texinfo->texture;
								v16 = v15->texturechain;
								v15->texturechain = surf;
								surf->texturechain = v16;
							}
						}
						else if ((v14 & 4) != 0)
						{
							surf->texturechain = skychain;
							skychain = surf;
						}
						else if ((v14 & 0x10) != 0)
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
				++surf;
				nodea = (mnode_t*)((char*)nodea - 1);
			} while (nodea);
		}
		node_ = node_->children[v8 == 0];
		if (node_->contents == -2)
			return;
	}
}
/*
void R_RecursiveWorldNode(mnode_t* node)
{
	//return Call_Function<void, mnt*>(0x49220, node);

	mplane_t* plane; // eax
	long double v3; // fst7
	__int16 v4; // fps
	bool v5; // c0
	char v6; // c2
	bool v7; // c3
	int v8; // ebx
	msurface_t* surf; // esi
	__int16 v10; // fps
	bool v11; // c0
	char v12; // c2
	bool v13; // c3
	unsigned int flags; // ecx
	texture_t* v15; // eax
	msurface_s* v16; // ecx
	int firstsurface_; // ecx

	int v21; // edx
	int* v22; // esi
	double v23; // [esp+Ch] [ebp-8h]

	if (node->contents == CONTENTS_SOLID)
		return; // hit a solid leaf

	mnode_t* n = node;
	int side;
	while (1)
	{
		if (n->visframe != r_visframecount)
			return;

		if (R_CullBox(n->minmaxs, &n->minmaxs[3]))
			return;

		if (n->contents < 0)
		{
			mleaf_t* pleaf = (mleaf_t*)node;

			msurface_t** mark = pleaf->firstmarksurface;

			for (int i = 0; i < pleaf->nummarksurfaces; i++)
			{
				mark[i]->visframe = r_framecount;
			}

			if (pleaf->efrags)
				R_StoreEfrags(&pleaf->efrags);
			break;
		}

		plane = n->plane;
		if (plane->type)
		{
			if (plane->type == 1)
			{
				v3 = modelorg[1];
			}
			else
			{
				v3 = modelorg[2];
				if (plane->type != 2)
					v3 = v3 * plane->normal[2] + modelorg[1] * plane->normal[1] + modelorg[0] * plane->normal[0];
			}
		}
		else
		{
			v3 = modelorg[0];
		}

		float z = v3;
		float dot = PlaneDiff(modelorg, node->plane);
		dot = floor(dot * 10) / 10;
		z = floor(z * 10) / 10;
		if (z != dot)
			printf(")");
		side = (dot >= 0.0f) ? 0 : 1;

		// recurse down the children, front side first
		R_RecursiveWorldNode(n->children[side]);

		if (n->numsurfaces)
		{
			surf = &g_pcl.worldmodel->surfaces[n->firstsurface];
			if (dot >= -0.01)
			{
				v11 = dot < 0.01;
				v12 = 0;
				v13 = dot == 0.01;
				if (v11 == 0)
					v8 = 0;
			}
			else
			{
				v8 = 2;
			}
			
			for (int i = 0; i < n->numsurfaces; i++)
			{
				if (surf[i].visframe != r_framecount)
					continue;
				flags = surf[i].flags;
				if (flags & 0x80 || dot < 0.0 == ((flags >> 1) & 1))
				{
					if (gl_texsort)
					{
						if (mirror == false || surf[i].texinfo->texture != g_pcl.worldmodel->textures[mirrortexturenum])
						{
							v15 = surf[i].texinfo->texture;
							v16 = v15->texturechain;
							v15->texturechain = &surf[i];
							surf[i].texturechain = v16;
						}
					}
					else if (flags & SURF_DRAWSKY)
					{
						surf->texturechain = skychain;
						skychain = surf;
					}
					else if (flags & SURF_DRAWTURB)
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


		n = n->children[v8]; side;
	}
}
*/
qboolean R_CullBox(vec_t* mins, vec_t* maxs)
{
	for (int i = 0; i < 4; i++)
	{
		if (BoxOnPlaneSide(mins, maxs,&frustum[i]) == 2)
		{
			return true;
		}
	}
	return false;
}

void R_StoreEfrags(efrag_t** ppefrag)
{
	efrag_t* efrag;
	cl_entity_s* ent;

	for (; *ppefrag; ppefrag = &efrag->leafnext)
	{
		efrag = *ppefrag;

		ent = efrag->entity;
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
	TO_IMPLEMENT;
	return Call_Function<void>(0x48500);
}

void R_DrawSequentialPoly(msurface_t* s, int face)
{
	//return Call_Function<void, msurface_t*, int>(0x47900, s, face);

	int rendermode;
	texture_t* animationtex;
	int renderstate;
	auto polygs = s->polys;
	auto vert = polygs->verts;

	if (currententity->curstate.rendermode == 1)
	{
		GL_DisableMultitexture();
		qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		int texnum = R_TextureAnimation(s)->gl_texturenum;
		GL_Bind(texnum);
		qglDisable(GL_TEXTURE_2D);
		DrawGLSolidPoly(s->polys);
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

		if (gl_mtexable && ((rendermode = currententity->curstate.rendermode) == 0 || rendermode == 4))
		{
			animationtex = R_TextureAnimation(s);
			GL_SelectTexture(TEXTURE0_SGIS);
			GL_Bind(animationtex->gl_texturenum);
			qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448);

			if (currententity->curstate.rendermode == 1)
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
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0,
					lightmap_rectchange[lmtexnum].t, 128,
					lightmap_rectchange[lmtexnum].h, gl_lightmap_format,
					GL_UNSIGNED_BYTE, pixels);

				lmrect->l = lmrect->t = 128;
				lmrect->h = lmrect->w = 0;
			}

			qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448);

			renderstate = DT_SetRenderState(animationtex->gl_texturenum);

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
			auto texanimation = R_TextureAnimation(s);
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
	TO_IMPLEMENT;
	return Call_Function<void>(0x48AB0);
}

void DrawGLSolidPoly(glpoly_t* p)
{
	TO_IMPLEMENT;
	return Call_Function<void, glpoly_t*>(0x483C0, p);
}

texture_t* R_TextureAnimation(msurface_t* s)
{
	TO_IMPLEMENT;
	return Call_Function<texture_t*, msurface_t*>(0x47750, s);
}

void EmitWaterPolys(msurface_t* fa, int direction)
{
	return Call_Function<void, msurface_t*, int>(0x4F192, fa, direction);
}

void R_RenderDynamicLightmaps(msurface_t* fa)
{
	//return Call_Function<void, msurface_t*>(0x47F00, fa);
	++c_brush_polys;
	if ((fa->flags & 0x14) != 0)
		return;

	int lmtexnum = fa->lightmaptexturenum;
	auto polys = fa->polys;
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
	TO_IMPLEMENT;
	return Call_Function<void, qboolean>(0x4B890, bMultitexture);
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
		Sys_Error(
			"Error: lightmap for texture% s too large(% d x % d = % d luxels); cannot exceed% d\n",
			psurf->texinfo->texture->name,
			smax,
			tmax,
			size,
			MAX_LIGHTMAP);

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
			blocklights[i].r = blocklights[i].g = blocklights[i].b = 65280;
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
					int index = (lightscale * *(&blocklights[(i * smax) + j].r + k)) / 16384;
					if (index > 1023)
						index = 1023;
					lightmap[k] = lightgammatable[index] / 4;
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
	
	mtexinfo_t* texinfo;
	long double v10;
	vec3_t v18;
	double radius;
	int tmax;
	int smax; 
	float minlight;

	texinfo = surf->texinfo;
	smax = (surf->extents[0] / 16) + 1;

	tmax = (surf->extents[1] / 16) + 1;
	for (int lnum = 0; lnum < 32; ++lnum)
	{
		if (((1 << lnum) & surf->dlightbits) != 0)
		{
			VectorSubtract(cl_dlights[lnum].origin, currententity->origin, v18);

			radius = cl_dlights[lnum].radius;
			float v5 = abs(_DotProduct(v18, surf->plane->normal) - surf->plane->dist);
			minlight = cl_dlights[lnum].minlight;
			float v36 = radius - v5;
			if (v36 >= minlight)
			{
				float v32 = v36 - minlight;

				float v15 = _DotProduct(v18, texinfo->vecs[0]) + texinfo->vecs[0][3] - surf->texturemins[0];
				float v17 = _DotProduct(v18, texinfo->vecs[1]) + texinfo->vecs[1][3] - surf->texturemins[1];

				for (int i = 0; i < tmax; i++)
				{
					int v7 = abs(v17 - (i * 16));
			
					for (int j = 0; j < smax; j++)
					{
						int v9 = abs(v15 - (j * 16));
						if (v9 <= v7)
							v10 = v7 + (v9 / 2);
						else
							v10 = v9 + (v7 / 2);

						float v35 = v10;
						if (v35 < v32)
						{
							int v11 = (v36 - v35) * 256;
							int blindex = (i * smax) + j;
							blocklights[blindex].r += (v11 * cl_dlights[lnum].color.r) / 256;
							blocklights[blindex].g += (v11 * cl_dlights[lnum].color.g) / 256;
							blocklights[blindex].b += (v11 * cl_dlights[lnum].color.b) / 256;
						}

					}
				}
			}
		}
	}
}

void R_RenderDlight(dlight_t* light)
{
	// Never executed code since gl_flashblend never changed
	return Call_Function<void, dlight_t*>(0x42F90, light);
}

void R_DrawWorld()
{
	// Call_Function<void>(0x49440);

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
	R_RecursiveWorldNode(g_pcl.worldmodel->nodes);	// Under implement
	gDecalSurfCount = 0;
	DrawTextureChains(); // Implement
	S_ExtraUpdate();
	if (!CL_IsDevOverviewMode())
	{
		R_DrawDecals(false);	// Implement
		R_BlendLightmaps();	// Implement
	}
}

void DrawGLPolyScroll(msurface_t* psurface, cl_entity_t* pEntity)
{
	TO_IMPLEMENT;
	return Call_Function<void, msurface_t*, cl_entity_t*>(0x48350, psurface,pEntity);
}

void R_DrawEntitiesOnList()
{
	return Call_Function<void>(0x44860);
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

float CalcFov(float* fov_x, float width, float height)
{
	float x, half_fov_y;

	if (*fov_x < 1.0f || *fov_x > 179.0f)
		*fov_x = 90.0f; // default value

	x = width / tan(DEG2RAD(*fov_x) * 0.5f);
	half_fov_y = atan(height / x);

	return RAD2DEG(half_fov_y) * 2;
}

float ScrollOffset(msurface_t* psurface, cl_entity_t* pEntity)
{
	return Call_Function<float, msurface_t*, cl_entity_t*>(0x482A0, psurface, pEntity);
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
	float v7 = _DotProduct(normal, p) * v5;
	for (int i = 0; i < 3; i++)
		dst[i] = p[i] - (normal[i] * v5 * v7);
}

void PerpendicularVector(vec_t* dst, const vec_t* src)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		if (src[i] <= src[0] && src[i] <= src[1] && src[i] <= src[2])
			break;
	}
	vec3_t tempvec = { 0 };
	tempvec[i] = 1;
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
	(*dst)[0] = rot[0][0] * *point + rot[0][1] * point[1] + rot[0][2] * point[2];
	(*dst)[1] = rot[1][0] * *point + rot[1][1] * point[1] + rot[1][2] * point[2];
	(*dst)[2] = rot[2][0] * *point + rot[2][1] * point[1] + rot[2][2] * point[2];
}

int SignbitsForPlane(mplane_t* out)
{
	int sign = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (out->normal[i] < 0.0)
			sign|= 1 << i;
	}
	return sign;
}