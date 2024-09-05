#include "precompiled.h"

void GL_CreateSurfaceLightmap(msurface_t *surf);

void BuildSurfaceDisplayList(msurface_t *fa);

// TODO: move 'allocated', 
ARRAY(int, allocated, [64][128], 0x49BCF);

ARRAY(int, lightmap_textures, [64], 0x49BD9);

VAR(GLenum, gl_lightmap_format, 0x49C17);
VAR(GLint, lightmap_used, 0x49C1C);

VAR(int, lightmap_bytes, 0x49C0E);

VAR(model_t*, currentmodel, 0x49C50);

ARRAY(int, lightmap_modified, [64], 0x49CE9);

ARRAY(uchar, lightmaps, [4194304], 0x49D47);

mvertex_t *r_pcurrentvertbase;

void GL_BuildLightmaps()
{
	Q_memset(allocated, 0, sizeof(allocated));
	r_framecount = 1;
	if (*lightmap_textures == 0)
	{
		for (int i = 1; i < 64; i++)
			lightmap_textures[i] = GL_GenTexture();
	}
	gl_lightmap_format = GL_RGBA;
	lightmap_used = GL_RGBA;
	lightmap_bytes = 4;

	for (int i = 0; i < MAX_MODELS; i++)
	{
		model_t* model = g_pcl.model_precache[i];
		if (model == nullptr)
			break;

		if (*model->name == '*' || model->numsurfaces <= 0)
			continue;

		model = CL_GetModelByIndex(i);	// Implement
		currentmodel = model;
		mvertex_t* vertexes = model->vertexes;

		int numsurfaces = model->numsurfaces;
		if (numsurfaces <= 0)
			continue;

		for (int i = 0; i < numsurfaces; i++)
		{
			GL_CreateSurfaceLightmap(&model->surfaces[i]);
			if ((model->surfaces[i].flags & 0x10) == 0)
				BuildSurfaceDisplayList(&model->surfaces[i]);
		}
		if (!gl_texsort)
			GL_SelectTexture(TEXTURE1_SGIS);
		for (int i = 0; i < 64; i++)
		{
			if (!allocated[i][0])
				break;

			lightmap_modified[i] = false;
			GL_Bind(lightmap_textures[i]);
			qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 9729.0);
			qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 9729.0);
			qglPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			qglTexImage2D(GL_TEXTURE_2D, 0, lightmap_used, 128, 128, 0, gl_lightmap_format, GL_UNSIGNED_BYTE, &lightmaps[lightmap_bytes * (0x4000*i)]);
		}
	}
}

void GL_CreateSurfaceLightmap(msurface_t *surf)
{
	return Call_Function<void, msurface_t*>(0x49B50, surf);
}

void BuildSurfaceDisplayList(msurface_t *fa)
{
	return Call_Function<void, msurface_t*>(0x497B0, fa);
}

void GL_Dump_f()
{
	NOT_IMPLEMENTED;
}