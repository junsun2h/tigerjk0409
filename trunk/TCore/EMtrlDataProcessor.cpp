#include "CResource.h"

#include "ILoader.h"
#include "IRDevice.h"
#include "IAssetMgr.h"

#include "XmlHelper.h"

#include "EGlobal.h"
#include "EMtrlDataProcessor.h"



void EMtrlDataProcessor::Init(CResourceBase* pRsc, bool bForward)
{
	m_pMaterial = (CResourceMtrl*)pRsc;
	m_pMaterial->loadState = RESOURCE_LOAD_STARTED;
}


bool EMtrlDataProcessor::CompleteWork()
{
	if( m_bForward )
		m_pMaterial->loadState = RESOURCE_LOAD_FINISHED;

	for( UINT i =0 ; i < NUM_TEXTURE_TYPE; ++i )
	{
		if( m_pMaterial->pTextures[i] == NULL )
			continue;

		if( m_pMaterial->pTextures[i]->loadState != RESOURCE_LOAD_FINISHED )
			return false;
	}

	m_pMaterial->loadState = RESOURCE_LOAD_FINISHED;
	return true;
}


const CResourceTexture* LoadTexture(TiXmlNode* pTextureNode, bool bForward)
{
	if( pTextureNode == NULL)
		return NULL;

	char ptextureName[64];
	GetAttribute( pTextureNode ,"texture", ptextureName);

	IAssetMgr* pAssetMgr = GLOBAL::AssetMgr();

	const CResourceBase* pTexture = GLOBAL::Loader()->Load( ptextureName, RESOURCE_FILE_TEXTURE, bForward );

	return (const CResourceTexture*)pTexture;
}


void EMtrlDataProcessor::Process( void* pData, SIZE_T cBytes )
{
	TiXmlDocument doc;
	doc.Parse( (const char*)pData );

	TiXmlNode* pMtrl = doc.FirstChild("mtrl");
	if( pMtrl == NULL )
	{
		assert(0);
		m_pMaterial->loadState = RESOURCE_LOAD_FAILED;
		return;
	}

	GetAttribute( pMtrl , "twoSided", m_pMaterial->bTwoSided);
	GetAttribute( pMtrl , "opacity", m_pMaterial->opacity);
	GetAttribute( pMtrl , "specularLevel", m_pMaterial->specularLevel);
	GetAttribute( pMtrl , "glossiness", m_pMaterial->glossiness	);

	TiXmlNode* pMap = pMtrl->FirstChild("map");
	if( pMap )
	{
		m_pMaterial->pTextures[TEXTURE_DIFFISE] = LoadTexture( pMap->FirstChild("Diffuse") , m_bForward);
		m_pMaterial->pTextures[TEXTURE_SPECULAR] = LoadTexture( pMap->FirstChild("Specular") , m_bForward  );
		m_pMaterial->pTextures[TEXTURE_BUMP] = LoadTexture( pMap->FirstChild("Bump") , m_bForward );
	}

	m_pMaterial->loadState = RESOURCE_LOAD_FINISHED_WAIT_FOR_SUB_RSC;
}