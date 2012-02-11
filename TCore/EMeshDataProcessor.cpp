#include "CResource.h"

#include "IAssetMgr.h"
#include "ILoader.h"
#include "IResourceMemMgr.h"

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
	for( UINT i=0; i< m_pResources.size(); ++i )
	{
		GLOBAL::AssetMgr()->Insert(m_pResources[i]);
	}

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
	
	CResourceMesh* pMesh = (CResourceMesh*)GLOBAL::ResourceMemMgr()->GetNew(RESOURCE_MESH);

	ECopyData( &pMesh->min, &pSrcBits,  12 );
	ECopyData( &pMesh->max, &pSrcBits,  12 );
	ECopyData( &pMesh->geometryNum, &pSrcBits,  1 );

	char buf[32];
	for(int i=0; i< pMesh->geometryNum; ++i)
	{
		_itoa_s( i, buf, 32);
		CResourceGeometry* pGeometry = (CResourceGeometry*)GLOBAL::ResourceMemMgr()->GetNew(RESOURCE_GEOMETRY);

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


		std::string geometryName = m_name + "Geometry" + buf;
		strcpy_s( pGeometry->name, geometryName.c_str());

		pMesh->goemetries[i] = GLOBAL::AssetMgr()->Insert( pGeometry );
	}

	strcpy_s( pMesh->name, m_name.c_str());
	GLOBAL::AssetMgr()->Insert( pMesh );
	return pMesh;
}

bool EMeshDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	assert ( GLOBAL::Loader()->IsDataProcThread() );
			
	return true;
}