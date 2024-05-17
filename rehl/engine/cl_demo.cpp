#include "precompiled.h"
#include "DemoPlayerWrapper.h"

char gDemoMessageBuffer[512] = {};
extern int host_framecount;
demo_info_t g_rp;

void CL_GetDemoViewInfo_OLD(ref_params_t *rp, vec_t *view, int *viewmodel);

#pragma pack(push, 1)

struct demo_anim_t
{
	byte cmd;
	float time;
	int frame;
	int anim;
	int body;
};

struct demo_command_t
{
	byte cmd;
	float time;
	int frame;
};

struct demo_event_t
{
	byte cmd;
	float time;
	int frame;
	int flags;
	int idx;
	float delay;
};

struct demo_sound1_t
{
	byte cmd;
	float time;
	int frame;
	int channel;
	int length;
};

struct hud_command_t
{
	byte cmd;
	float time;
	int frame;
	char szNameBuf[64];
};

struct cl_demo_data_t
{
	byte cmd;
	float time;
	int frame;
	vec3_t origin;
	vec3_t viewangles;
	int iWeaponBits;
	float fov;
};

#pragma pack(pop)

struct demo_sound2_t
{
	float volume;
	float attenuation;
	int flags;
	int pitch;
};

struct sequence_info_t
{
	int incoming_sequence;
	int incoming_acknowledged;
	int incoming_reliable_acknowledged;
	int incoming_reliable_sequence;
	int outgoing_sequence;
	int reliable_sequence;
	int last_reliable_sequence;
	int length;
};

client_textmessage_t tm_demomessage =
{
	0,
	255, 255, 255, 255,
	255, 255, 255, 255,
	-1, -1,
	0, 0,
	0, 0,
	"__DEMOMESSAGE__",
	gDemoMessageBuffer };

demo_api_t demoapi =
{
	&CL_DemoAPIRecording,
	&CL_DemoAPIPlayback,
	&CL_DemoAPITimedemo,
	&CL_WriteClientDLLMessage };

int CL_DemoAPIRecording()
{
	return (unsigned int)(g_pcls.demorecording != false);
}

int CL_DemoAPIPlayback()
{
	return (unsigned int)(g_pcls.demoplayback != false);
}

int CL_DemoAPITimedemo()
{
	return (unsigned int)(g_pcls.timedemo != false);
}

void CL_WriteClientDLLMessage(int size, byte* buf)
{
	unsigned long localTime;
	float f;
	byte cmd;

	if ((CL_DemoAPIRecording() && (g_pcls.demofile != NULL)) && (size > -1))
	{
		cmd = '\t';
		FS_Write(&cmd, 1, 1, g_pcls.demofile);
		f = (realtime - g_pcls.demostarttime);
		FS_Write(&f, 4, 1, g_pcls.demofile);
		localTime = (host_framecount - g_pcls.demostartframe);
		FS_Write(&localTime, 4, 1, g_pcls.demofile);
		FS_Write(&size, 4, 1, g_pcls.demofile);
		FS_Write(buf, size, 1, g_pcls.demofile);
	}
	return;
}

void CL_WriteDLLUpdate(client_data_t* cdata)
{
	cl_demo_data_t demcmd;

	if (g_pcls.demofile != NULL)
	{
		demcmd.cmd = '\x04';
		demcmd.time = (realtime - g_pcls.demostarttime);
		demcmd.frame = (host_framecount - g_pcls.demostartframe);
		demcmd.origin[0] = cdata->origin[0];
		demcmd.origin[1] = cdata->origin[1];
		demcmd.origin[2] = cdata->origin[2];
		demcmd.viewangles[0] = cdata->viewangles[0];
		demcmd.viewangles[1] = cdata->viewangles[1];
		demcmd.viewangles[2] = cdata->viewangles[2];
		demcmd.iWeaponBits = cdata->iWeaponBits;
		demcmd.fov = cdata->fov;
		FS_Write(&demcmd, 41, 1, g_pcls.demofile);
	}
	return;
}

void CL_DemoAnim(int anim, int body)
{
	demo_anim_t demcmd;

	if (g_pcls.demofile != NULL)
	{
		demcmd.cmd = '\a';
		demcmd.time = (realtime - g_pcls.demostarttime);
		demcmd.frame = (host_framecount - g_pcls.demostartframe);
		demcmd.anim = anim;
		demcmd.body = body;
		FS_Write(&demcmd, 17, 1, g_pcls.demofile);
	}
	return;
}

void CL_DemoEvent(int flags, int idx, float delay, event_args_t* pargs)
{
	demo_event_t demcmd;

	if (g_pcls.demofile != NULL)
	{
		demcmd.cmd = '\x06';
		demcmd.time = (realtime - g_pcls.demostarttime);
		demcmd.frame = (host_framecount - g_pcls.demostartframe);
		demcmd.flags = flags;
		demcmd.idx = idx;
		demcmd.delay = delay;
		FS_Write(&demcmd, 21, 1, g_pcls.demofile);
		FS_Write(pargs, 72, 1, g_pcls.demofile);
	}
	return;
}


void CL_BeginDemoStartup(void)
{
	if (g_pcls.demoheader != (FileHandle_t)0x0) {
		FS_Close(g_pcls.demoheader);
	}
	g_pcls.demoheader = FS_OpenPathID("demoheader.dmf", "w+b", "GAMECONFIG");
	if (g_pcls.demoheader != (FileHandle_t)0x0) {
		Con_DPrintf("Spooling demo header.\n");
		return;
	}
	Con_DPrintf("ERROR: couldn\'t open temporary header file.\n");
	return;
}



void CL_StopPlayback(void)
{
	NOT_IMPLEMENTED;
	//float fVar1;
	//int iVar2;

	//if (g_pcls.demoplayback != false) {
	//	iVar2 = DemoPlayer_IsActive();
	//	if (iVar2 == 0) {
	//		FS_Close(g_pcls.demofile);
	//	}
	//	else {
	//		DemoPlayer_Stop();
	//	}
	//	g_pcls.demoplayback = false;
	//	g_pcls.demofile = (FileHandle_t)0x0;
	//	g_pcls.state = ca_disconnected;
	//	g_pcls.demoframecount = 0;
	//	if (demodir.p_rgEntries != (demoentry_t*)0x0) {
	//		Mem_Free(demodir.p_rgEntries);
	//	}
	//	demodir.p_rgEntries = (demoentry_t*)0x0;
	//	demodir.nEntries = 0;
	//	if (cl_gamegauge.value != 0.0) {
	//		CL_FinishGameGauge();
	//	}
	//	if (g_pcls.timedemo != false) {
	//		g_pcls.timedemo = false;
	//		iVar2 = (host_framecount - g_pcls.td_startframe) + -1;
	//		fVar1 = 1.0;
	//		if ((float)realtime - g_pcls.td_starttime != 0.0) {
	//			fVar1 = (float)realtime - g_pcls.td_starttime;
	//		}
	//		Con_Printf("%i frames %5.3f seconds %5.3f fps\n", iVar2, (double)fVar1,
	//			(double)((float)iVar2 / fVar1));
	//	}
	//	Cbuf_AddText("cd stop\n");
	//	return;
	//}
	return;
}

void CL_Stop_f(void)
{
	NOT_IMPLEMENTED;
	Con_Printf("Not recording a demo.\n");
	return;
}

void CL_SetDemoViewInfo(ref_params_t * rp, vec_t * view, int viewmodel)
{
	g_rp.rp = *rp;
	g_rp.movevars = *rp->movevars;
	VectorCopy(g_rp.view, view);
	g_rp.viewmodel = viewmodel;
}

void CL_GetDemoViewInfo(ref_params_t * rp, float * view, int * viewmodel)
{
	if (DemoPlayer_IsActive())
		DemoPlayer_GetDemoViewInfo(rp, view, viewmodel);
	else
		CL_GetDemoViewInfo_OLD(rp, view, viewmodel);
}

void CL_GetDemoViewInfo_OLD(ref_params_t *rp, vec_t *view, int *viewmodel)
{
	NOT_IMPLEMENTED;
}