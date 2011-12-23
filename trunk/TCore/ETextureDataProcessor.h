#pragma once

#include "IAsyncLoader.h"


struct IRDevice;


class ETextureDataProcessor : public IDataProcessor
{
	IRDevice*	m_pRDevice;
	IResource*	m_pResource;
	char*		m_Name;

public:
	ETextureDataProcessor( IRDevice* pRDevice, char* name );
	~ETextureDataProcessor();

	virtual bool			PopData(IAssetMgr* pAssetMgr) override;
	virtual bool			PT_Process( void* pData, SIZE_T cBytes ) override;
};