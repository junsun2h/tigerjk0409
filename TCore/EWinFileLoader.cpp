#include "EGlobal.h"
#include "EWinFileLoader.h"



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
	assert ( GLOBAL::Loader()->IsIOThread() );

	Load();

	return true;
}

bool EWinFileLoader::Load()
{
	HANDLE hFile = CreateFileA( m_szFileName, 
		GENERIC_READ, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL,	
		NULL );

	if( INVALID_HANDLE_VALUE == hFile )
	{
		assert(0);
		return false;
	}


	DWORD size = GetFileSize(hFile, NULL );
	m_pData = new BYTE[size];

	// read the header
	DWORD dwRead;
	if( !ReadFile( hFile, m_pData, size, &dwRead, NULL ) )
	{
		assert(0);
		return false;
	}

	CloseHandle(hFile);

	return true;
}
