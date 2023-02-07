#include "pch.h"
#include "Session.h"

Session::Session()
{
	this->UpdateHeartBeat();
}

Session::~Session()
{
	this->Close();
}

time_t Session::GetHeartBeat()
{
	return m_HeartBeat;
}

void Session::UpdateHeartBeat()
{
	m_HeartBeat = chrono::system_clock::to_time_t(chrono::system_clock::now());
}

UINT64 Session::GetId()
{
	return m_Id;
}

void Session::SetId(UINT64 id)
{
	m_Id = id;
}
