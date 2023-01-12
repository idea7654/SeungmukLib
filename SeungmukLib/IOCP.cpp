#include "pch.h"
#include "IOCP.h"

bool IOCP::CreateListenSocket()
{
	m_ListenSocket = WSASocket(AF_INET, SOCK_STREAM, NULL, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_ListenSocket == INVALID_SOCKET) {
		//SErrLog(L"listenSocket fail");
		return false;
	}

	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((u_short)this->Port);
	inet_pton(AF_INET, this->Ip, &(serverAddr.sin_addr));

	int reUseAddr = 1;
	setsockopt(m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reUseAddr, sizeof(reUseAddr));

	int retval = ::bind(m_ListenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR) {
		//SErrLog(L"bind fail");
		return false;
	}

	const int BACK_SOCKETS = 5;
	retval = ::listen(m_ListenSocket, BACK_SOCKETS);
	if (retval == SOCKET_ERROR) {
		//SErrLog(L"listen fail");
		return false;
	}

	array<char, 64> ip;
	inet_ntop(AF_INET, &(serverAddr.sin_addr), ip.data(), ip.size());
	//SLog(L"server listen socket created, ip: %S, port: %d", ip.data(), port_);
	return true;
}

IOCP::IOCP()
{
}

IOCP::~IOCP()
{
	::closesocket(m_ListenSocket);
}

bool IOCP::Initialize()
{
	if (WORKER_THREAD_SIZE < WorkerThreadCount) {
		//SErrLog(L"WorkerThread limit[%d], but config setting [%d]", MAX_IOCP_THREAD, workerThreadCount_);
		//Log
		return false;
	}
	m_Iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, WorkerThreadCount);

	if (m_Iocp == nullptr) {
		return false;
	}
	this->CreateListenSocket();

	m_AcceptThread = THREAD_MANAGER.CreateThread<DWORD __stdcall(LPVOID), LPVOID>(this->AcceptThread, "AcceptThread", this);

	for (int i = 0; i < WorkerThreadCount; ++i) {
		m_WorkerThread[i] = THREAD_MANAGER.CreateThread<DWORD __stdcall(LPVOID), LPVOID>(this->WorkerThread, "WorkerThread", this);
	}
	this->Status = SERVER_READY;

	return true;
}
