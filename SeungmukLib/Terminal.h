#pragma once
#include "pch.h"

typedef enum TERMINAL_STATUS
{
	TERMINAL_STOP,
	TERMINAL_READY
};

class Terminal
{
protected:

	Network*			m_Network;
	char*				m_TerminalName;
	TERMINAL_STATUS		m_Status;

	char*				m_Ip;
	int					m_Port;

	Thread*				m_ProcessThread;

public:
	Terminal(char *name, Network*);
	~Terminal();

	TERMINAL_STATUS&	GetStatus();

	void				Initialize(const char* configFile);
	void				SendPacket(unsigned char* sendMsg, TCP_SOCKETINFO* pSocket, int packetLength);
	const char*			GetIp();
	void				ConnectProcess();

private:
	void				Run();
};

