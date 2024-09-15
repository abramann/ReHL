#include "precompiled.h"

void GL_CreateSurfaceLightmap(msurface_t *surf);

void BuildSurfaceDisplayList(msurface_t *fa);

const int BLOCK_SIZE = 128;

ARRAY(int, allocated, [64][128], 0x49BCF);

ARRAY(int, lightmap_textures, [64], 0x49BD9);

VAR(GLenum, gl_lightmap_format, 0x494FB);

VAR(GLint, lightmap_used, 0x49C1C);

VAR(int, lightmap_bytes, 0x49C0E);

VAR(model_t*, currentmodel, 0x49C50);

ARRAY(qboolean, lightmap_modified, [64], 0x48677);

ARRAY(uchar, lightmaps, [4194304], 0x49D47);

VVAR(mvertex_t*, r_pcurrentvertbase, 0x49C5B, nullptr);

VVAR(qboolean, mtexenabled, 0x478A1, false);

ARRAY(glRect_t, lightmap_rectchange, [64], 0x47A5A);

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
			GL_CreateSurfaceLightmap(surface);
			if ((surface->flags & 0x10) == 0)
				BuildSurfaceDisplayList(surface);
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
	}
}

void BuildSurfaceDisplayList(msurface_t* fa)
{
	int numedges = fa->numedges;
	glpoly_t* poly;
	model_t* currmodel;
	mvertex_t* vertexbase;
	mtexinfo_t* texinfo = fa->texinfo;
	texture_t* texture = texinfo->texture;
	medge_t* pedges = currentmodel->edges;

	poly = (glpoly_t*)Hunk_Alloc(28 * numedges + 16);
	poly->next = fa->polys;
	poly->flags = fa->flags;
	fa->polys = poly;
	poly->numverts = numedges;

	currmodel = currentmodel;

	for (int i = 0; i < numedges; i++)
	{
		int surfedge = currentmodel->surfedges[i + fa->firstedge];
		int v;
		if (surfedge <= 0)
			v = pedges[-surfedge].v[1];
		else
			v = pedges[surfedge].v[0];

		vertexbase = &r_pcurrentvertbase[v];
		texinfo = fa->texinfo;

		float v13 = _DotProduct(texinfo->vecs[0], vertexbase->position) + texinfo->vecs[0][3];// texinfo->vecs[0][0] * v9 + texinfo->vecs[0][1] * v10 + texinfo->vecs[0][2] * v11 + texinfo->vecs[0][3];
		float v4 = _DotProduct(texinfo->vecs[1], vertexbase->position) + texinfo->vecs[1][3];

		poly->verts[0][3] = v13 / texture->width;
		poly->verts[0][4] = v4 / texture->height;

		VectorCopy(vertexbase->position, poly->verts[0]);

		float v22 = _DotProduct(vertexbase->position, fa->texinfo->vecs[0]) + fa->texinfo->vecs[0][3]
			- fa->texturemins[0]
			+(16 * fa->light_s)
			+ 8.0;

			float v23 = _DotProduct(vertexbase->position, fa->texinfo->vecs[1]) + fa->texinfo->vecs[1][3]
			- fa->texturemins[1]
			+ 16 * fa->light_t
			+ 8.0;

		poly->verts[0][5] = v22 * 0.00048828125;
		poly->verts[0][6] = 0.00048828125 * v23;
		poly = (glpoly_t*)((char*)poly + 28);
	}
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

void GL_DisableMultitexture()
{
	if (mtexenabled)
	{
		qglDisable(GL_TEXTURE_2D);
		GL_SelectTexture(TEXTURE0_SGIS);
		mtexenabled = false;
	}
}

void GL_EnableMultitexture()
{
	if (gl_mtexable)
	{
		GL_SelectTexture(TEXTURE1_SGIS);
		qglEnable(GL_TEXTURE_2D);
		mtexenabled = true;
	}
}

void GL_Dump_f()
{
	NOT_IMPLEMENTED;
}