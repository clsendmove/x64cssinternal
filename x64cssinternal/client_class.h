#pragma once
#include "dt_common.h"
#include "dt_recv.h"
#include "typedef.h"

class client_class
{
public:
	const char* GetName() const
	{
		return m_pNetworkName;
	}

public:
	CreateClientClassFn m_pCreateFn{};
	CreateEventFn m_pCreateEventFn{};
	const char* m_pNetworkName{};
	RecvTable* m_pRecvTable{};
	client_class* m_pNext{};
	int m_ClassID{};
};
