#include "CDefine.h"

#include "ILoader.h"

#include "EWinFileLoader.h"



EWinFileLoader::EWinFileLoader()
	: m_pData(NULL)
	, m_cBytes(0)
{

}

EWinFileLoader::~EWinFileLoader()
{
	SAFE_DELETE_ARRAY( m_pData );
	m_cBytes = 0;
}

void EWinFileLoader::SetFile( const char* szFileName )
{
    strcpy_s( m_szFileName, szFileName );
}

bool EWinFileLoader::GetData( void** ppData, SIZE_T* pcBytes )
{
	*ppData = m_pData;
	*pcBytes = m_cBytes;
	return true;
}


bool EWinFileLoader::IOT_Load()
{
	return Load();
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


	m_cBytes = GetFileSize(hFile, NULL );
	m_pData = new BYTE[m_cBytes];

	// read the header
	DWORD dwRead;
	if( !ReadFile( hFile, m_pData, m_cBytes, &dwRead, NULL ) )
	{
		assert(0);
		return false;
	}

	CloseHandle(hFile);

	return true;
}
