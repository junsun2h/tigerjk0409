#pragma once

#include "IAsyncLoader.h"
#include <vector>

struct IRDevice;


class EMeshDataProcessor : public IDataProcessor
{
	IRDevice*				m_pRDevice;
	std::string				m_name;
	std::vector<IResource*>	m_pResources;
	
	long					PT_ReadMesh(BYTE** ppSrcBits, std::string name);

public:
	EMeshDataProcessor( IRDevice* pRDevice, std::string name );
	~EMeshDataProcessor();

	virtual bool			MT_Complete(IAssetMgr* pAssetMgr) override;
	virtual bool			MT_Destroy() override;
	virtual bool			PT_Process( void* pData, SIZE_T cBytes ) override;
};