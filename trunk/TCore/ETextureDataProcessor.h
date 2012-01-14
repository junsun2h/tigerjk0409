#pragma once

#include "IAsyncLoader.h"



class ETextureDataProcessor : public IDataProcessor
{
	CResourceBase*	m_pResource;
	char*		m_Name;

public:
	ETextureDataProcessor( char* name );
	~ETextureDataProcessor();

	virtual bool			PopData(IAssetMgr* pAssetMgr) override;
	virtual bool			PT_Process( void* pData, SIZE_T cBytes ) override;
};