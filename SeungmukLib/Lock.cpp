#include "pch.h"
#include "Lock.h"

Lock::Lock()
{
	InitializeSRWLock(&m_Mutex);
}

void Lock::EnterLock(LOCK_TYPE lockType)
{
    switch (lockType)
    {
    case READ_LOCK:
        AcquireSRWLockExclusive(&m_Mutex);
        break;
    case WRITE_LOCK:
        AcquireSRWLockShared(&m_Mutex);
        break;
    }
}

void Lock::LeaveLock(LOCK_TYPE lockType)
{
    switch (lockType)
    {
    case READ_LOCK:
        ReleaseSRWLockExclusive(&m_Mutex);
        break;
    case WRITE_LOCK:
        ReleaseSRWLockShared(&m_Mutex);
        break;
    }
}

bool Lock::TryLock(LOCK_TYPE lockType)
{
    bool bRet = 0;
    switch (lockType)
    {
    case READ_LOCK:
        bRet = TryAcquireSRWLockExclusive(&m_Mutex);
        break;
    case WRITE_LOCK:
        bRet = TryAcquireSRWLockShared(&m_Mutex);
        break;
    }
    return bRet != 0 ? TRUE : FALSE;
}


