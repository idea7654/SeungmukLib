#pragma once
#include "pch.h"

class Lock
{
private:
	SRWLOCK m_Mutex;

public:
	Lock();

	void EnterReadLock();
	void ReadUnLock();
	void EnterWriteLock();
	void WriteUnLock();
};

