#include "precompiled.h"

int msg_buckets[64];
int total_data[64];

VAR(event_hook_t*, g_pEventHooks, 0x1EF42);
VAR(UserMsg*, gClientUserMsgs, 0x1A8C1);

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

	UserMsg* pNewMsg = (UserMsg*)Mem_ZeroMalloc(sizeof(UserMsg));
	if (pHooked)
		Q_memcpy(pNewMsg, pHooked, sizeof(UserMsg));
	else
	{
		Q_strncpy(pNewMsg->szName, pszName, 15);
		pNewMsg->szName[15] = 0;
	}
	pNewMsg->pfn = pfn;
	if (!gClientUserMsgs)
	{
		gClientUserMsgs = pNewMsg;
		gClientUserMsgs->next = nullptr;
	}
	else
	{
		pNewMsg->next = gClientUserMsgs;
		gClientUserMsgs = pNewMsg;
	}
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

qboolean CL_RequestMissingResources(void)
{
	if (g_pcls.dl.doneregistering || g_pcls.dl.custom == false && g_pcls.state != ca_uninitialized)
		return false;

	resource_t* p = g_pcl.resourcesneeded.pNext;
	if (p == &g_pcl.resourcesneeded)
	{
		g_pcls.dl.custom = false;
		g_pcls.dl.doneregistering = true;
		return false;
	}
	else
	{
		if ((p->ucFlags & 2) == 0)
		{
			NOT_IMPLEMENTED;

			//CL_MoveToOnHandList(p);
			return true;
		}
	}

	return false;
}
