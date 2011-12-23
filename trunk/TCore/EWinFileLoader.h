#pragma once

#include "IAsyncLoader.h"




class EWinFileLoader : public IDataLoader
{
private:
	char				m_szFileName[MAX_PATH];
	BYTE*				m_pData;
	UINT				m_cBytes;
	HANDLE				m_hFile;

public:
	EWinFileLoader( char* szFileName );
	~EWinFileLoader();

	// overrides
public:
	virtual bool PT_Decompress( void** ppData, SIZE_T* pcBytes ) override;
	virtual bool IOT_Load() override;
};