#include "ETextureDataProcessor.h"
#include "IRDevice.h"
#include "EAsyncLoader.h"


ETextureDataProcessor::ETextureDataProcessor( char* name )
	: m_pResource(NULL)
	, m_Name(name)
{
}

ETextureDataProcessor::~ETextureDataProcessor()
{

}

bool ETextureDataProcessor::PopData(IAssetMgr* pAssetMgr)
{
	pAssetMgr->LoadComplete(m_pResource);
	return true;
}


bool ETextureDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	assert ( EAsyncLoader::IsDataProcThread() );

	m_pResource = new CResourceTexture;
	m_pResource->RID = GET_HASH_KEY(m_Name);	

//	m_pRDevice->PT_CreateGraphicBuffer( m_pResource);
	
	return true;
}