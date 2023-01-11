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
	CircularQueue();
	bool	Push(T data);
	T&		Pop();
	bool	IsEmpty();
};
