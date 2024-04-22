#include "precompiled.h"
#include "DemoPlayerWrapper.h"
#include "IDemoPlayer.h"

static IDemoPlayer* s_DemoPlayer = nullptr;

int DemoPlayer_Init()
{
	s_DemoPlayer = (IDemoPlayer*)gSystemWrapper.GetModule(DEMOPLAYER_INTERFACE_VERSION, "demoplayer");
	if (s_DemoPlayer)
	{
		s_DemoPlayer->SetEditMode(COM_CheckParm("-demoedit"));
		return 1;
	}
	else
	{
		Con_DPrintf("Failed to load demo player module.\n");
		return 0;
	}
}