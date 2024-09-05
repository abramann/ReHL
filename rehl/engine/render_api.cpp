#include "precompiled.h"


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
	return Call_Function<void>(0x46350);
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
	
		R_Clear();	// Implement

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
	NOT_IMPLEMENTED;
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
