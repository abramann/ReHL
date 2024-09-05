#pragma once

#include "cvar.h"


extern int msg_buckets[64];

extern int total_data[64];

typedef struct event_hook_s
{
	event_hook_s* next;
	char* name;
	void(*pfnEvent)(event_args_s*);
} event_hook_t;

#ifdef SHARED_GAME_DATA
extern event_hook_t* & g_pEventHooks;
#else
extern event_hook_t* g_pEventHooks;
#endif

void CL_InitEventSystem();

pfnUserMsgHook HookServerMsg(const char* pszName, pfnUserMsgHook pfn);

void CL_HookEvent(char* name, void(*pfnEvent)(event_args_t*));

qboolean CL_RequestMissingResources(void);