#include "precompiled.h"

cvar_t ati_npatch = { "ati_npatch", "1.0" , FCVAR_ARCHIVE };
cvar_t gl_wireframe = { "gl_wireframe", "0", 40 };
cvar_t r_cullsequencebox = { "r_cullsequencebox", "1" };
cvar_t r_bmodelinterp = { "r_bmodelinterp", "1" };
cvar_t r_norefresh = { "r_norefresh", "0" };
cvar_t r_lightmap = { "r_lightmap", "0" };
cvar_t r_fullbright = { "r_fullbright", "0" };
cvar_t r_decals = { "r_decals", "4096" };
cvar_t sp_decals = { "sp_decals", "4096", FCVAR_ARCHIVE };
cvar_t mp_decals = { "mp_decals", "300", FCVAR_ARCHIVE };
cvar_t r_drawentities = { "r_drawentities", "1" };
cvar_t r_drawviewmodel = { "r_drawviewmodel", "1" };
cvar_t r_mirroralpha = { "r_mirroralpha", "1" };
cvar_t r_wateralpha = { "r_wateralpha", "1" };
cvar_t r_dynamic = { "r_dynamic", "1" };
cvar_t r_novis = { "r_novis", "0" };
cvar_t r_speeds = { "r_speeds", "0" };
cvar_t d_spriteskip = { "d_spriteskip", "0" };
cvar_t r_mmx = { "r_mmx", "0" };
cvar_t r_traceglow = { "r_traceglow", "0" };
cvar_t r_glowshellfreq = { "r_glowshellfreq", "2.2" };
cvar_t gl_clear = { "gl_clear" , "0" };
cvar_t gl_cull = { "gl_cull" , "1" };
cvar_t gl_affinemodels = { "gl_affinemodels" , "0" };

extern cvar_t gl_dither;// = { "gl_dither" , "1", FCVAR_ARCHIVE };
extern cvar_t gl_spriteblend;// = { "gl_spriteblend" , "1", FCVAR_ARCHIVE };

cvar_t gl_polyoffset = { "gl_polyoffset" , "4", FCVAR_ARCHIVE };
cvar_t gl_lightholes = { "gl_lightholes" , "1", FCVAR_ARCHIVE };
cvar_t gl_keeptjunctions = { "gl_keeptjunctions" , "1", FCVAR_ARCHIVE };
cvar_t gl_wateramp = { "gl_wateramp", "0.3", FCVAR_ARCHIVE };
cvar_t gl_overbright = { "gl_overbright", "1" , FCVAR_ARCHIVE };
cvar_t gl_zmax = { "gl_zmax", "4096" };
cvar_t gl_alphamin = { "gl_alphamin", "0.25" };
cvar_t gl_flipmatrix = { "gl_flipmatrix", "0", FCVAR_ARCHIVE };
cvar_t gl_monolights = { "gl_monolights", "0", FCVAR_ARCHIVE };
cvar_t gl_fog = { "gl_fog", "0", FCVAR_ARCHIVE };


int dottexture[8][8] =
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

bool gl_texsort = true;

int r_numparticles;

BEAM* gBeams = nullptr;

particle_t* particles = nullptr;

int playertextures[16];

int particletexture;

void R_TimeRefresh_f(void);

void R_Envmap_f(void);

void R_ReadPointFile_f(void);

void GL_Dump_f(void);

void R_InitParticles();

void R_InitParticleTexture();

void R_UploadEmptyTex();



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
	R_UploadEmptyTex();
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
		qglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA2, 8, 8, 0, GL_RGBA,GL_UNSIGNED_BYTE, data);

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
	NOT_IMPLEMENTED;
}

void R_Envmap_f(void)
{
	NOT_IMPLEMENTED;
}

void R_ReadPointFile_f(void)
{
	NOT_IMPLEMENTED;
}

void GL_Dump_f(void)
{
	NOT_IMPLEMENTED;
}
