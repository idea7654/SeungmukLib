#include "pch.h"
#include "UdpNetwork.h"

#pragma warning(disable : 4996)

unsigned int CallWorkerThread(LPVOID p)
{
	UdpNetwork* thisClass = (UdpNetwork*)p;
	thisClass->WorkerThread();
	return 0;
}

UdpNetwork::UdpNetwork()
{
	m_Socket = NULL;
	m_RecvEvent = NULL;
	m_WriteEvent = NULL;
}

UdpNetwork::~UdpNetwork()
{
}

bool UdpNetwork::Initialize(const char* configFile)
{
	if (m_Socket)
		return false;

	memset(&m_ReadBuffer, 0, sizeof(m_ReadBuffer));
	memset(&m_WriteBuffer, 0, sizeof(m_WriteBuffer));
	memset(&m_UdpRemoteInfo, 0, sizeof(m_UdpRemoteInfo));

	if (WSAStartup(0x202, &m_WsaData) == SOCKET_ERROR)
	{
		cout << "WinSock Initialize Error " << endl;
		WSACleanup();
	}

	m_Socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_Socket == INVALID_SOCKET)
	{
		cout << "Socket Create Error" << endl;
		closesocket(m_Socket);
		WSACleanup();
		return false;
	}

	m_RecvEvent = CreateEvent(0, false, false, NULL);
	m_WriteEvent = CreateEvent(0, false, false, NULL);

	return true;
}

bool UdpNetwork::End()
{
	m_Socket = NULL;
	memset(&m_ReadBuffer, 0, sizeof(m_ReadBuffer));
	memset(&m_WriteBuffer, 0, sizeof(m_WriteBuffer));
	memset(&m_UdpRemoteInfo, 0, sizeof(m_UdpRemoteInfo));

	CloseHandle(m_RecvEvent);
	CloseHandle(m_WriteEvent);

	return true;
}

bool UdpNetwork::Bind()
{
	if (!m_Socket)
		return false;

	m_UdpRemoteInfo.sin_family = AF_INET;
	m_UdpRemoteInfo.sin_port = htons(m_Port);
	m_UdpRemoteInfo.sin_addr.s_addr = htonl(INADDR_ANY);
	if (::bind(m_Socket, (SOCKADDR*)&m_UdpRemoteInfo, sizeof(m_UdpRemoteInfo)) == SOCKET_ERROR)
	{
		cout << "Failed To Bind" << endl;
		closesocket(m_Socket);
		WSACleanup();
		return false;
	}
	return true;
}

bool UdpNetwork::RecvFrom()
{
	if (!m_Socket)
		return false;
	int clientSize = sizeof(m_ClientInfo);
	int recvLen = 0;

	if ((recvLen = recvfrom(m_Socket, m_ReadBuffer, sizeof(m_ReadBuffer) - 1, 0, (SOCKADDR*)&m_ClientInfo, &clientSize)) == -1)
		return false;
	char* remoteAddress = inet_ntoa(m_ClientInfo.sin_addr);
	int remotePort = htons(m_ClientInfo.sin_port);

	UDP_QUEUE_DATA newData;
	newData.ipAddress = remoteAddress;
	newData.packetData = m_ReadBuffer;
	newData.packetLength = recvLen;
	newData.port = remotePort;

	if (m_ReadQueue.Push(newData) == false)
		return false;

	SetEvent(m_RecvEvent);

	return true;
}

bool UdpNetwork::Send(char* sendMsg, int packetSize, UDP_QUEUE_DATA sendData)
{
	if (!m_Socket)
		return false;

	//|  int32(PacketLength)  |  int32(PacketNumber)  |  data  |
	if (m_WriteQueue.Push(sendData) == false)
		return false;

	SetEvent(m_WriteEvent);
	return true;
}

void UdpNetwork::CreateWorkerThread()
{
	for (int i = 0; i < NUM_MAX_THREAD; i++)
	{
		THREAD_MANAGER.CreateThread<void(LPVOID)>(CallWorkerThread, "WorkerThread", this);
	}
}

void UdpNetwork::WorkerThread()
{
	HANDLE ThreadEvents[2] = { m_RecvEvent, m_WriteEvent };
	DWORD EventID = 0;

	while (true)
	{
		EventID = WaitForMultipleObjects(2, ThreadEvents, false, INFINITE);
		switch (EventID)
		{
		case WAIT_OBJECT_0:
			this->ProcessPacket();
			break;
		case WAIT_OBJECT_0 + 1:
			this->WriteEvent();
			break;
		default:
			break;
		}
	}
}

void UdpNetwork::WriteEvent()
{
	while (!m_WriteQueue.IsEmpty())
	{
		auto sendInfo = m_WriteQueue.Pop();

		m_Lock.EnterLock(LOCK_TYPE::WRITE_LOCK);
		m_ClientInfo.sin_family = AF_INET;
		m_ClientInfo.sin_addr.s_addr = inet_addr(sendInfo.ipAddress);
		m_ClientInfo.sin_port = htons(sendInfo.port);

		int returnVal = sendto(m_Socket, m_WriteBuffer, sendInfo.packetLength, 0, (SOCKADDR*)&m_ClientInfo, sizeof(m_ClientInfo));

		if (returnVal < 0)
			return;
		m_Lock.LeaveLock(LOCK_TYPE::WRITE_LOCK);
	}
}
