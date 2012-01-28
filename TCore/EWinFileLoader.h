#pragma once

#include "ILoader.h"




class EWinFileLoader : public IDataLoader
{
private:
	char				m_szFileName[MAX_PATH];
	BYTE*				m_pData;
	UINT				m_cBytes;

public:
	EWinFileLoader( char* szFileName );
	~EWinFileLoader();

	// overrides
public:
	virtual bool GetData( void** ppData, SIZE_T* pcBytes ) override;
	virtual bool IOT_Load() override;
	virtual bool Load() override;
};