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
	SOCKETINFO*		m_SocketInfo;		// 소켓 정보
	SOCKET			m_ListenSocket;		// 서버 리슨 소켓
	HANDLE			m_IOCP;			// IOCP 객체 핸들
	bool			m_bAccept;			// 요청 동작 플래그
	bool			m_bWorkerThread;	// 작업 스레드 동작 플래그

protected:
	CircularQueue<QUEUE_DATA>	m_ReadQueue;
	CircularQueue<QUEUE_DATA>	m_WriteQueue;
};