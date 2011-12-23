#include "EWinFileLoader.h"
#include "EAsyncLoader.h"


EWinFileLoader::EWinFileLoader( char* szFileName  )
{
    strcpy_s( m_szFileName, szFileName );
}

EWinFileLoader::~EWinFileLoader()
{
	SAFE_DELETE_ARRAY( m_pData );
	m_cBytes = 0;
}

bool EWinFileLoader::PT_Decompress( void** ppData, SIZE_T* pcBytes )
{
	*ppData = m_pData;
	*pcBytes = m_cBytes;
	return false;
}


bool EWinFileLoader::IOT_Load()
{
	assert ( EAsyncLoader::IsIOThread() );

    // Open the file
	m_hFile = CreateFileA( m_szFileName, 
							GENERIC_READ, 
							FILE_SHARE_READ, 
							NULL, 
							OPEN_EXISTING, 
							FILE_ATTRIBUTE_NORMAL,	
							NULL );

	if( INVALID_HANDLE_VALUE == m_hFile )
		return false;

	DWORD size = GetFileSize(m_hFile, NULL );
	m_pData = new BYTE[size];

	// read the header
	DWORD dwRead;
	if( !ReadFile( m_hFile, m_pData, size, &dwRead, NULL ) )
		return false;

	return true;
}