#pragma once
#include "pch.h"

template <typename T>
class CircularQueue
{
private:
	int		m_Front;
	int		m_Tail;
	Lock	m_Lock;

	T		m_Queue[MAX_QUEUE_LENGTH];

public:
	CircularQueue()
	{
		m_Front = 0;
		m_Tail = 0;
	}

	bool	Push(T data)
	{
		m_Lock.EnterLock(WRITE_LOCK);
		if (m_Front == (m_Tail + 1) % MAX_QUEUE_LENGTH)
		{
			//Queue Full
			return false;
		}
		else
		{
			m_Tail++;
			m_Tail = m_Tail % MAX_QUEUE_LENGTH;
			m_Queue[m_Tail] = data;
			return true;
		}
		m_Lock.LeaveLock(WRITE_LOCK);
	}

	T& Pop()
	{
		m_Lock.EnterLock(WRITE_LOCK);
		m_Front++;
		m_Front = m_Front % MAX_QUEUE_LENGTH;
		m_Lock.LeaveLock(WRITE_LOCK);
		return m_Queue[m_Front % MAX_QUEUE_LENGTH];
	}

	bool	IsEmpty()
	{
		return m_Front == m_Tail;
	}
};
