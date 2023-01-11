#pragma once
#include "pch.h"
#include <new>
#include <memory>

template <typename T>
class MemoryPool
{
public:
	static BYTE*	m_MemoryBlock;

public:
	~MemoryPool(){}

	static void* Alloc(size_t allocSize)
	{
		if (m_MemoryBlock == nullptr)
		{
			Initialize();
		}

		BYTE* ReturnPointer = m_MemoryBlock;
		m_MemoryBlock = *reinterpret_cast<BYTE**>(ReturnPointer);
		return reinterpret_cast<void*>(ReturnPointer);
	}
	static void	Free(void* deletePointer)
	{
		*reinterpret_cast<BYTE**>(deletePointer) = m_MemoryBlock;
		m_MemoryBlock = static_cast<BYTE*>(deletePointer);
	}

private:
	static void	Initialize()
	{
		m_MemoryBlock = new BYTE[sizeof(T) * OBJECTPOOL_SIZE];
		BYTE** Current = reinterpret_cast<BYTE**>(m_MemoryBlock);
		BYTE* Next = m_MemoryBlock;
		for (int i = 0; i < OBJECTPOOL_SIZE - 1; ++i)
		{
			Next += sizeof(T);
			*Current = Next;
			Current = reinterpret_cast<BYTE**>(Next);
		}
		*Current = nullptr;
	}
	static void		ExtendMemoryBlock() {};
};

template <typename T>
BYTE* MemoryPool<T>::m_MemoryBlock;

template <typename T, typename... Args>
T* Attach(Args&&... args)
{
	T* myPtr = static_cast<T*>(MemoryPool<T>::Alloc(sizeof(T)));
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

//MemoryPool 고치고...네트워크 부분 완성하자!