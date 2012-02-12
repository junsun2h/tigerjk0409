#include "CResource.h"

#include "IAssetMgr.h"
#include "ILoader.h"
#include "IRDevice.h"

#include "EGlobal.h"
#include "EMeshDataProcessor.h"





EMeshDataProcessor::EMeshDataProcessor( std::string name )
	: m_name(name)
{
}

EMeshDataProcessor::~EMeshDataProcessor()
{

}

bool EMeshDataProcessor::PopData()
{
	return true;
}

CResourceBase* EMeshDataProcessor::Process( void* pData, SIZE_T cBytes )
{
	BYTE* pSrcBits = ( BYTE* )pData;

	UINT version;
	ECopyData( &version, &pSrcBits,  4 );

	if( version != MESH_FILE_VERSION )
	{
		assert(0);
		return NULL;
	}

	IAssetMgr* pAssetMgr = GLOBAL::AssetMgr();
	CResourceMesh* pMesh = (CResourceMesh*)pAssetMgr->CreateResource( RESOURCE_MESH, m_name.c_str() );
	pMesh->loadState = RESOURCE_LOAD_STARTED;

	ECopyData( &pMesh->min, &pSrcBits,  12 );
	ECopyData( &pMesh->max, &pSrcBits,  12 );
	ECopyData( &pMesh->geometryNum, &pSrcBits,  1 );

	char buf[32];
	for(int i=0; i< pMesh->geometryNum; ++i)
	{
		_itoa_s( i, buf, 32);
		std::string geometryName = m_name + "Geometry" + buf;

		CResourceGeometry* pGeometry = (CResourceGeometry*)pAssetMgr->CreateResource( RESOURCE_GEOMETRY, geometryName.c_str() );
		pMesh->loadState = RESOURCE_LOAD_STARTED;

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

		GLOBAL::Engine()->RDevice()->CreateGraphicBuffer( pGeometry );
		pGeometry->loadState = RESOURCE_LOAD_FINISHED;

		pMesh->goemetries[i] = pGeometry->RID;
	}

	pMesh->loadState = RESOURCE_LOAD_FINISHED;
	return pMesh;
}

bool EMeshDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	assert ( GLOBAL::Loader()->IsDataProcThread() );
			
	return true;
}