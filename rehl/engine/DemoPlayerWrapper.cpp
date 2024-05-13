#include "precompiled.h"
#include "DemoPlayerWrapper.h"
#include "IDemoPlayer.h"

static IDemoPlayer* s_DemoPlayer = nullptr;
static int s_DemoPlayerMode;

bool DemoPlayer_Init()
{
	s_DemoPlayer = (IDemoPlayer*)gSystemWrapper.GetModule(DEMOPLAYER_INTERFACE_VERSION, "demoplayer");
	if (s_DemoPlayer)
	{
		s_DemoPlayer->SetEditMode(COM_CheckParm("-demoedit"));
		return true;
	}
	else
	{
		Con_DPrintf("Failed to load demo player module.\n");
		return false;
	}
}

bool DemoPlayer_IsActive()
{
	if (s_DemoPlayer)
		return s_DemoPlayerMode != 0;
	return false;
}

int DemoPlayer_ReadDemoMessage(unsigned char *buffer, int size)
{
	if (s_DemoPlayer)
		return s_DemoPlayer->ReadDemoMessage(buffer, size);

	return 0;
}

void DemoPlayer_ReadNetchanState(int *incoming_sequence, int *incoming_acknowledged, int *incoming_reliable_acknowledged, int *incoming_reliable_sequence, int *outgoing_sequence, int *reliable_sequence, int *last_reliable_sequence)
{
	if (s_DemoPlayer)
		s_DemoPlayer->ReadNetchanState(
			incoming_sequence,
			incoming_acknowledged,
			incoming_reliable_acknowledged,
			incoming_reliable_sequence,
			outgoing_sequence,
			reliable_sequence,
			last_reliable_sequence);
}