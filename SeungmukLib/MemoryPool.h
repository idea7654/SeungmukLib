#pragma once
#include "pch.h"
#include <new>
#include <memory>

#ifdef _WIN64 || __amd64__

template <typename T>
class MemoryPool
{
public:
	static CircularQueue<T*> m_MemoryBlock;

public:
	~MemoryPool()
	{
		while (!m_MemoryBlock.IsEmpty())
		{
			auto object = m_MemoryBlock.Pop();
			delete object;
		}
	}

	static void* Alloc()
	{
		if (m_MemoryBlock.IsEmpty()) Initialize();

		T* returnPointer = m_MemoryBlock.Pop();
		return reinterpret_cast<void*>(returnPointer);
	}

	static void Free(void* deletePointer)
	{
		auto returnedPointer = reinterpret_cast<T*>(deletePointer);
		m_MemoryBlock.Push(returnedPointer);
	}

private:
	static void Initialize()
	{
		for (int i = 0; i < OBJECTPOOL_SIZE; i++)
		{
			T* newObject = new T();
			m_MemoryBlock.Push(newObject);
		}
	}
};

template <typename T>
CircularQueue<T*> MemoryPool<T>::m_MemoryBlock;

#else
struct Node
{
	Node* next;
};


template <typename T>
class MemoryPool
{
public:
	static unsigned char*	m_MemoryBlock;
	static Node*			m_Head;

public:
	~MemoryPool(){}

	static void* Alloc()
	{
		if (m_MemoryBlock == nullptr)
		{
			Initialize();
		}

		auto returnPointer = m_Head;
		m_Head = returnPointer->next;

		return reinterpret_cast<void*>(returnPointer);
	}
	static void	Free(void* deletePointer)
	{
		Node* returnedPointer = reinterpret_cast<Node*>(deletePointer);
		auto oldHead = m_Head;
		m_Head = returnedPointer;
		m_Head->next = oldHead;
	}

private:
	static void	Initialize()
	{
		m_MemoryBlock = new unsigned char[sizeof(T) * OBJECTPOOL_SIZE];
		m_Head = reinterpret_cast<Node*>(m_MemoryBlock);

		auto originHead = m_Head;

		printf("%d", sizeof(Node*));
		for (int i = 0; i < OBJECTPOOL_SIZE - 1; i++)
		{
			m_Head->next = reinterpret_cast<Node*>(reinterpret_cast<char*>(m_Head) + sizeof(T));
			m_Head = m_Head->next;
		}

		m_Head = originHead;
	}
	static void		ExtendMemoryBlock() {};
};

template <typename T>
unsigned char* MemoryPool<T>::m_MemoryBlock;

template <typename T>
Node* MemoryPool<T>::m_Head;
#endif

template <typename T, typename... Args>
T* Attach(Args&&... args)
{
	T* myPtr = static_cast<T*>(MemoryPool<T>::Alloc());
	new(myPtr) T(std::forward<Args>(args)...);
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
	return std::shared_ptr<T>{ Attach<T>(std::forward<Args>(args)...), Detatch<T> };
}