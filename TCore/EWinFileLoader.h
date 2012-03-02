#pragma once


class EWinFileLoader
{
private:
	char				m_szFileName[MAX_PATH];
	BYTE*				m_pData;
	DWORD				m_cBytes;

public:
	EWinFileLoader();
	~EWinFileLoader();

	// overrides
public:
	void		SetFile( const char* szFileName );
	bool		GetData( void** ppData, SIZE_T* pcBytes );
	bool		IOT_Load();
	bool		Load();
};