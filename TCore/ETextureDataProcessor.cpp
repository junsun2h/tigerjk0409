#include "CResource.h"

#include "ILoader.h"
#include "IRDevice.h"

#include "EGlobal.h"
#include "ETextureDataProcessor.h"



void ETextureDataProcessor::Init(CResourceBase* pRsc, bool bForward)
{
	m_pTexture = (CResourceTexture*)pRsc;
	m_pTexture->loadState = RESOURCE_LOAD_STARTED;
}

bool ETextureDataProcessor::CompleteWork()
{
	m_pTexture->loadState = RESOURCE_LOAD_FINISHED;
	return true;
}

void ETextureDataProcessor::Process( void* pData, SIZE_T cBytes )
{
	GLOBAL::RDevice()->CreateTextureFromMemory( ( BYTE* )pData, cBytes, m_pTexture);
}