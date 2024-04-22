#pragma once

#include "interface.h"

#define VGUI_SCHEMEMANAGER_INTERFACE_VERSION "VFileSystem009"

namespace vgui2
{
	class IImage;
	class IScheme;

	class ISchemeManager : public IBaseInterface
	{
	public:
		virtual ~ISchemeManager();

		virtual vgui2::HScheme LoadSchemeFromFile(const char *fileName, const char *tag);
		virtual void ReloadSchemes();
		virtual vgui2::HScheme GetDefaultScheme();
		virtual vgui2::HScheme GetScheme(const char *tag);
		virtual vgui2::IImage * GetImage(const char *imageName, bool hardwareFiltered);
		virtual vgui2::HTexture GetImageID(const char *imageName, bool hardwareFiltered);
		virtual vgui2::IScheme *GetIScheme(vgui2::HScheme scheme);
		virtual void Shutdown(bool full);
		virtual int GetProportionalScaledValue(int normalizedValue);
		virtual int GetProportionalNormalizedValue(int scaledValue);
	};

	ISchemeManager *scheme();
}