#include "precompiled.h"

// TODO: Find good name for variables in R_AnimateLight, R_SetupGL
// TODO: Correct implement of R_SetupGL, R_AnimateLight, GL_BuildLightmaps(gl_main.cpp)

struct cshift_t
{
	int destcolor[3];
	int percent;
};

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
VVAR(qboolean, gl_texsort, 0x46D69, true);
VAR(int, r_numparticles, 0x7ABDA);
VVAR(BEAM*, gBeams, 0x7AB53, nullptr);
VVAR(particle_t*, particles, 0x7AC0C, nullptr);
VAR(int, particletexture, 0x46713);
VAR(cl_entity_t, r_worldentity, 0x46DE9);
VAR(int, c_brush_polys, 0x4649F);
VAR(int, c_alias_polys, 0x464A9);
VAR(qboolean, mirror, 0x464B3);
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
VVAR(cl_entity_t*, currententity, 0x4949C, nullptr;);
VAR(vec3_t, modelorg, 0x4947A);
VAR(colorVec, gWaterColor, 0x4948B);
ARRAY(glpoly_t*, lightmap_polys, [64], 0x494E0);
ARRAY(float, skymins, [2][6], 0x505FC);
ARRAY(float, skymaxs, [2][6], 0x5060E);
VAR(int, gDecalSurfCount, 0x49577);

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

const cvar_t gl_envmapsize = { "gl_envmapsize", "256" };
const cvar_t gl_flipmtrix = { "gl_flipmatrix", "0" };
const double FAR_PLANE = 16000.0;

void R_SetupFrame();
void R_SetFrustum();
void R_SetupGL();
void R_DrawWorld();
void R_DrawEntitiesOnList();
void R_Clear();
void R_CheckVariables();
void R_AnimateLight();
void AllowFog(int allowed);
void GL_LoadFilterTexture(float r, float g, float b, float brightness);
void RotatePointAroundVector(vec_t* dst, const vec_t* dir, const vec_t* point, float degrees);
int SignbitsForPlane(mplane_t* out);

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
	//return Call_Function<void>(0x46350);

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
			qglClearColor(0.0, 1.0, 0.0, 0.0);
			qglClear(0x4000u);
		}
		R_DrawWorld();
		S_ExtraUpdate();
		R_DrawEntitiesOnList();
	}
	if (g_bUserFogOn)
		R_RenderFinalFog();

	isFogEnabled = qglIsEnabled(GL_FOG);

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
	//R_UploadEmptyTex();
	for (int i = 0; i < 16; i++)
		playertextures[i] = GL_GenTexture();
}

void R_PushDlights()
{
	NOT_IMPLEMENTED;
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
	double time1a;
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

		R_RenderScene(); // Implement
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
	return Call_Function<void>(0x495C0);
	if (mirror)
		return;
	if (r_oldviewleaf == r_viewleaf && r_novis.value == 0) 
		return;

	uchar solid[4096];
	uchar* vis = nullptr;

	++r_visframecount;
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
		if ((((int)vis[i >> 3] >> (i & 7)) & 1) != 0)
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
				qglClear(GL_DEPTH_BUFFER_BIT0_QCOM);
			else
				qglClear(GL_DEPTH_BUFFER_BIT6_QCOM | GL_DEPTH_BUFFER_BIT0_QCOM);
			gldepthmin = 0.0;
			gldepthmax = 1.0;
			glDepthFunc(GL_LEQUAL);
		}
		else
		{
			if (gl_clear.value != 0.0)
				glClear(GL_DEPTH_BUFFER_BIT6_QCOM);
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
			qglClear(GL_DEPTH_BUFFER_BIT0_QCOM);
		else
			qglClear(GL_DEPTH_BUFFER_BIT6_QCOM | GL_DEPTH_BUFFER_BIT0_QCOM);
		gldepthmin = 0.0f;
		gldepthmax = 0.5f;
		qglDepthFunc(GL_LEQUAL);
	}
	qglDepthRange(gldepthmin, gldepthmax);
}

void R_SetupFrame()
{
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
	mplane_t frustum[4];
	float fov = scr_fov_value;
	CalcFov(&scr_fov_value, glwidth, glheight);
	float xa = -(90 - fov * 0.5f);
	float xb = 90 - (0.5 * glwidth);

	RotatePointAroundVector(frustum[0].normal, vup, vpn, xa);
	RotatePointAroundVector(frustum[1].normal, vup, vpn, -xa);
	RotatePointAroundVector(frustum[2].normal, vright, vpn, xb);
	RotatePointAroundVector(frustum[3].normal, vright, vpn, -xb);

	for (int i = 0; i < ARRAYSIZE(frustum); i++)
	{
		frustum[i].normal[3] = _DotProduct(frustum[i].normal, r_origin);
		frustum->normal[3] = SignbitsForPlane(&frustum[i]);
	}
}

void R_SetupGL()
{
	return Call_Function<void>(0x458D0);
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
		vpWidth = vpHeight = gl_envmapsize.value;
		glwidth = glheight = gl_envmapsize.value;
	}
	qglViewport(x + glx, y + gly, vpWidth, vpHeight);
	float width = r_refdef.vrect.width;
	float height = r_refdef.vrect.height;
	float screenaspect = width / height;

	float fov = CalcFov(&scr_fov_value, width, height);
	if (r_refdef.onlyClientDraws)
	{
		qglFrustum(screenaspect * -fov, screenaspect * fov, -fov, fov, 4.0, FAR_PLANE);
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
		qglFrustum(screenaspect * -fov, fov * screenaspect, -fov, fov, 4.0, movevars.zmax);
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

	float z = -r_refdef.vieworg[2];
	y = -r_refdef.vieworg[1];
	x = -r_refdef.vieworg[0];
	qglTranslatef(x, y, z);

	qglGetFloatv(GL_MODELVIEW_MATRIX, (float*)r_world_matrix);
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
				world2scren[j] = 0;
				for (int k = 0; k < 4; k++)
				{
					world2scren[j] = r_worldmat[k] * projmat[j];
				}
			}
		}
	}
	else
	{
		// 
	}
	InvertMatrix((float*)&gWorldToScreen, (float*)&gScreenToWorld);
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
}

void R_BlendLightmaps()
{
}

void DrawTextureChains()
{
}

void R_DrawDecals(qboolean bMultitexture)
{
}

void R_DrawWorld()
{
	return Call_Function<void>(0x49440);

	cl_entity_t ent;
	Q_memset(&ent, 0, sizeof(cl_entity_t));
	currententity = &ent;

	ent.model = g_pcl.worldmodel;

	VectorCopy(r_refdef.vieworg, modelorg);
	
	currenttexture = -1;
	memcpy(&ent.curstate.rendercolor, &gWaterColor, sizeof(colorVec));

	qglColor3f(1.0, 1.0, 1.0);
	Q_memset(lightmap_polys, 0, 256);
	R_ClearSkyBox();
	if (gl_texsort == false)
	{
		switch (gl_lightmap_format)
		{
		case GL_LUMINANCE:
			qglBlendFunc(0, GL_ONE_MINUS_SRC_COLOR);
			break;
		case GL_INTENSITY:
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448.0);
			qglColor4f(0.0, 0.0, 0.0, 1.0);
			qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		R_DrawDecals(0);
		R_BlendLightmaps();
	}
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
	const int v1 = 10 * g_pcl.time;
	for (int i = 0; i < ARRAYSIZE(d_lightstylevalue); i++)
	{
		int len = cl_lightstyle[i].length;
		if (len > 0)
		{
			d_lightstylevalue[i] = 22 * (cl_lightstyle[i].map[v1 % len]) - 97;
			d_lightstylevalue[i] = 22 * (cl_lightstyle[0].map[v1 % len] + i * 68) - 97;
		}
		else
		{
			d_lightstylevalue[i] = 256;
		}
	}
	Call_Function<void>(0x42eb0);
}

void GL_LoadFilterTexture(float r, float g, float b, float brightness)
{
	uchar* texbuf = (uchar*)Mem_Malloc(192);
	uchar rfiltered = r * brightness * 255;
	uchar gfiltered = g * brightness * 255;
	uchar bfiltered = b * brightness * 255;
	for (int i = 0; i < 192;)
	{
		texbuf[i++] = rfiltered;
		texbuf[i++] = gfiltered;
		texbuf[i++] = bfiltered;
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
	free(texbuf);
}

void AllowFog(int allowed)
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

void RotatePointAroundVector(vec_t* dst, const vec_t* dir, const vec_t* point, float degrees)
{
	return Call_Function<void, const vec_t*, const vec_t*, const vec_t*, float>(0x43A10, dst, dir, point, degrees);
	NOT_IMPLEMENTED;
}

int SignbitsForPlane(mplane_t* out)
{
	for (int i = 0; i < 3; ++i)
	{
		if (out->normal[i] < 0.0)
			return 1 << i;
	}
	return 0;
}