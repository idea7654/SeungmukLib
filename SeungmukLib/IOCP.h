#pragma once
#include "pch.h"

class IOCompletionPort : public Network
{
public:
	IOCompletionPort();
	~IOCompletionPort();

	virtual bool	Initialize(const char* configFile) override;
	void			StartServer();
	bool			CreateWorkerThread();
	void			WorkerThread();

	bool			Receive(char pPacket[], TCP_SOCKETINFO* pSocket);
	virtual bool	Send(unsigned char* sendMsg, TCP_SOCKETINFO* pSocket, int packetLength) override;

	void			SendMsg(TCP_SOCKETINFO* pSocket);

	TCP_SOCKETINFO*	GetSocketInfo();
	virtual bool	SetSocketOpt();

private:
	TCP_SOCKETINFO*	m_SocketInfo;		// ���� ����
	SOCKET			m_ListenSocket;		// ���� ���� ����
	HANDLE			m_IOCP;			// IOCP ��ü �ڵ�
	bool			m_bAccept;			// ��û ���� �÷���
	bool			m_bWorkerThread;	// �۾� ������ ���� �÷���

protected:
	CircularQueue<QUEUE_DATA>	m_ReadQueue;
	CircularQueue<QUEUE_DATA>	m_WriteQueue;
};