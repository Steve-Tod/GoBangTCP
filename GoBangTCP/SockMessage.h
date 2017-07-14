#pragma once
#include "stdafx.h"
class CSockMessage
{

public:
	CSockMessage(void);
	~CSockMessage(void);
	int RecvMsg(char *cs);
	bool SendMsg(SOCKET &Msock, SOCKET &Fsock, int p, BOOL IsServer);
};

