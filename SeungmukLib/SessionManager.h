#pragma once
#include "pch.h"

#define MAX_SESSION 5000

template <typename T>
class SessionManager
{
private:
	std::vector<T*>				m_SessionList;
	int							m_SessionCount;
	int							m_MaxConnection;
	UINT64						m_IdSeed;
	Lock						m_Lock;

public:
	SessionManager();
	~SessionManager();

	UINT64				CreateSessionId();
	bool				AddSession(T* session);
	std::vector<T*>		&GetSessionList();
	bool				CloseSession(T* session);
	void				ForceCloseSession(T* session);

	T*					GetSession(UINT64 id);
};