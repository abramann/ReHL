#pragma once

#include "vgui2\VGui.h"

#define VGUI_SCHEME_INTERFACE_VERSION "VGUI_Scheme009"

namespace vgui2
{
	class IBorder;

	class IScheme : public IBaseInterface
	{
	public:
		virtual ~IScheme();

		virtual const char *GetResourceString(const char *stringName);
		virtual vgui2::IBorder * GetBorder(const char *borderName);
		virtual vgui2::HFont GetFont(const char *fontName, bool proportional);
		virtual Color GetColor(const char *colorName, Color defaultColor);
	};
}