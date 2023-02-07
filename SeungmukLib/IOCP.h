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
	TCP_SOCKETINFO*	m_SocketInfo;		// 소켓 정보
	SOCKET			m_ListenSocket;		// 서버 리슨 소켓
	HANDLE			m_IOCP;			// IOCP 객체 핸들
	bool			m_bAccept;			// 요청 동작 플래그
	bool			m_bWorkerThread;	// 작업 스레드 동작 플래그

protected:
	CircularQueue<QUEUE_DATA>	m_ReadQueue;
	CircularQueue<QUEUE_DATA>	m_WriteQueue;
};