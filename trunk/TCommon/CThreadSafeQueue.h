#pragma once

#include "CThread.h"


template<typename T>
class CThreadSafeQueue
{
public:
	CThreadSafeQueue(void)
	{
		m_Semaphore = CreateSemaphore(NULL,0,MAXLONG,NULL);
	}
	~CThreadSafeQueue(void)
	{
		CloseHandle(m_Semaphore);
	}

public:
	void PushBack(T item)
	{
		{
			CCriticalSectionSpinLock::Guard guard(m_Lock);
			m_List.push_back(item);
		}
		ReleaseSemaphore(m_Semaphore,1,NULL);
	}

	void PushFront(T item)
	{
		{
			CCriticalSectionSpinLock::Guard guard(m_Lock);
			m_List.push_front(item);
		}
		ReleaseSemaphore(m_Semaphore,1,NULL);
	}

	bool TryPopFront(T& item)
	{
		return PopFront(item,0);
	}

	bool PopFront(T& item,DWORD dwMilliseconds)
	{
		if( WAIT_OBJECT_0 == WaitForSingleObject(m_Semaphore, dwMilliseconds) )
		{
			CCriticalSectionSpinLock::Guard guard(m_Lock);
			LIST_TYPE::iterator itor = m_List.begin();
			item = *(itor);
			m_List.erase(itor);
			return true;
		}
		return false;
	}

	size_t Size(void)
	{	
		CCriticalSectionSpinLock::Guard guard(m_Lock);
		return m_List.size();
	}

	void Clear(void)
	{
		T item;
		while( true == TryPopFront(item) ) {}
	}
	
private:
	typedef	std::list<T>		LIST_TYPE;
	LIST_TYPE					m_List;
	CCriticalSectionSpinLock	m_Lock;
	HANDLE						m_Semaphore;
};