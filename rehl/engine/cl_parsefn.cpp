#include "precompiled.h"

int msg_buckets[64];
int total_data[64];

event_hook_t* g_pEventHooks;

UserMsg* gClientUserMsgs = nullptr;

void CL_InitEventSystem(void)
{
	g_pEventHooks = NULL;
}

void CL_ShutDownUsrMessages()
{
	UserMsg* p = gClientUserMsgs; // eax

	if (gClientUserMsgs)
	{
		while (1)
		{
			Mem_Free(p->next);

			if (!p->next)
				break;

			p = p->next;
		}

		gClientUserMsgs = NULL;
	}
}

pfnUserMsgHook HookServerMsg(const char* pszName, pfnUserMsgHook pfn)
{
	NOT_IMPLEMENTED_IGNORE;
		// Give clear names 
		UserMsg *v2; // ebx
	UserMsg *v3; // edi
	pfnUserMsgHook result; // eax
	char *v5; // eax
	UserMsg *v6; // eax
	UserMsg *v7; // edx
	char *v8; // [esp+1Ch] [ebp-20h]
	UserMsg *v9; // [esp+1Ch] [ebp-20h]

	v2 = gClientUserMsgs;
	if (!gClientUserMsgs)
	{
		v5 = (char *)Mem_ZeroMalloc(0x20u);
	LABEL_12:
		v9 = (UserMsg *)v5;
		Q_strncpy(v5 + 8, pszName, 15);
		v6 = v9;
		v9->szName[15] = 0;
		goto LABEL_9;
	}
	v3 = 0;
	do
	{
		if (!Q_stricmp(pszName, v2->szName))
		{
			result = v2->pfn;
			if (result == pfn)
				return result;
			v3 = v2;
		}
		v2 = v2->next;
	} while (v2);
	v5 = (char *)Mem_ZeroMalloc(0x20u);
	if (!v3)
		goto LABEL_12;
	v8 = v5;
	Q_memcpy(v5, v3, 32);
	v6 = (UserMsg *)v8;
LABEL_9:
	v7 = gClientUserMsgs;
	v6->pfn = pfn;
	gClientUserMsgs = v6;
	v6->next = v7;
	return 0;
}

void CL_HookEvent(char * name, void(*pfnEvent)(event_args_t *))
{
	// TODO: give clear names

	event_hook_t *v2; // ebx
	event_hook_t *v3; // ebx
	event_hook_t *v4; // eax

	g_engdstAddrs.pfnHookEvent(&name, &pfnEvent);
	if (name && *name)
	{
		if (pfnEvent)
		{
			v2 = g_pEventHooks;
			if (g_pEventHooks)
			{
				while (!v2->name || Q_stricmp(name, v2->name))
				{
					v2 = v2->next;
					if (!v2)
						goto LABEL_8;
				}
				Con_DPrintf("CL_HookEvent:  Called on existing hook, updating event hook\n");
				v2->pfnEvent = pfnEvent;
			}
			else
			{
			LABEL_8:
				v3 = (event_hook_t *)Mem_ZeroMalloc(0xCu);
				v3->name = Mem_Strdup(name);
				v3->pfnEvent = pfnEvent;
				v4 = g_pEventHooks;
				g_pEventHooks = v3;
				v3->next = v4;
			}
		}
		else
		{
			Con_Printf("CL_HookEvent:  Must provide an event hook callback\n", &pfnEvent);
		}
	}
	else
	{
		Con_Printf("CL_HookEvent:  Must provide a valid event name\n", &pfnEvent);
	}
}

