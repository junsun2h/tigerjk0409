#pragma once

#include "ILoader.h"


class EActorDataProcessor : public IDataProcessor
{
	std::string				m_name;
public:
	EActorDataProcessor(std::string name );
	~EActorDataProcessor();

	virtual bool			PopData() override;
	virtual bool			PT_Process( void* pData, SIZE_T cBytes ) override;
	virtual CResourceBase*	Process( void* pData, SIZE_T cBytes ) override;
};