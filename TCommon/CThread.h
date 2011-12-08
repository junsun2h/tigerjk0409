#pragma once

#include <Windows.h>
#include <assert.h>


class CCriticalSectionSpinLock
{
private:
	CRITICAL_SECTION	m_cs;

public:
#ifdef _DEBUG
	CCriticalSectionSpinLock() : m_nCount(0)
	{
		InitializeCriticalSectionAndSpinCount(&m_cs,4000);
	}
	~CCriticalSectionSpinLock() {	DeleteCriticalSection(&m_cs);	}
#else
	CCriticalSectionSpinLock()	{	InitializeCriticalSectionAndSpinCount(&m_cs,4000);	}
	~CCriticalSectionSpinLock() {	DeleteCriticalSection(&m_cs); }
#endif

	class Guard
	{
	private:
		CCriticalSectionSpinLock& m_csSyncObject;

		// block copy
		Guard(const Guard& rhs);
		Guard &operator=(const Guard& rhs);

	public:
		explicit Guard(CCriticalSectionSpinLock & cs) : m_csSyncObject(cs)
		{
			m_csSyncObject.Lock();
#ifdef _DEBUG
			++ m_csSyncObject.m_nCount;
			assert( m_csSyncObject.m_nCount <= 1);
#endif

		}
		~Guard()
		{
#ifdef _DEBUG
			-- m_csSyncObject.m_nCount;
			assert( m_csSyncObject.m_nCount == 0 );
#endif
			m_csSyncObject.Unlock();
		}
	};

	void Lock()			{ EnterCriticalSection(&m_cs); }
	void Unlock()		{ LeaveCriticalSection(&m_cs); }

#ifdef _DEBUG
	unsigned int  m_nCount;
#endif
};