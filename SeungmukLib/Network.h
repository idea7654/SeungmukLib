#pragma once
#include "pch.h"

typedef enum SERVER_STATUS {
	SERVER_STOP,
	SERVER_INITIALIZE,
	SERVER_READY
};

class Network
{
protected:
	Lock						m_Lock;
	char						m_ReadBuffer[MAX_BUFFER_LENGTH];
	char						m_WriteBuffer[MAX_BUFFER_LENGTH];
	short						m_Port;

public:
	virtual bool				Initialize(const char* configFile) { return false; }
	virtual bool				Send(unsigned char* sendMsg, SOCKETINFO* pSocket) { return false; }
	virtual bool				Send(unsigned char* sendMsg, int packetSize, UDP_QUEUE_DATA sendData) { return false; }
	//이 부분을 부모로 올려서 Template으로 만들던가 하는 방법이 있음..
	virtual void				ProcessPacket() {}

	//Checksum
	//DataLength
};