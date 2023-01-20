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
		printf_s("[ERROR] winsock Initialize Failed\n");
		return false;
	}

	// 소켓 생성
	m_ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_ListenSocket == INVALID_SOCKET)
	{
		printf_s("[ERROR] Socket Create Failed\n");
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
		printf_s("[ERROR] bind Failed\n");
		closesocket(m_ListenSocket);
		WSACleanup();
		return false;
	}

	nResult = ::listen(m_ListenSocket, 5);
	if (nResult == SOCKET_ERROR)
	{
		printf_s("[ERROR] listen Failed\n");
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

		if (clientSocket == INVALID_SOCKET)
		{
			printf_s("[ERROR] Accept 실패\n");
			return;
		}

		m_SocketInfo = new SOCKETINFO();
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
			printf_s("[ERROR] IO Pending 실패 : %d", WSAGetLastError());
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
	SOCKETINFO* completionKey;
	// I/O 작업을 위해 요청한 Overlapped 구조체를 받을 포인터
	SOCKETINFO* socketInfo;
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
				printf_s("[ERROR] WSARecv Failed : %d", WSAGetLastError());
			}
		}
	}
}

bool IOCompletionPort::Receive(char pPacket[], SOCKETINFO* pSocket)
{
	QUEUE_DATA newData;
	newData.clientInfo = pSocket;
	newData.packetData = pPacket;
	newData.type = OVERLAPPED_TYPE::RECV;

	if (!m_ReadQueue.Push(newData))
		return false;

	//auto packet = m_ReadQueue.Pop();
	//packet.type
	ProcessPacket();
}

bool IOCompletionPort::Send(char* sendMsg, SOCKETINFO* pSocket, int packetSize)
{
	QUEUE_DATA newData;
	newData.clientInfo = pSocket;
	newData.packetData = sendMsg;
	newData.type = OVERLAPPED_TYPE::SEND;

	if (!m_WriteQueue.Push(newData))
		return false;

	return true;
}

void IOCompletionPort::SendMsg(SOCKETINFO* pSocket)
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
		printf_s("[ERROR] WSASend Failed : %d", WSAGetLastError());
	}
}

SOCKETINFO* IOCompletionPort::GetSocketInfo()
{
	return this->m_SocketInfo;
}