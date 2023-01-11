#include "pch.h"
#include "ThreadManager.h"

//template <typename T>
//Thread* ThreadManager::CreateThread(std::function<T> function, const char* threadName)
//{
//	Thread *thread = new Thread();
//	thread->CreateThread(function, threadName);
//	m_ThreadList.push_back(thread);
//
//	return thread;
//}

int ThreadManager::GetThreadNum()
{
	return m_ThreadList.size();
}

std::vector<Thread*> ThreadManager::GetThreadList()
{
	return m_ThreadList;
}

bool ThreadManager::EmptyThreadList()
{
	return m_ThreadList.size() == 0;
}

void ThreadManager::JoinThread(std::thread::id threadId)
{
	auto it = find_if(m_ThreadList.begin(), m_ThreadList.end(), [&](Thread *element) {
		return element->GetThreadID() == threadId;
		});

	(*it)->Join();
}

void ThreadManager::JoinThread(const char* threadName)
{
	auto it = find_if(m_ThreadList.begin(), m_ThreadList.end(), [&](Thread *element) {
		return element->GetThreadName() == std::wstring(threadName, threadName + strlen(threadName));
		});

	(*it)->Join();
}

void ThreadManager::DetachThread(std::thread::id threadId)
{
	auto it = find_if(m_ThreadList.begin(), m_ThreadList.end(), [&](Thread *element) {
		return element->GetThreadID() == threadId;
		});

	(*it)->Detach();
}

void ThreadManager::DetachThread(const char* threadName)
{
	auto it = find_if(m_ThreadList.begin(), m_ThreadList.end(), [&](Thread *element) {
		return element->GetThreadName() == std::wstring(threadName, threadName + strlen(threadName));
		});

	(*it)->Detach();
}