#pragma once

#include "CDefine.h"

/*
Uses when [T] has virtual functions.
CUnitPoolArray doesn't change its size
[T] should have [ void Destroy() ] as function
*/

template<class T>
class CObjectPool
{
public:
	CObjectPool()
		: m_nUsingCount(0)
		, m_nReservedCount(0)
		, m_pData(NULL)
		, m_bInitialized(false)
	{
	}


	CObjectPool(UINT reserveCount)
		: m_nUsingCount(0)
		, m_nReservedCount(reserveCount)
		, m_pData(NULL)
		, m_bInitialized(false)
	{
		Init(reserveCount);
	}

	~CObjectPool()
	{
		if( m_nUsingCount > 0 )
			assert(0);

		SAFE_DELETE_ARRAY(m_pData);	
	}

	void Init(UINT reserveCount)
	{
		if( m_bInitialized == true)
		{
			assert(0);
			return;
		}

		m_pData = new T[reserveCount];

		for(UINT i= 0; i<reserveCount ;i++)
		{
			m_UnusingList.push_back(i);
			CHandleInfo info;
			info.m_bUsed = false;
			m_vecInfo.push_back(info);
		}
		m_nReservedCount = reserveCount;

		m_bInitialized = true;
	}
	
	T* GetNew()
	{
		if( m_nUsingCount >= m_nReservedCount)
		{
			assert(0);
			return NULL;
		}

		int nUnusedIndex = m_UnusingList.front();
		m_UnusingList.pop_front();

		m_UsingList.push_back(nUnusedIndex);

		assert(!IsUsed(nUnusedIndex));
		m_vecInfo[nUnusedIndex].m_bUsed = true;
		m_vecInfo[nUnusedIndex].m_position = --m_UsingList.end();
		m_nUsingCount++;

		m_pData[nUnusedIndex];

		assert(IsUsed(nUnusedIndex));

		return &m_pData[nUnusedIndex];
	}

	UINT				GetFirst()					{ return m_UsingList.front(); }
	bool				IsUsed(UINT nIndex) const	{ return m_vecInfo[nIndex].m_bUsed; };
	UINT				GetUsingCount() const		{ return m_nUsingCount; }
	UINT				GetCapacity() const			{ return m_nReservedCount; }
	void				Remove(void* pItem)			{ Remove( GetHandle(pItem) );	}

private:
	UINT GetHandle(void* pItem)
	{
		int address = UINT(pItem) - UINT(m_pData);
		int handle = address / sizeof(T);
		return handle;
	}

	void Remove(UINT nHandle)
	{
		assert(IsUsed(nHandle));

		m_vecInfo[nHandle].m_bUsed = false;
		m_UsingList.erase( m_vecInfo[nHandle].m_position );

		m_nUsingCount--;
		m_UnusingList.push_front(nHandle);

		m_pData[nHandle].Destroy();
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

	bool						m_bInitialized;
};




//---------------------------------------------------------------------------------------------------------
template<class T>
class CUnitPool
{
public:
	CUnitPool(UINT reserveCount)
		: m_nUsingCount(0)
		, m_nReservedCount(0)
		, m_pData(NULL)
	{
		Reserve(reserveCount);
	}

	~CUnitPool()
	{
		assert( m_nUsingCount==0 );

		while( GetUsingCount() )
			Remove(GetFirst());

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

		m_pData[nUnusedIndex];

		assert(IsUsed(nUnusedIndex));

		return &m_pData[nUnusedIndex];
	}

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



private:
	UINT				GetHandle(void* pItem)
	{
		int address = UINT(pItem) - UINT(m_pData);
		int handle = address / sizeof(T);
		return handle;
	}

	void				Remove(UINT nHandle)
	{
		assert(IsUsed(nHandle));

		m_vecInfo[nHandle].m_bUsed = false;
		m_UsingList.erase( m_vecInfo[nHandle].m_position );

		m_nUsingCount--;
		m_UnusingList.push_front(nHandle);

		m_pData[nHandle].Destroy();
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
