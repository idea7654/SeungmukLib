#pragma once
#include "pch.h"
#include "Thread.h"

class ThreadManager
{
private:
	std::vector<Thread*>	m_ThreadList;

public:
	template <typename T, typename... Types>
	Thread* CreateThread(std::function<T> function, const char* threadName, Types&&... args)
	{
		Thread* thread = new Thread();
		thread->CreateThread<T>(function, threadName, args...);
		m_ThreadList.push_back(thread);
		
		return thread;
	}

	int						GetThreadNum();
	std::vector<Thread*>	GetThreadList();
	bool					EmptyThreadList();

	void					JoinThread(std::thread::id threadId);
	void					JoinThread(const char* threadName);

	void					DetachThread(std::thread::id threadId);
	void					DetachThread(const char* threadName);
};

static ThreadManager THREAD_MANAGER;

