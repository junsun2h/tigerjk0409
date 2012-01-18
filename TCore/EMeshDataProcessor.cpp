#include "EMeshDataProcessor.h"
#include "IRDevice.h"
#include "EEngine.h"


void ECopyData(void* dest, BYTE** src, int size)
{
	CopyMemory( dest, *src, size );
	*src += size;
}

void ECopyString(void* dest, BYTE** src)
{
	BYTE nameLength;
	ECopyData(&nameLength, src, 1);
	ECopyData(&dest, src, nameLength);
}


EMeshDataProcessor::EMeshDataProcessor( IRDevice* pRDevice, std::string name )
	: m_pRDevice(pRDevice)
	, m_name(name)
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

bool EMeshDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	assert ( ELoader::IsDataProcThread() );

	byte ptr = NULL;

	BYTE* pSrcBits = ( BYTE* )pData;

	uint16 version;
	uint8 levelOfDetail;

	ECopyData(&version, &pSrcBits, 2);
	ECopyData(&levelOfDetail, &pSrcBits, 2);

	if( levelOfDetail = 1 )
	{
		PT_ReadMesh(&pSrcBits, m_name);
	}
	else
	{
		CResourceLODMesh* pResource = new CResourceLODMesh;
		pResource->RID = GET_HASH_KEY(m_name);	
		pResource->LOD = levelOfDetail;
		char s[25];

		for( int i =0; i < levelOfDetail; ++i)
		{
			_itoa_s(i, s, 10);
			pResource->RID_mesh[i] = PT_ReadMesh(&pSrcBits, m_name + s );
		}

		m_pResources.push_back( pResource );
	}
	
	return true;
}


long EMeshDataProcessor::PT_ReadMesh(BYTE** ppSrcBits, std::string name)
{
	assert ( ELoader::IsDataProcThread() );

	BYTE* pSrcBits = *ppSrcBits;
	
	CResourceMesh* pMesh = new CResourceMesh;
	pMesh->RID = GET_HASH_KEY(name);
	strcpy_s( pMesh->name, m_name.c_str() );

	ECopyData(&pMesh->geometryNum, &pSrcBits, 1);

	for( int i=0; pMesh->geometryNum; ++i )
	{
		CResourceGeometry* pGeometry = new CResourceGeometry;
		
		ECopyString(&pGeometry->name, &pSrcBits );
		pGeometry->RID = GET_HASH_KEY( pGeometry->name);

		{	// default material
			ECopyString(&pGeometry->mtrlName, &pSrcBits );
			pGeometry->defaultMtrl = GET_HASH_KEY( pGeometry->mtrlName);
		}
		
		{	// vertex information
			ECopyData(&pGeometry->eVertexType, &pSrcBits, 2);
			ECopyData(&pGeometry->vertexCount, &pSrcBits, 2);

			memcpy(pGeometry->pVertexBuffer, pSrcBits, SIZE_OF_VERTEX(pGeometry->eVertexType) * pGeometry->vertexCount );
			*ppSrcBits += pGeometry->vertexCount;
		}
		
		{	// index information
			ECopyData(&pGeometry->eIndexType, &pSrcBits, 2);
			ECopyData(&pGeometry->primitiveCount, &pSrcBits, 2);

			if( pGeometry->eIndexType == INDEX_16BIT_TYPE )
				memcpy(pGeometry->pIndexBuffer, pSrcBits, 2 * 3 * pGeometry->primitiveCount );
			else
				memcpy(pGeometry->pIndexBuffer, pSrcBits, 4 * 3 * pGeometry->primitiveCount );
			*ppSrcBits += pGeometry->primitiveCount;	
		}
		m_pRDevice->PT_CreateGraphicBuffer( pGeometry );

		pMesh->goemetries[i] = pGeometry->RID;

		m_pResources.push_back( pGeometry );
	}

	m_pResources.push_back( pMesh );

	return pMesh->RID;
}
