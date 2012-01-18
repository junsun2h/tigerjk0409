#pragma once

#include "ILoader.h"



class ETextureDataProcessor : public IDataProcessor
{
	CResourceBase*	m_pResource;
	char*		m_Name;

public:
	ETextureDataProcessor( char* name );
	~ETextureDataProcessor();

	virtual bool	PopData() override;
	virtual bool	PT_Process( void* pData, SIZE_T cBytes ) override;
	virtual bool	Process( void* pData, SIZE_T cBytes ) override;
};