#pragma once
#include "pch.h"
#include <array>

struct SOCKETINFO
{
public:
    char            Ip[16];
    int             Port;
    int             WorkerThreadCount;
    SERVER_STATUS   Status;
};

class IOCP : public Network, public SOCKETINFO
{
private:
    SOCKET						            m_ListenSocket;
    HANDLE						            m_Iocp;
    Thread*                                 m_AcceptThread;
    array<Thread*, WORKER_THREAD_SIZE>		m_WorkerThread;

private:
    bool						CreateListenSocket();
    static DWORD WINAPI			AcceptThread(LPVOID serverPtr);
    static DWORD WINAPI			WorkerThread(LPVOID serverPtr);

public:
    IOCP();
    virtual ~IOCP();

    virtual bool	Initialize() override;
    SOCKET			GetListenSocket();
    HANDLE			GetIOCPHandle();
    void			OnAccept(SOCKET accepter, SOCKADDR_IN addrInfo);
};