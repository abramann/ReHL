#pragma once

#include "vgui2\VGui.h"

class KeyValues;

namespace vgui2
{
	class IScheme;

	class IBorder
	{
	public:
		enum sides_e
		{
			SIDE_LEFT = 0x0,
			SIDE_TOP = 0x1,
			SIDE_RIGHT = 0x2,
			SIDE_BOTTOM = 0x3,
		};

		virtual void Paint(vgui2::VPANEL panel);
		virtual void Paint(int x, int y, int wide, int tall);
		virtual void Paint(int x, int y, int wide, int tall, int breakSide, int breakStart, int breakEnd);
		virtual void SetInset(int left, int top, int right, int bottom);
		virtual void GetInset(int& left, int& top, int& right, int& bottom);
		virtual void ApplySchemeSettings(vgui2::IScheme *pScheme, KeyValues *inResourceData);
		virtual const char *GetName();
		virtual void SetName(const char *name);

	};
}