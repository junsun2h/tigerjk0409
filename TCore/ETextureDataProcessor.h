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

	virtual bool			MT_Complete(IAssetMgr* pAssetMgr) override;
	virtual bool			MT_Destroy() override;
	virtual bool			PT_Process( void* pData, SIZE_T cBytes ) override;
};