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
	CircularQueue<QUEUE_DATA*>	m_ReadQueue;
	CircularQueue<QUEUE_DATA*>	m_WriteQueue;
	Lock						m_Lock;
	char						m_ReadBuffer[MAX_BUFFER_LENGTH];
	char						m_WriteBuffer[MAX_BUFFER_LENGTH];
	short						m_Port;

public:
	virtual bool				Initialize();
	virtual bool				Send();
};