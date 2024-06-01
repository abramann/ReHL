#include "precompiled.h"
#include "net.h"


net_api_query_t* g_queries = nullptr;
net_adrlist_t* g_addresses = nullptr;

net_api_t netapi =
{
	&Net_InitNetworking,
	&Net_Status,
	&Net_SendRequest,
	&Net_CancelRequest,
	&Net_CancelAllRequests,
	&Net_AdrToString,
	&Net_CompareAdr,
	&Net_StringToAdr,
	&Info_ValueForKey,
	&Info_RemoveKey,
	&Info_SetValueForStarKey 
};

void Net_InitNetworking()
{
	NET_Config(true);
}

void Net_APIInit()
{
	g_queries = 0;
	g_addresses = 0;
}

void Net_APIShutDown()
{
	for (net_api_query_t *p = g_queries; p; p = p->next)
	{
		Mem_Free(p);
	}
	
	g_queries = 0;

	for (net_adrlist_t* pAddr = g_addresses; pAddr; pAddr = pAddr->next)
	{
		Mem_Free(pAddr);
	}

	g_addresses = 0;
}

void Net_Status(net_status_t* status)
{
	if (status)
	{
		int inGame = g_pcls.state != ca_disconnected;

		status->connected = inGame;
		status->local_address = net_local_adr;
		status->remote_address = g_pcls.netchan.remote_address;

		if (inGame)
		{
			int allowedPacketLoss = 0;

			if (g_pcls.state == ca_active)
			{
				int packet_loss = (int)g_pcls.packet_loss;

				if (packet_loss < 0)
					packet_loss = 0;

				allowedPacketLoss = 100;
				if (packet_loss <= 100)
					allowedPacketLoss = packet_loss;
			}

			long double time = realtime;
			// status->latency = g_flLatency;
			int connection_time = time - g_pcls.netchan.connect_time;
			status->packet_loss = allowedPacketLoss;
			status->connection_time = connection_time;
			status->rate = rate.value;
		}
		else
		{
			status->packet_loss = 0;
			status->connection_time = 0.0;
			long double value = rate.value;
			status->latency = 0.0;
			status->rate = value;
		}
	}
}

void Net_SendRequest(int context, int request, int flags, double timeout, netadr_t* remote_address, net_api_response_func_t response)
{
	NOT_IMPLEMENTED;
	// TODO: implement - Solokiller
	
	/*
	sizebuf_t msg;
	Q_memset(&msg, 0, 20);

	byte* data;
	msg.data = data;
	msg.cursize = 0;
	msg.buffername = "Net_SendRequest";
	msg.maxsize = 1024;

	// net_api_query_t* v6 = (net_api_query_t*)Mem_ZeroMalloc(0x38);
	// v6->type = request;
	// v6->context = context;
	// v6->requesttime = realtime;
	// v6->flags = flags;
	// v6->timeout = timeout;
	// v6->request = *remote_address;
	// v6->callback = response;
	// net_api_query_t* v7 = g_queries;
	// g_queries = v6;
	// v6->next = v7;

	switch (request)
	{
	case 0:
		// net_adrlist_t v8 = g_addresses;
		// if (g_addresses)
	{
		while (1)
		{
			// net_adrlist_s next = v8->next;
			// Mem_Free(v8);
			// if (!next)
			break;
			// v8 = next;
		}
	}
	// g_addresses = 0;
	MSG_WriteByte(&msg, 101);
	MSG_WriteLong(&msg, 0);
	break;
	case 1:
		MSG_WriteLong(&msg, -1);
		MSG_WriteString(&msg, "ping\n");
		break;
	case 2:
		MSG_WriteLong(&msg, -1);
		MSG_WriteString(&msg, "rules\n");
		break;
	case 3:
		MSG_WriteLong(&msg, -1);
		MSG_WriteString(&msg, "players\n");
		break;
	case 4:
		MSG_WriteLong(&msg, -1);
		MSG_WriteString(&msg, "details\n");
		break;
	default:
		Con_Printf("Unknown request type:  %i\n", request);
		break;
	}

	if (msg.cursize > 0)
		; // NET_SendPacket(NS_CLIENT, msg.cursize, msg.data, v6->request);
		*/
}

void Net_CancelRequest(int context)
{
	NOT_IMPLEMENTED;
}

void Net_CancelAllRequests()
{
	NOT_IMPLEMENTED;
}

char* Net_AdrToString(netadr_t* a)
{
	return NET_AdrToString(*a);
}

int Net_CompareAdr(netadr_t* a, netadr_t* b)
{
	if (b->type != a->type)
		return false;

	if (b->type == NA_LOOPBACK)
		return false;

	if (b->type == NA_IP && a->ip == b->ip)
		return b->port == a->port;

	return false;
}

/*
qboolean NET_StringToSockaddr(const char* s, struct sockaddr* sadr)
{
	struct hostent* h;
	char* colon;
	char copy[128];

	Q_memset(sadr, 0, sizeof(*sadr));
	((struct sockaddr_in*)sadr)->sin_family = AF_INET;
	((struct sockaddr_in*)sadr)->sin_port = 0;

	Q_strcpy(copy, s);
	// strip off a trailing :port if present
	for (colon = copy; *colon; colon++)
		if (*colon == ':')
		{
			*colon = 0;
			((struct sockaddr_in*)sadr)->sin_port = htons((short)atoi(colon + 1));
		}

	if (copy[0] >= '0' && copy[0] <= '9' && Q_strstr(copy, "."))
	{
		*(int*)&((struct sockaddr_in*)sadr)->sin_addr = inet_addr(copy);
	}
	else
	{
		if (!(h = gethostbyname(copy)))
			return 0;
		*(int*)&((struct sockaddr_in*)sadr)->sin_addr = *(int*)h->h_addr_list[0];
	}

	return true;
}
*/

int Net_StringToAdr(char* s, netadr_t* a)
{
	if (strcmp(s, "localhost") && strcmp(s, "127.0.0.1"))
	{
		sockaddr sadr;
		if (NET_StringToSockaddr(s, &sadr) && sadr.sa_family == 2)
		{
			a->type = NA_IP;
			*(DWORD*)a->ip = *(DWORD*)&sadr.sa_data[2];
			a->port = (DWORD)sadr.sa_data;
		}

		return true;
	}
	else
	{
		memset(a, 0, 20);
		a->type = NA_LOOPBACK;

		return true;
	}
	return false;
}


void Net_APICheckTimeouts(void)
{
	NOT_IMPLEMENTED;
}