#include "ETextureDataProcessor.h"
#include "IRDevice.h"
#include "EEngine.h"



ETextureDataProcessor::ETextureDataProcessor( char* name )
	: m_pResource(NULL)
	, m_Name(name)
{
}

ETextureDataProcessor::~ETextureDataProcessor()
{

}

bool ETextureDataProcessor::PopData()
{
	g_Engine.AssetMgr()->Insert(m_pResource);
	return true;
}


bool ETextureDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	assert( ELoader::IsDataProcThread() );

	m_pResource = g_Engine.EngineMemoryMgr()->GetNewResource(RESOURCE_TEXTURE);
	m_pResource->RID = GET_HASH_KEY(m_Name);	

//	m_pRDevice->PT_CreateGraphicBuffer( m_pResource);
	
	return true;
}

CResourceBase* ETextureDataProcessor::Process( void* pData, SIZE_T cBytes )
{
	return NULL;
}
