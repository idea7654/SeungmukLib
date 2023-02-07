#pragma once
#include "pch.h"

class UdpNetwork : public Network
{
public:
	UdpNetwork();
	~UdpNetwork();

public:
	virtual bool	Initialize(const char *configFile) override;
	bool			End();
	bool			Bind();
	bool			RecvFrom();
	virtual bool	Send(unsigned char* sendMsg, int packetSize, UDP_SOCKETINFO sendData) override;

	void			CreateWorkerThread();
	void			WorkerThread();

	void			WriteEvent();

public:
	HANDLE			m_RecvEvent;
	HANDLE			m_WriteEvent;

private:
	SOCKET			m_Socket;
	SOCKADDR_IN		m_ClientInfo;
	WSADATA			m_WsaData;
	SOCKADDR_IN		m_UdpRemoteInfo;

	CircularQueue<UDP_SOCKETINFO>	m_ReadQueue;
	CircularQueue<UDP_SOCKETINFO>	m_WriteQueue;
};

