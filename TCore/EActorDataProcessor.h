#pragma once

#include "IAsyncLoader.h"


class EActorDataProcessor : public IDataProcessor
{

public:
	EActorDataProcessor();
	~EActorDataProcessor();

	virtual bool			MT_Complete(IAssetMgr* pAssetMgr) override;
	virtual bool			MT_Destroy() override;
	virtual bool			PT_Process( void* pData, SIZE_T cBytes ) override;
};