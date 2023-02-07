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
	virtual bool				Send(unsigned char* sendMsg, TCP_SOCKETINFO* pSocket, int packetLength) { return false; }
	virtual bool				Send(unsigned char* sendMsg, int packetSize, UDP_SOCKETINFO sendData) { return false; }
	//�� �κ��� �θ�� �÷��� Template���� ������� �ϴ� ����� ����..
	virtual void				ProcessPacket() {}
	
	virtual bool				SetSocketOpt() { return false; }

	//Checksum
	//DataLength
};