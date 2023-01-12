#pragma once
#include "pch.h"

class Lock
{
private:
	SRWLOCK m_Mutex;

public:
	Lock();

	void EnterLock(LOCK_TYPE lockType);
	void LeaveLock(LOCK_TYPE lockType);
	bool TryLock(LOCK_TYPE lockType);
};

