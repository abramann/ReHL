#pragma once

#include "interface.h"

namespace vgui2
{
	bool VGuiControls_Init(const char *moduleName, CreateInterfaceFn *factoryList, int numFactories);
	void VGuiControls_Shutdown();
	const char* GetControlsModuleName();
}