#include "pch.h"
#include "IOCP.h"
#include <process.h>

unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	IOCompletionPort* pOverlappedEvent = (IOCompletionPort*)p;
	pOverlappedEvent->WorkerThread();
	return 0;
}

IOCompletionPort::IOCompletionPort()
{
	m_bWorkerThread = true;
	m_bAccept = true;
}

IOCompletionPort::~IOCompletionPort()
{
	WSACleanup();
	// 다 사용한 객체를 삭제
	/*if (m_pSocketInfo)
	{
		delete[] m_pSocketInfo;
		m_pSocketInfo = NULL;
	}*/

	/*if (m_pWorkerHandle)
	{
		delete[] m_pWorkerHandle;
		m_pWorkerHandle = NULL;
	}*/
}

bool IOCompletionPort::Initialize(const char *configFile)
{
	WSADATA wsaData;
	int nResult;

	Json serverInfo(configFile);
	m_Port = serverInfo.GetValue<int>("port");

	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (nResult != 0)
	{
		g_Log.error("Winsock Initialize Failed\n");
		return false;
	}

	// 소켓 생성
	m_ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_ListenSocket == INVALID_SOCKET)
	{
		g_Log.error("Socket Create Failed\n");
		return false;
	}

	// 서버 정보 설정
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(m_Port);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 소켓 설정
	nResult = ::bind(m_ListenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	if (nResult == SOCKET_ERROR)
	{
		g_Log.error("Bind Failed\n");
		closesocket(m_ListenSocket);
		WSACleanup();
		return false;
	}

	nResult = ::listen(m_ListenSocket, 5);
	if (nResult == SOCKET_ERROR)
	{
		g_Log.warn("Listen Failed\n");
		closesocket(m_ListenSocket);
		WSACleanup();
		return false;
	}
	return true;
}

void IOCompletionPort::StartServer()
{
	int nResult;
	// Client Info
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	SOCKET clientSocket;
	DWORD recvBytes;
	DWORD flags;

	// Create Completion Port Object
	m_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// Create Worker Thread
	if (!CreateWorkerThread()) return;

	printf_s("[INFO] Server Start...\n");

	// Accept Client
	while (m_bAccept)
	{
		clientSocket = WSAAccept(m_ListenSocket, (struct sockaddr*)&clientAddr, &addrLen, NULL, NULL);
		if (!SetSocketOpt())
		{
			g_Log.warn("Socket Opt Failed!\n");
			return;
		}

		if (clientSocket == INVALID_SOCKET)
		{
			g_Log.warn("Accept Failed!\n");
			return;
		}

		m_SocketInfo = new TCP_SOCKETINFO();
		m_SocketInfo->socket = clientSocket;
		m_SocketInfo->recvBytes = 0;
		m_SocketInfo->sendBytes = 0;
		m_SocketInfo->dataBuf.len = MAX_BUFFER_LENGTH;
		m_SocketInfo->dataBuf.buf = m_SocketInfo->messageBuffer;
		flags = 0;

		m_IOCP = CreateIoCompletionPort((HANDLE)clientSocket, m_IOCP, (DWORD)m_SocketInfo, 0);

		nResult = WSARecv(m_SocketInfo->socket, &m_SocketInfo->dataBuf, 1, &recvBytes, &flags, &(m_SocketInfo->overlapeed), NULL);

		if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			g_Log.warn("IO Pending Failed: %d\n", WSAGetLastError());
			return;
		}

	}
}

bool IOCompletionPort::CreateWorkerThread()
{
	for (int i = 0; i < WORKER_THREAD_SIZE; i++)
	{
		THREAD_MANAGER.CreateThread<unsigned int __stdcall (LPVOID)>(CallWorkerThread, "WorkerThread", this);
	}
	printf_s("[INFO] Worker Thread Start...\n");
	return true;
}

void IOCompletionPort::WorkerThread()
{
	// 함수 호출 성공 여부
	BOOL	bResult;
	int		nResult;
	// Overlapped I/O 작업에서 전송된 데이터 크기
	DWORD	recvBytes;
	DWORD	sendBytes;
	// Completion Key를 받을 포인터 변수
	TCP_SOCKETINFO* completionKey;
	// I/O 작업을 위해 요청한 Overlapped 구조체를 받을 포인터
	TCP_SOCKETINFO* socketInfo;
	DWORD dwFlags = 0;

	while (m_bWorkerThread)
	{
		// 이 함수로 인해 쓰레드들은 WaitingThread Queue 에 대기상태로 들어가게 됨
		// 완료된 Overlapped I/O 작업이 발생하면 IOCP Queue 에서 완료된 작업을 가져와 뒷처리를 함

		bResult = GetQueuedCompletionStatus(m_IOCP, &recvBytes, (PULONG_PTR)&completionKey, (LPOVERLAPPED*)&socketInfo, INFINITE);

		if (!bResult && recvBytes == 0)
		{
			printf_s("[INFO] socket(%d) disconnect \n", socketInfo->socket);
			closesocket(socketInfo->socket);
			free(socketInfo);
			continue;
		}

		socketInfo->dataBuf.len = recvBytes;

		if (recvBytes == 0)
		{
			closesocket(socketInfo->socket);
			free(socketInfo);
			continue;
		}
		else
		{
			Receive((char*)socketInfo->messageBuffer, socketInfo);

			//stSOCKETINFO 데이터 초기화
			ZeroMemory(&(socketInfo->overlapeed), sizeof(OVERLAPPED));
			socketInfo->dataBuf.len = MAX_BUFFER_LENGTH;
			socketInfo->dataBuf.buf = socketInfo->messageBuffer;
			ZeroMemory(socketInfo->messageBuffer, MAX_BUFFER_LENGTH);
			socketInfo->recvBytes = 0;
			socketInfo->sendBytes = 0;

			dwFlags = 0;

			nResult = WSARecv(socketInfo->socket, &(socketInfo->dataBuf), 1, &recvBytes, &dwFlags, (LPWSAOVERLAPPED) & (socketInfo->overlapeed), NULL);

			if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
			{
				g_Log.warn("WSARecv Failed: %d\n", WSAGetLastError());
			}
		}
	}
}

bool IOCompletionPort::Receive(char pPacket[], TCP_SOCKETINFO* pSocket)
{
	QUEUE_DATA newData;
	newData.clientInfo = pSocket;
	newData.packetData = pPacket;
	newData.type = OVERLAPPED_TYPE::RECV;

	if (!m_ReadQueue.Push(newData))
		return false;

	ProcessPacket();

	return true;
}

bool IOCompletionPort::Send(unsigned char* sendMsg, TCP_SOCKETINFO* pSocket, int packetLength)
{
	/*QUEUE_DATA newData;
	newData.clientInfo = pSocket;
	newData.packetData = reinterpret_cast<char*>(sendMsg);
	newData.type = OVERLAPPED_TYPE::SEND;

	if (!m_WriteQueue.Push(newData))
		return false;*/

	pSocket->dataBuf.buf = reinterpret_cast<char*>(sendMsg);
	pSocket->dataBuf.len = packetLength;

	SendMsg(pSocket);

	return true;
}

void IOCompletionPort::SendMsg(TCP_SOCKETINFO* pSocket)
{
	int		nResult;
	DWORD	sendBytes;
	DWORD	dwFlags = 0;

	nResult = WSASend(
		pSocket->socket,
		&(pSocket->dataBuf),
		1,
		&sendBytes,
		dwFlags,
		NULL,
		NULL
	);

	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		g_Log.warn("WSASend Failed: %d\n", WSAGetLastError());
	}
}

TCP_SOCKETINFO* IOCompletionPort::GetSocketInfo()
{
	return this->m_SocketInfo;
}

bool IOCompletionPort::SetSocketOpt()
{
	tcp_keepalive keepAliveSet = { 0 }, returned = { 0 };
	keepAliveSet.onoff = 1;
	keepAliveSet.keepalivetime = 3000; //Keep Alive in 3 sec
	keepAliveSet.keepaliveinterval = 3000; //Resend in No-Reply

	DWORD dwBytes;
	if (WSAIoctl(m_ListenSocket, SIO_KEEPALIVE_VALS, &keepAliveSet, sizeof(keepAliveSet), &returned, sizeof(returned), &dwBytes, NULL, NULL) != 0)
		return false;

	return true;
}
