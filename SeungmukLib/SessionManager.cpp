#include "pch.h"
#include "SessionManager.h"

template <typename T>
SessionManager<T>::SessionManager()
{
	m_IdSeed = 1;
	m_MaxConnection = MAX_SESSION;
}

template <typename T>
SessionManager<T>::~SessionManager()
{
	std::vector<T*> removeSessionVec;
	removeSessionVec.resize(m_SessionList.size());
	std::copy(m_SessionList.begin(), m_SessionList.end(), removeSessionVec.begin());
	for (auto i : removeSessionVec)
	{
		cout << "Remove!!" << endl;
		i->Close();
	}
	m_SessionList.clear();
}

template <typename T>
std::vector<T*>& SessionManager<T>::GetSessionList()
{
	return m_SessionList;
}

template <typename T>
UINT64 SessionManager<T>::CreateSessionId()
{
	return m_IdSeed++;
}

template <typename T>
bool SessionManager<T>::AddSession(T* session)
{
	m_Lock.EnterLock(LOCK_TYPE::WRITE_LOCK);
	auto findSession = std::find(m_SessionList.begin(), m_SessionList.end(), session);
	m_Lock.LeaveLock(LOCK_TYPE::WRITE_LOCK);

	if (findSession != m_SessionList.end())
		return false;

	if (m_SessionCount > m_MaxConnection)
		return false;

	m_Lock.EnterLock(LOCK_TYPE::WRITE_LOCK);
	session->SetId(this->CreateSessionId());
	m_SessionList.push_back(session);
	++m_SessionCount;
	m_Lock.LeaveLock(LOCK_TYPE::WRITE_LOCK);
	
	return true;
}

template <typename T>
bool SessionManager<T>::CloseSession(T* session)
{
	if (session == nullptr)
		return false;

	m_Lock.EnterLock(LOCK_TYPE::WRITE_LOCK);

	auto findSession = std::find(m_SessionList.begin(), m_SessionList.end(), session);
	if (findSession != m_SessionList.end())
	{
		T* delSession = *findSession;
		g_Log.debug("detected close by client");
		delSession->Close();

		remove(m_SessionList.begin(), m_SessionList.end(), delSession);
		--m_SessionCount;
		delete delSession;
	}

	m_Lock.LeaveLock(LOCK_TYPE::WRITE_LOCK);
	return true;
}

template<typename T>
void SessionManager<T>::ForceCloseSession(T* session)
{
	if (session == nullptr)
		return;

	m_Lock.EnterLock(LOCK_TYPE::WRITE_LOCK);

	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 0;

	session->ForceClose(linger);
	this->CloseSession(session);
}

template <typename T>
T* SessionManager<T>::GetSession(UINT64 id)
{
	m_Lock.EnterLock(LOCK_TYPE::WRITE_LOCK);
	T* findSession = nullptr;

	for (auto i : m_SessionList)
	{
		if (i->GetId() == id)
		{
			findSession = i;
			break;
		}
	}
	m_Lock.LeaveLock(LOCK_TYPE::WRITE_LOCK);
	return findSession;
}

template class SessionManager<TCP_SOCKETINFO>;
template class SessionManager<UDP_SOCKETINFO>;