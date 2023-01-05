#include "pch.h"
#include "Lock.h"

Lock::Lock()
{
	InitializeSRWLock(&m_Mutex);
}

void Lock::EnterReadLock()
{
	AcquireSRWLockShared(&m_Mutex);
}

void Lock::ReadUnLock()
{
	ReleaseSRWLockShared(&m_Mutex);
}

void Lock::EnterWriteLock()
{
	AcquireSRWLockExclusive(&m_Mutex);
}

void Lock::WriteUnLock()
{

	ReleaseSRWLockExclusive(&m_Mutex);
}

