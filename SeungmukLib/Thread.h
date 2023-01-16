#pragma once
#include "pch.h"

class Thread
{
private:
	std::thread::id	m_Id;
	std::thread*	m_Thread;
	std::wstring	m_Name;
	Lock			m_Lock;

public:
	std::thread::id	GetThreadID();
	std::wstring	GetThreadName();

	template <typename T, typename... Types>
	void			CreateThread(std::function<T> function, const char* threadName, Types&&... params)
	{
		m_Thread = new thread(std::bind(function, params...));
		m_Id = m_Thread->get_id();
		SetThreadName(threadName);
	}
	void			Join();
	void			Detach();

private:
	void			SetThreadName(const char* threadName);
};