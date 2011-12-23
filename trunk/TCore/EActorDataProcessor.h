#pragma once

#include "IAsyncLoader.h"


class EActorDataProcessor : public IDataProcessor
{

public:
	EActorDataProcessor();
	~EActorDataProcessor();

	virtual bool			PopData(IAssetMgr* pAssetMgr) override;
	virtual bool			PT_Process( void* pData, SIZE_T cBytes ) override;
};