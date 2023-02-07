#include "pch.h"
#include "Terminal.h"

Terminal::Terminal(char* name, Network *network)
{
	m_TerminalName = name;
	m_Network = network;
}

Terminal::~Terminal()
{
	m_Status = TERMINAL_STOP;
}

TERMINAL_STATUS& Terminal::GetStatus()
{
	return m_Status;
}

void Terminal::Initialize(const char* configFile)
{
	Json config(configFile);

	m_Ip = const_cast<char*>(config.GetValue<std::string>("Ip").c_str());
	m_Port = config.GetValue<int>("Port");

	this->Run();
}

void Terminal::SendPacket(unsigned char* sendMsg, TCP_SOCKETINFO* pSocket, int packetLength)
{
	if (m_Status == TERMINAL_READY)
	{
		m_Network->Send(sendMsg, pSocket, packetLength);
	}
}

const char* Terminal::GetIp()
{
	return m_Ip;
}

void Terminal::ConnectProcess()
{
	//�͹̳� �������� �˷��ִ� ��Ŷ
	//
}

void Terminal::Run()
{
	THREAD_MANAGER.CreateThread<void(void)>(std::bind(&Terminal::ConnectProcess, this), "TerminalThread");
}
