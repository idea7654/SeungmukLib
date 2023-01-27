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

	bool			Receive(char pPacket[], SOCKETINFO* pSocket);
	virtual bool	Send(unsigned char* sendMsg, SOCKETINFO* pSocket) override;

	void			SendMsg(SOCKETINFO* pSocket);

	SOCKETINFO*		GetSocketInfo();

private:
	SOCKETINFO*		m_SocketInfo;		// ���� ����
	SOCKET			m_ListenSocket;		// ���� ���� ����
	HANDLE			m_IOCP;			// IOCP ��ü �ڵ�
	bool			m_bAccept;			// ��û ���� �÷���
	bool			m_bWorkerThread;	// �۾� ������ ���� �÷���

protected:
	CircularQueue<QUEUE_DATA>	m_ReadQueue;
	CircularQueue<QUEUE_DATA>	m_WriteQueue;
};