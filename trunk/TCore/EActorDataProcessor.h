#pragma once

#include "ILoader.h"


class EActorDataProcessor : public IDataProcessor
{

public:
	EActorDataProcessor();
	~EActorDataProcessor();

	virtual bool			PopData() override;
	virtual bool			PT_Process( void* pData, SIZE_T cBytes ) override;
};