#pragma once
#include "pch.h"

enum OVERLAPPED_TYPE
{
	ACCEPT = 1,
	RECV,
	SEND,
	CLOSE
};

class TCP_SOCKETINFO : public Session
{
public:
	WSAOVERLAPPED	overlapeed;
	WSABUF			dataBuf;
	SOCKET			socket;
	char			messageBuffer[MAX_BUFFER_LENGTH];
	int				recvBytes;
	int				sendBytes;

public:
	virtual void	Close() override
	{
		::closesocket(socket);
	}

	virtual void	ForceClose(LINGER linger) override
	{
		::setsockopt(socket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));
	}
};

struct QUEUE_DATA
{
	char*				packetData;
	TCP_SOCKETINFO*		clientInfo;
	OVERLAPPED_TYPE		type;
};

class UDP_SOCKETINFO : public Session
{
public:
	char*	packetData;
	char*	ipAddress;
	short	port;
	int		packetLength;
};

enum LOCK_TYPE
{
	READ_LOCK = 0,
	WRITE_LOCK
};

enum DB_TYPE
{
	DB_REDIS,
	DB_MYSQL
};

enum QUERY_TYPE
{
	READ,
	WRITE
};