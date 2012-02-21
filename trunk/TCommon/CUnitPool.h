#pragma once

#include "CDefine.h"

/*
Uses when [T] has virtual functions.
CUnitPoolArray doesn't change its size
[T] should have [ void Destroy() ] as function
*/

typedef std::list<unsigned int>		OBJ_HANDLE_LIST;

template<class T>
class CObjectPool
{
public:
	CObjectPool(UINT reserveCount = 0)
		: m_nUsingCount(0)
		, m_nReservedCount(0)
		, m_pData(NULL)
	{
		if( reserveCount> 0)
			Reserve(reserveCount);
	}

	~CObjectPool()
	{
		if( m_nUsingCount > 0 )
			assert(0);

		free(m_pData);	
	}

	T*					GetNew()
	{
		if( m_nUsingCount >= m_nReservedCount)
			Reserve(m_nReservedCount*2);

		int nUnusedIndex = m_UnusingList.front();
		m_UnusingList.pop_front();

		m_UsingList.push_back(nUnusedIndex);

		assert(!IsUsed(nUnusedIndex));
		m_vecInfo[nUnusedIndex].m_bUsed = true;
		m_vecInfo[nUnusedIndex].m_position = --m_UsingList.end();
		m_nUsingCount++;

		// call constructor
		new (&m_pData[nUnusedIndex]) T;

		assert(IsUsed(nUnusedIndex));

		return &m_pData[nUnusedIndex];
	}

	T*					Get(UINT handle)			{ return &m_pData[handle]; }
	UINT				GetFirst()					{ return m_UsingList.front(); }
	bool				IsUsed(UINT nIndex) const	{ return m_vecInfo[nIndex].m_bUsed; };
	UINT				GetUsingCount() const		{ return m_nUsingCount; }
	UINT				GetCapacity() const			{ return m_nReservedCount; }
	void				Remove(void* pItem)			{ Remove( GetHandle(pItem) );	}
	bool				Reserve(UINT newCapacity)
	{
		assert( newCapacity >= m_nReservedCount );

		void* pNewArray = realloc(m_pData, newCapacity * sizeof(T));
		if( pNewArray == NULL )
		{
			assert(0);
			return false;
		}

		m_pData = (T*)pNewArray;
		for(UINT i= m_nReservedCount; i<newCapacity ;i++)
		{
			m_UnusingList.push_back(i);
			CHandleInfo info;
			info.m_bUsed = false;
			m_vecInfo.push_back(info);
		}

		m_nReservedCount = newCapacity;
		return true;
	}

	const OBJ_HANDLE_LIST*	UsingHandleList()		{ return &m_UsingList; }

private:
	UINT				GetHandle(void* pItem)
	{
		int address = int(pItem) - int(m_pData);
		UINT handle = address / sizeof(T);

		if( handle > m_nReservedCount )
			assert(0);

		return handle;
	}

	void				Remove(UINT nHandle)
	{
		assert(IsUsed(nHandle));

		m_vecInfo[nHandle].m_bUsed = false;
		m_UsingList.erase( m_vecInfo[nHandle].m_position );

		m_nUsingCount--;
		m_UnusingList.push_front(nHandle);

		// call destructor
		m_pData[nHandle].~T();
	}

	struct CHandleInfo
	{
		bool						m_bUsed;
		std::list<UINT>::iterator	m_position;
	};

protected:
	std::vector<CHandleInfo>	m_vecInfo;
	T*							m_pData;		
	unsigned int				m_nReservedCount;	
	unsigned int				m_nUsingCount;

	std::list<unsigned int>		m_UnusingList;
	std::list<unsigned int>		m_UsingList;
};