#include "ETextureDataProcessor.h"
#include "IRDevice.h"
#include "EAsyncLoader.h"


ETextureDataProcessor::ETextureDataProcessor( IRDevice* pRDevice, char* name )
	: m_pRDevice(pRDevice)
	, m_pResource(NULL)
	, m_Name(name)
{
}

ETextureDataProcessor::~ETextureDataProcessor()
{

}

bool ETextureDataProcessor::PopData(IAssetMgr* pAssetMgr)
{
	pAssetMgr->LoadCompletedResource(m_pResource);
	return true;
}


bool ETextureDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	assert ( EAsyncLoader::IsDataProcThread() );

	m_pResource = new CResourceTexture;
	m_pResource->RID = GET_RID(m_Name);	

	m_pRDevice->TS_CreateDPResource( DP_RESOURCE_TEXTURE, pData, cBytes, m_pResource);
	
	return true;
}