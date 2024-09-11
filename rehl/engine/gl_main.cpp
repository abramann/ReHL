#include "precompiled.h"

void GL_CreateSurfaceLightmap(msurface_t *surf);

void BuildSurfaceDisplayList(msurface_t *fa);

const int BLOCK_SIZE = 128;

ARRAY(int, allocated, [64][128], 0x49BCF);

ARRAY(int, lightmap_textures, [64], 0x49BD9);

VAR(GLenum, gl_lightmap_format, 0x49C17);

VAR(GLint, lightmap_used, 0x49C1C);

VAR(int, lightmap_bytes, 0x49C0E);

VAR(model_t*, currentmodel, 0x49C50);

ARRAY(qboolean, lightmap_modified, [64], 0x48677);

ARRAY(uchar, lightmaps, [4194304], 0x49D47);

VVAR(mvertex_t*, r_pcurrentvertbase, 0x49C5B, nullptr);

void GL_BuildLightmaps()
{
	//return Call_Function<void>(0x49BC0);

	Q_memset(allocated, 0, sizeof(allocated));
	r_framecount = 1;
	if (*lightmap_textures == 0)
	{
		for (int i = 0; i < 64; i++)
			lightmap_textures[i] = GL_GenTexture();
	}

	gl_lightmap_format = lightmap_used = GL_RGBA;
	lightmap_bytes = 4;

	for (int i = 1; i < MAX_MODELS; i++)
	{
		model_t* model = g_pcl.model_precache[i];
		if (model == nullptr)
			break;

		if (*model->name == '*' || model->numsurfaces <= 0)
			continue;

		currentmodel = model = CL_GetModelByIndex(i);
		r_pcurrentvertbase = model->vertexes;
		int numsurfaces = model->numsurfaces;
		if (numsurfaces <= 0)
			continue;

		for (int j = 0; j < numsurfaces; j++)
		{
			msurface_t* surface = &model->surfaces[j];
			GL_CreateSurfaceLightmap(surface);	// Implement
			if ((surface->flags & 0x10) == 0)
				BuildSurfaceDisplayList(surface);	// Implement
		}
	}
	if (!gl_texsort)
		GL_SelectTexture(TEXTURE1_SGIS);
	for (int i = 0; i < 64; i++)
	{
		if (!allocated[i][0])
			break;

		lightmap_modified[i] = false;
		GL_Bind(lightmap_textures[i]);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		qglPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		qglTexImage2D(GL_TEXTURE_2D, 0, lightmap_used, 128, 128, 0, gl_lightmap_format, GL_UNSIGNED_BYTE, &lightmaps[lightmap_bytes * (16384 * i)]);
	}
}

void GL_CreateSurfaceLightmap(msurface_t *surf)
{
	//return Call_Function<void, msurface_t*>(0x49B50, surf);
	int flags = surf->flags;
	if ((flags & 0x14) == 0 && ((flags & 0x20) == 0 || (surf->texinfo->flags & 1) == 0))
	{
		int texnum = surf->lightmaptexturenum = AllocBlock((surf->extents[0] / 16) + 1, (surf->extents[1] / 16) + 1, &surf->light_s, &surf->light_t);
		int bytes = lightmap_bytes;
		R_BuildLightMap(surf, &lightmaps[bytes * (surf->light_s + ((surf->light_t + (texnum *128)) * 128))], bytes * 128);

		//R_BuildLightMap(surf, &lightmaps[bytes * (surf->light_s + ((surf->light_t + (texnum << 7)) << 7))], bytes << 7);
	}
}

void BuildSurfaceDisplayList(msurface_t *fa)
{
	return Call_Function<void, msurface_t*>(0x497B0, fa);
	
	/*
	int j; // [esp+3Ch] [ebp-60h]
	int ja; // [esp+3Ch] [ebp-60h]
	medge_t* pedges;
	glpoly_t* poly;

	int numedges = fa->numedges;

	pedges = currentmodel->edges;

	poly = (glpoly_t*)Hunk_Alloc(28 * numedges + 16);

	poly->next = fa->polys;

	poly->flags = fa->flags;

	fa->polys = poly;

	poly->numverts = numedges;

	if (numedges <= 0)
	{
		for (int i = 0; i < numedges; i++)
		{
			
			int v;
			int surfedge = currentmodel->surfedges[i + fa->firstedge];
			if (surfedge > 0)
				v = pedges[-surfedge].v[1];
			else
				v = pedges[-surfedge].v[0];

			mtexinfo_t* texinfo = fa->texinfo;
			texture_t* tex = texinfo->texture;
			mvertex_t* vertex = &r_pcurrentvertbase[v];
			vec3_t v2;
			VectorCopy(vertex->position, v2);
			float v3 = _DotProduct(texinfo->vecs[0], v2) + texinfo->vecs[0][3];
			float v4 = _DotProduct(texinfo->vecs[1], v2);
			float v5 = v3 / tex->width;
			
		}
	}
	if (gl_keeptjunctions.value == 0.0 && fa->flags >= 0 && numedges > 0)
	{
		
	}
	*/
}

int AllocBlock(int w, int h, int* x, int* y)
{	
	int best;
	int texnum;
	int best2 = 0;
	texnum = 0;
	for (int k = 0; ; k += 128)
	{
		best2 = BLOCK_SIZE;
		for (int i = 0; i < BLOCK_SIZE - w; i++)
		{
			best = 0;
			int j;
			for (j = 0; j < w; j++)
			{
				int* v = (int*)allocated + i + k + j;
				if (*v >= best2)
					break;
				if (*v > best)
					best = *v;
			}
			if (j == w)
			{
				best2 = best;
				*x = i;
				*y = best;
			}
		}
		if (best2 + h <= BLOCK_SIZE)
			break;
		++texnum;
		if (k + BLOCK_SIZE >= 0x2000)
			Sys_Error("%s: Full", __func__);
	}

	for (int i = 0; i < w; i++)
		allocated[texnum][i + *x] = best2 + h;

	return texnum;
}

void GL_Dump_f()
{
	NOT_IMPLEMENTED;
}