#include "stdafx.h"
#include "SockMessage.h"
#pragma warning (disable: 4996)

CSockMessage::CSockMessage(void)
{
}


CSockMessage::~CSockMessage(void)
{
}

int CSockMessage::RecvMsg(char *cs)
{
	return atoi(cs);
}
bool CSockMessage::SendMsg(SOCKET &Msock, SOCKET &Fsock, int p, BOOL IsServer)
{
	char s[4];
	sprintf(s, "%d", p);
	if (IsServer)
	{
		return ::send(Fsock, s, strlen(s) + 1, 0) != SOCKET_ERROR;
	}
	else
	{
		return ::send(Msock, s, strlen(s) + 1, 0) != SOCKET_ERROR;
	}
}