#include "pch.h"
#include "CircularQueue.h"

template<typename T>
CircularQueue<T>::CircularQueue()
{
	m_Front = 0;
	m_Tail = 0;
}

template <typename T>
bool CircularQueue<T>::Push(T data)
{
	m_Lock.EnterWriteLock();
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
	m_Lock.WriteUnLock();
}

template<typename T>
T& CircularQueue<T>::Pop()
{
	m_Lock.EnterWriteLock();
	m_Front++;
	m_Front = m_Front % MAX_QUEUE_LENGTH;
	return m_Queue[m_Front % MAX_QUEUE_LENGTH];
	m_Lock.WriteUnLock();
}

template<typename T>
bool CircularQueue<T>::IsEmpty()
{
	m_Lock.EnterReadLock();
	return m_Front == m_Tail;
	m_Lock.ReadUnLock();
}

//template class CircularQueue<Packet>;
template class CircularQueue<QUEUE_DATA*>;
template class CircularQueue<QUEUE_DATA>;