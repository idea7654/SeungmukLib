#pragma once
#include "pch.h"

class Session
{
public:
	Session();
	~Session();

	time_t	GetHeartBeat();
	void	UpdateHeartBeat();

	UINT64	GetId();
	void	SetId(UINT64 id);

	virtual void Close() {}
	virtual void ForceClose(LINGER linger){}

protected:
	std::time_t		m_HeartBeat;
	UINT64			m_Id;
};

