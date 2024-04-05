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
#pragma once

typedef struct rect_s
{
	int left, right, top, bottom;
} wrect_t, rect_t;

typedef struct vmode_s
{
	int width;
	int height;
	int bpp;
} vmode_t;

typedef byte pixel_t;	 // a pixel can be one, two, or four bytes

typedef struct viddef_s
{
	pixel_t* buffer;			// invisible buffer
	pixel_t* colormap;			// 256 * VID_GRADES size
	unsigned short* colormap16; // 256 * VID_GRADES size
	int fullbright;				// index of first fullbright color
	unsigned rowbytes;			// may be > width if displayed in a window
	unsigned width;
	unsigned height;
	float aspect; // width / height -- < 0 is taller than wide
	int numpages;
	int recalc_refdef; // if true, recalc vid-based stuff
	pixel_t* conbuffer;
	int conrowbytes;
	unsigned conwidth;
	unsigned conheight;
	int maxwarpwidth;
	int maxwarpheight;
	pixel_t* direct; // direct drawing to framebuffer, if not NULL
} viddef_t;

typedef struct _TargaHeader
{
	unsigned char id_length, colormap_type, image_type;
	unsigned short colormap_index, colormap_length;
	unsigned char colormap_size;
	unsigned short x_origin, y_origin, width, height;
	unsigned char pixel_size, attributes;
} TargaHeader;

extern viddef_t vid; // global video state