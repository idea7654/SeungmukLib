#include "pch.h"
#include "Thread.h"

std::thread::id Thread::GetThreadID()
{
    return m_Id;
}

std::wstring Thread::GetThreadName()
{
    return m_Name;
}

void Thread::Join()
{
    m_Thread->join();
}

void Thread::Detach()
{
    m_Thread->detach();
}

void Thread::SetThreadName(const char* threadName)
{
    m_Name = std::wstring(threadName, threadName + strlen(threadName)); //char* to wstring
}
