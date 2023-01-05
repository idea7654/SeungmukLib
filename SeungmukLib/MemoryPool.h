#pragma once
#include "pch.h"
#include <new>
#include <memory>

class Node
{
	Node* m_Next;
};

template <typename T>
class MemoryPool
{
private:
	static Node*	m_Head;
	static char*	m_MemoryBlock;

public:
	static void* Alloc(size_t allocSize)
	{
		if (m_MemoryBlock == nullptr)
		{
			Initialize();
		}

		auto returnPointer = m_Head;
		m_Head = m_Head->m_Next;
		return reinterpret_cast<void*>(returnPointer);
	}
	static void	Free(void* deletePointer)
	{
		auto returnMemory = reinterpret_cast<Node*>(deletePointer);
		auto oldHead = m_Head;
		m_Head = returnMemory;
		m_Head->m_Next = oldHead;
	}

private:
	static void	Initialize()
	{
		m_MemoryBlock = (char*)malloc(sizeof(T) * OBJECTPOOL_SIZE);
		m_Head = reinterpret_cast<Node*>(m_MemoryBlock);

		for (int i = 0; i < OBJECTPOOL_SIZE - 1; i++) {
			auto thisNode = reinterpret_cast<Node*>(m_MemoryBlock);
			thisNode->next = reinterpret_cast<Node*>(m_MemoryBlock + sizeof(T));
		}
	}
	static void		ExtendMemoryBlock();
};

template <typename T>
Node* MemoryPool<T>::m_Head;

template <typename T, typename... Args>
T* Attach(Args&&... args)
{
	T* myPtr = static_cast<T*>(MemoryPool<T>::Alloc(sizeof(T)));
	new(myPtr) T(std::forward<Args...>(args...));
	return myPtr;
}

template <typename T>
void Detatch(T* obj)
{
	obj->~T();
	MemoryPool<T>::Free(obj);
}

template <typename T, typename... Args>
std::shared_ptr<T> MakeShared(Args&&... args)
{
	return std::shared_ptr<T>{ Attach<T>(std::forward<Args...>(args...)), Detatch<T> };
}
