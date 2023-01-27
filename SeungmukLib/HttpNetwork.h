#pragma once
#include "pch.h"
#include "httplib.h"

class HttpNetwork : public Network
{
public:
	virtual bool Initialize(const char* configFile) override;
	void Listen();

public:
	virtual void SetAPI() = 0;

public:
	httplib::Server m_Server;

private:
	char	*m_IP;
	int		m_Port;
};

