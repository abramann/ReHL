#include "precompiled.h"

void CL_MoveToOnHandList(resource_t *pResource)
{
	if (pResource)
	{
		CL_RemoveFromResourceList(pResource);
		CL_AddToResourceList(pResource, &m1.resourcesonhand);
	}
	else
	{
		Con_DPrintf("Null resource passed to CL_MoveToOnHandList\n");
	}
}

void CL_RemoveFromResourceList(resource_t *pResource)
{
	if (pResource->pPrev == NULL || pResource->pNext == NULL)
		Sys_Error("mislinked resource in CL_RemoveFromResourceList\n");

	if (pResource->pNext == pResource || pResource->pPrev == pResource)
		Sys_Error("attempt to free last entry in list.\n");

	pResource->pPrev->pNext = pResource->pNext;
	pResource->pNext->pPrev = pResource->pPrev;
	pResource->pPrev = NULL;
	pResource->pNext = NULL;
}

void CL_AddToResourceList(resource_t *pResource, resource_t *pList)
{
	if (pResource->pPrev || pResource->pNext)
	{
		Con_Printf("Resource already linked\n");
		return;
	}

	if (pList->pPrev == NULL || pList->pNext == NULL)
		Sys_Error("Resource list corrupted.\n");

	pResource->pPrev = pList->pPrev;
	pResource->pNext = pList;
	pList->pPrev->pNext = pResource;
	pList->pPrev = pResource;
}