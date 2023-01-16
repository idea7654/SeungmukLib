#pragma once
#include "pch.h"

enum OVERLAPPED_TYPE
{
	ACCEPT = 1,
	RECV,
	SEND,
	CLOSE
};

struct SOCKETINFO
{
	WSAOVERLAPPED	overlapeed;
	WSABUF			dataBuf;
	SOCKET			socket;
	char			messageBuffer[MAX_BUFFER_LENGTH];
	int				recvBytes;
	int				sendBytes;
};

struct Session
{

};

struct QUEUE_DATA
{
	char*				packetData;
	SOCKETINFO*			clientInfo;
	OVERLAPPED_TYPE		type;
};

struct UDP_QUEUE_DATA
{
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