#include "CResource.h"

#include "IAssetMgr.h"
#include "ILoader.h"
#include "IRDevice.h"

#include "EGlobal.h"
#include "EMeshDataProcessor.h"





EMeshDataProcessor::EMeshDataProcessor()
{
}

EMeshDataProcessor::~EMeshDataProcessor()
{

}

void EMeshDataProcessor::Init(CResourceBase* pRsc, bool bForward)
{
	m_pMesh = (CResourceMesh*)pRsc;
	m_pMesh->loadState = RESOURCE_LOAD_STARTED;
	m_bForward = bForward;
}

bool EMeshDataProcessor::CompleteWork()
{
	if( m_bForward )
		m_pMesh->loadState = RESOURCE_LOAD_FINISHED;

	return true;
}

void EMeshDataProcessor::Process( void* pData, SIZE_T cBytes )
{
	IAssetMgr* pAssetMgr = GLOBAL::AssetMgr();
	BYTE* pSrcBits = ( BYTE* )pData;

	UINT version;
	ECopyData( &version, &pSrcBits,  4 );

	if( version != MESH_FILE_VERSION )
	{
		assert(0);
		m_pMesh->loadState = RESOURCE_LOAD_FAILED;
		return;
	}
	
	ECopyData( &m_pMesh->min, &pSrcBits,  12 );
	ECopyData( &m_pMesh->max, &pSrcBits,  12 );

	uint8 geometryNum;
	ECopyData( &geometryNum, &pSrcBits,  1 );

	bool bSkinMesh = false;
	char buf[64];
	for(int i=0; i< geometryNum; ++i)
	{
		_itoa_s( i, buf, 32);
		std::string geometryName = std::string(m_pMesh->name) + "Geometry" + buf;

		CResourceGeometry* pGeometry = (CResourceGeometry*)pAssetMgr->CreateResource( RESOURCE_GEOMETRY, geometryName.c_str() );

		ECopyData( &pGeometry->eVertexType, &pSrcBits,  4 );
		ECopyData( &pGeometry->vertexCount, &pSrcBits,  4 );

		pGeometry->pVertexBuffer = NEW_VERTEX( pGeometry->eVertexType, pGeometry->vertexCount); 
		ECopyData( pGeometry->pVertexBuffer, &pSrcBits, VERTEX_STRIDE(pGeometry->eVertexType) * pGeometry->vertexCount );

		ECopyData( &pGeometry->eIndexType, &pSrcBits,  4 );
		ECopyData( &pGeometry->primitiveCount, &pSrcBits,  4 );

		if( pGeometry->eIndexType == INDEX_16BIT_TYPE)
		{
			pGeometry->pIndexBuffer = new uint16[ pGeometry->primitiveCount * 3 ]; 
			ECopyData( pGeometry->pIndexBuffer, &pSrcBits, 2 * pGeometry->primitiveCount * 3);
		}
		else
		{
			pGeometry->pIndexBuffer = new UINT[ pGeometry->primitiveCount * 3 ]; 
			ECopyData( pGeometry->pIndexBuffer, &pSrcBits, 4 * pGeometry->primitiveCount * 3);
		}

		ECopyString(pGeometry->mtrlName, &pSrcBits);
/*
		CResourceMtrl* pMtrl = (CResourceMtrl*)pAssetMgr->GetResource(RESOURCE_MATERIAL, pGeometry->mtrlName);
		if( pMtrl == NULL)
			pMtrl = (CResourceMtrl*)GLOBAL::Loader()->Load( pGeometry->mtrlName, RESOURCE_FILE_MESH, m_bForward );
		pGeometry->defaultMtrl = pMtrl->RID;*/

		GLOBAL::RDevice()->CreateGraphicBuffer( pGeometry );
		if( pGeometry->IsSkinedMesh() )
			bSkinMesh = true;

		m_pMesh->goemetries.push_back( pGeometry );
		pGeometry->loadState = RESOURCE_LOAD_FINISHED;
	}

	if( bSkinMesh )
	{
		uint8 boneCount;
		ECopyData( &boneCount, &pSrcBits,  1 );
		for( UINT bi=0; bi < boneCount; ++bi )
		{
			ECopyString(buf, &pSrcBits);
			m_pMesh->skinBoneList.push_back(buf);
		}
	}

	m_pMesh->loadState = RESOURCE_LOAD_FINISHED_WAIT_FOR_SUB_RSC;
}

bool EMeshDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	return true;
}