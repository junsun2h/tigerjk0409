#include "EWinFileLoader.h"
#include "ELoader.h"


EWinFileLoader::EWinFileLoader( char* szFileName  )
	: m_pData(NULL)
	, m_cBytes(0)
{
    strcpy_s( m_szFileName, szFileName );
}

EWinFileLoader::~EWinFileLoader()
{
	SAFE_DELETE_ARRAY( m_pData );
	m_cBytes = 0;
}

bool EWinFileLoader::GetData( void** ppData, SIZE_T* pcBytes )
{
	*ppData = m_pData;
	*pcBytes = m_cBytes;
	return true;
}


bool EWinFileLoader::IOT_Load()
{
	assert ( ELoader::IsIOThread() );

    // Open the file
	m_hFile = CreateFileA( m_szFileName, 
							GENERIC_READ, 
							FILE_SHARE_READ, 
							NULL, 
							OPEN_EXISTING, 
							FILE_ATTRIBUTE_NORMAL,	
							NULL );

	if( INVALID_HANDLE_VALUE == m_hFile )
	{
		assert(0);
		return false;
	}

	DWORD size = GetFileSize(m_hFile, NULL );
	m_pData = new BYTE[size];

	// read the header
	DWORD dwRead;
	if( !ReadFile( m_hFile, m_pData, size, &dwRead, NULL ) )
	{
		assert(0);
		return false;
	}

	return true;
}

bool EWinFileLoader::Load()
{
	m_hFile = CreateFileA( m_szFileName, 
		GENERIC_READ, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL,	
		NULL );

	if( INVALID_HANDLE_VALUE == m_hFile )
	{
		assert(0);
		return false;
	}


	DWORD size = GetFileSize(m_hFile, NULL );
	m_pData = new BYTE[size];

	// read the header
	DWORD dwRead;
	if( !ReadFile( m_hFile, m_pData, size, &dwRead, NULL ) )
	{
		assert(0);
		return false;
	}


	return true;
}
