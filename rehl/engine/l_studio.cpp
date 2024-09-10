/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    In addition, as a special exception, the author gives permission to
*    link the code of this program with the Half-Life Game Engine ("HL
*    Engine") and Modified Game Libraries ("MODs") developed by Valve,
*    L.L.C ("Valve").  You must obey the GNU General Public License in all
*    respects for all of the code used other than the HL Engine and MODs
*    from Valve.  If you modify this file, you may extend this exception
*    to your version of the file, but you are not obligated to do so.  If
*    you do not wish to do so, delete this exception statement from your
*    version.
*
*/

#include "precompiled.h"

//int giTextureSize;

void EXT_FUNC Mod_LoadStudioModel_internal(model_t * mod, void * buffer)
{
	uint8 *poutdata;
	uint8 *pindata;
	mstudiotexture_t *ptexture;
	int size;
	int i;
	uint8 *pout;

	studiohdr_t * phdr = (studiohdr_t *)buffer;
	i = LittleLong(phdr->version);
	if (i != STUDIO_VERSION)
	{
		Q_memset(phdr, 0, 244u);
		Q_strcpy(phdr->name, "bogus");
		phdr->length = 244;
		phdr->texturedataindex = 244;
	}

	mod->type = mod_studio;
	mod->flags = phdr->flags;
	Cache_Alloc(&mod->cache, phdr->length + 1280 * phdr->numtextures, mod->name);
	pout = (uint8 *)mod->cache.data;
	if (pout)
	{
		if (phdr->textureindex && phdr->numtextures > 0)
		{
			Q_memcpy(pout, buffer, phdr->texturedataindex);
			poutdata = pout + phdr->texturedataindex;
			pindata = (uint8*)buffer + phdr->texturedataindex;
			ptexture = (mstudiotexture_t *)(pout + phdr->textureindex);
			for (i = 0; i < phdr->numtextures; i++, ptexture++)
			{
				size = ptexture->height * ptexture->width;
				ptexture->index = poutdata - pout;
				Q_memcpy(poutdata, pindata, size);
				poutdata += size;
				pindata += size;

				for (int j = 0; j < 256; j++, pindata += 3, poutdata += 8)
				{
					((uint16*)poutdata)[0] = texgammatable[pindata[0]];
					((uint16*)poutdata)[1] = texgammatable[pindata[1]];
					((uint16*)poutdata)[2] = texgammatable[pindata[2]];
					((uint16*)poutdata)[3] = 0;
				}
			}
		}
		else
		{
			Q_memcpy(pout, buffer, phdr->length);
		}
	}
}

void Mod_LoadStudioModel(model_t * mod, void * buffer)
{
	return Call_Function<void, model_t*, void*>(0x61AD0, mod, buffer);
	uint8 *poutdata;
	uint8 *pindata;
	mstudiotexture_t *ptexture;
	int size;
	int i;
	uint8 *pout;

	studiohdr_t * phdr = (studiohdr_t *)buffer;
	i = LittleLong(phdr->version);
	if (i != STUDIO_VERSION)
	{
		Q_memset(phdr, 0, 244u);
		Q_strcpy(phdr->name, "bogus");
		phdr->length = 244;
		phdr->texturedataindex = 244;
	}

	mod->type = mod_studio;
	mod->flags = phdr->flags;
	int total = (phdr->textureindex) ? phdr->texturedataindex : phdr->length;
	Cache_Alloc(&mod->cache, total, mod->name);
	pout = (uint8 *)mod->cache.data;
	if (!pout)
		return;

	if (phdr->textureindex)
	{
		ptexture = (mstudiotexture_t *)((byte*)phdr + (int)phdr->textureindex);
		for (i = 0; i < phdr->numtextures; i++)//, ptexture++)
		{
			int width = ptexture[i].width;
			int height = ptexture[i].height;
			int flags = ptexture[i].flags;
			int size = width*height;

			char name[256];
			Q_snprintf(name, 256, "%s%s", mod->name, ptexture[i].name);

			uchar* pPal = (uchar*)phdr + ptexture[i].index + size;
			uchar* data = (uchar*)phdr + ptexture[i].index;

			ptexture[i].index = GL_LoadTexture(name, GLT_STUDIO, width, height, data, flags & 8, flags & 64, pPal);
		}
	}
	Q_memcpy(pout, buffer, total);
}
