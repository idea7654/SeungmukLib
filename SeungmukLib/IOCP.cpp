#include "pch.h"
#include "IOCP.h"

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
