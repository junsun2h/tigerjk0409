#pragma once


//--------------------------------------------------------------------------------------
// Simple helper stack class
//--------------------------------------------------------------------------------------
template <class T> class CStack
{
private:
	UINT m_MemorySize;
	UINT m_NumElements;
	T* m_pData;

	bool    EnsureStackSize( UINT64 iElements )
	{
		if( m_MemorySize > iElements )
			return true;

		T* pTemp = new T[ ( size_t )( iElements * 2 + 256 ) ];
		if( !pTemp )
			return false;

		if( m_NumElements )
		{
			CopyMemory( pTemp, m_pData, ( size_t )( m_NumElements * sizeof( T ) ) );
		}

		if( m_pData ) delete []m_pData;
		m_pData = pTemp;
		return true;
	}

public:
	CStack()
	{
		m_pData = NULL; m_NumElements = 0; m_MemorySize = 0;
	}
	~CStack()
	{
		if( m_pData ) delete []m_pData;
	}

	UINT    GetCount()
	{
		return m_NumElements;
	}
	T       GetAt( UINT i )
	{
		return m_pData[i];
	}
	T       GetTop()
	{
		if( m_NumElements < 1 )
			return NULL;

		return m_pData[ m_NumElements - 1 ];
	}

	T       GetRelative( INT i )
	{
		INT64 iVal = m_NumElements - 1 + i;
		if( iVal < 0 )
			return NULL;
		return m_pData[ iVal ];
	}

	bool    Push( T pElem )
	{
		if( !EnsureStackSize( m_NumElements + 1 ) )
			return false;

		m_pData[m_NumElements] = pElem;
		m_NumElements++;

		return true;
	}

	T       Pop()
	{
		if( m_NumElements < 1 )
			return NULL;

		m_NumElements --;
		return m_pData[m_NumElements];
	}
};
