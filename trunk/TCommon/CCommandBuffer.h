#pragma once



template<class cammandType>
class CCommandBuffer
{
public:
	CCommandBuffer()
		: m_pData(NULL)
		, m_pSafty(NULL)
		, m_CommandCount(0)
	{
		m_pData = (byte*)realloc( m_pData,  1024 * 1000 );	// 1mb
		m_pTop = m_pData;
		m_pBottom = m_pData;
	}

	~CCommandBuffer()
	{
		free(m_pData);
	}

	void	AddCommandStart(cammandType cmd)
	{
		if( m_pSafty != 0)
		{
			assert(0);
			return;
		}

		// push command
		m_pSafty = UINT(m_pTop);
		*(UINT*)m_pTop = cmd;
		m_pTop += 4;
	}

	void	AddCommandEnd()
	{
		if( m_pSafty == 0)
		{
			assert(0);
			return;
		}

		memcpy( m_pTop, &m_pSafty, 4);
		m_pTop += 4;
		m_CommandCount++;

		m_pSafty = 0;
	}

	template<class T>
	void	AddParam(T* pData)
	{
		if( m_pSafty == 0)
		{
			assert(0);
			return;
		}

		// push data 
		memcpy( m_pTop, pData, sizeof(T));
		m_pTop += sizeof(T);
	}

	template<class T>	
	void	AddParam(T pData)
	{
		if( m_pSafty == 0)
		{
			assert(0);
			return;
		}

		// push data 
		memcpy( m_pTop, &pData, sizeof(T));
		m_pTop += sizeof(T);
	}

	void	AddData(void* pData, size_t size )
	{
		if( m_pSafty == 0)
		{
			assert(0);
			return;
		}

		// push size
		*(UINT*)m_pTop = size;
		m_pTop += 4;

		// push data 
		memcpy( m_pTop, pData, size);
		m_pTop += size;
	}

	void	AddAddress(void* pData)
	{
		if( m_pSafty == 0)
		{
			assert(0);
			return;
		}

		UINT data = UINT(pData);
		memcpy( m_pTop, &data, 4);
		m_pTop += 4;
	}

	cammandType	PopCommandStart()
	{
		if( m_pSafty != 0)
			assert(0);

		// pop command
		m_pSafty = UINT(m_pBottom);
		cammandType cmd = (cammandType)*m_pBottom;
		m_pBottom += 4;

		return cmd;
	}

	void	PopCommandEnd()
	{
		if( m_pSafty == 0)
		{
			assert(0);
			return;
		}

		UINT commandStartPoint;
		memcpy( &commandStartPoint, m_pBottom, 4);
		if( commandStartPoint != m_pSafty )
			assert(0);

		m_pBottom += 4;
		m_CommandCount--;
		m_pSafty = 0;

		if( m_pTop == m_pBottom )
		{
			m_pBottom = m_pData;
			m_pTop = m_pData;
		}
	}

	template<class T>
	void	PopParam(T*& pData)
	{
		if( m_pSafty == 0)
		{
			assert(0);
			return;
		}

		// pop data
		pData = (T*)m_pBottom;
		m_pBottom += sizeof(T);
	}

	template<class T>
	void PopAddress(T*& pOutData)
	{
		if( m_pSafty == 0)
		{
			assert(0);
			return;
		}

		UINT* pData = (UINT*)m_pBottom;
		m_pBottom += 4;

		pOutData = (T*)*pData;
	}

	template<class T>
	void	PopData(T*& pData)
	{
		if( m_pSafty == 0)
		{
			assert(0);
			return;
		}

		// pop size
		size_t size = (size_t)*m_pBottom;
		m_pBottom += 4;

		// pop data
		pData = (T*)m_pBottom;
		m_pBottom += size;
	}

	bool	IsEmpty()
	{
		return m_pTop == m_pBottom;
	}

private:
	byte*	m_pData;
	byte*	m_pTop;
	byte*	m_pBottom;

	UINT	m_pSafty;

	UINT	m_CommandCount;
};
