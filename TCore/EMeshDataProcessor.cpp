#include "EMeshDataProcessor.h"
#include "EEngine.h"


void ECopyData(void* dest, BYTE** src, int size)
{
	CopyMemory( dest, *src, size );
	*src += size;
}

void ECopyString(char* dest, BYTE** src)
{
	BYTE nameLength;
	ECopyData(&nameLength, src, 1);
	strcpy_s( dest, 64, (char*)*src);
	*src += nameLength;
}


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
		g_Engine.AssetMgr()->Insert(m_pResources[i]);
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
	
	CResourceMesh* pMesh = (CResourceMesh*)g_Engine.EngineMemoryMgr()->GetNewResource(RESOURCE_MESH);

	ECopyData( &pMesh->min, &pSrcBits,  12 );
	ECopyData( &pMesh->max, &pSrcBits,  12 );
	ECopyData( &pMesh->geometryNum, &pSrcBits,  1 );


	for(int i=0; i< pMesh->geometryNum; ++i)
	{
		CResourceGeometry* pGeometry = (CResourceGeometry*)g_Engine.EngineMemoryMgr()->GetNewResource(RESOURCE_GEOMETRY);

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

		pMesh->goemetries[i] = g_Engine.AssetMgr()->Insert( pGeometry );
	}

	strcpy_s( pMesh->name, m_name.c_str());
	g_Engine.AssetMgr()->Insert( pMesh );
	return pMesh;
}

bool EMeshDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	assert ( ELoader::IsDataProcThread() );
			
	return true;
}