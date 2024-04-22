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
	UserMsg* pHooked = nullptr;

	for (UserMsg* pClientMsg = gClientUserMsgs; pClientMsg; pClientMsg = pClientMsg->next)
	{
		if (!Q_stricmp(pszName, pClientMsg->szName))
		{
			pHooked = pClientMsg;
			if (pHooked->pfn == pfn)
			{
				return pHooked->pfn;
			}
		}
	}

	if (!pHooked)
	{
		gClientUserMsgs = (UserMsg*)Mem_ZeroMalloc(sizeof(UserMsg));
		Q_strncpy(gClientUserMsgs->szName, pszName, 15);
		gClientUserMsgs->szName[15] = 0;
		gClientUserMsgs->pfn = pfn;
		gClientUserMsgs->next = nullptr;// gClientUserMsgs;
		return nullptr;
	}

	UserMsg* pNewMsg = (UserMsg*)Mem_ZeroMalloc(sizeof(UserMsg));
	Q_memcpy(pNewMsg, pHooked, sizeof(UserMsg));
	pNewMsg->pfn = pfn;
	pNewMsg->next = gClientUserMsgs;
	gClientUserMsgs = pNewMsg;
	return nullptr;
}

void CL_HookEvent(char * name, void(*pfnEvent)(event_args_t *))
{
	g_engdstAddrs.pfnHookEvent(&name, &pfnEvent);

	if (!name || (*name == '\0'))
	{
		Con_Printf("CL_HookEvent:  Must provide a valid event name\n", &pfnEvent);
		return;
	}

	if (!pfnEvent)
	{
		Con_Printf("CL_HookEvent:  Must provide an event hook callback\n", &pfnEvent);
		return;
	}

	event_hook_t* pHooked = g_pEventHooks;

	if (g_pEventHooks)
	{
		while (1)
		{
			pHooked = pHooked->next;
			if (!pHooked)
				break;

			if (!(!pHooked->name || Q_stricmp(name, pHooked->name)))
			{
				Con_DPrintf("CL_HookEvent:  Called on existing hook, updating event hook\n");
				pHooked->pfnEvent = pfnEvent;
				break;
			}
		}
	}

	if (!pHooked)
	{
		event_hook_t* pNewEvent = (event_hook_t *)Mem_ZeroMalloc(0xCu);
		pNewEvent->name = Mem_Strdup(name);
		pNewEvent->pfnEvent = pfnEvent;
		pNewEvent->next = g_pEventHooks;
		g_pEventHooks = pNewEvent;
	}
}