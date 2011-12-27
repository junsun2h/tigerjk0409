#include "EMeshDataProcessor.h"
#include "IRDevice.h"
#include "EAsyncLoader.h"


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

bool EMeshDataProcessor::PopData(IAssetMgr* pAssetMgr)
{
	for( UINT i=0; i< m_pResources.size(); ++i )
	{
		pAssetMgr->LoadCompletedResource(m_pResources[i]);
	}

	return true;
}

bool EMeshDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	assert ( EAsyncLoader::IsDataProcThread() );

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
	assert ( EAsyncLoader::IsDataProcThread() );

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
			pGeometry->RID_mtrl = GET_HASH_KEY( pGeometry->mtrlName);
		}
		
		{	// vertex information
			CResourceVB* pVB = new CResourceVB;

			std::string strVB = pGeometry->name;
			strVB += "_VB";
			strcpy_s( pVB->name, strVB.c_str() );
			pVB->RID = GET_HASH_KEY( pVB->name);

			ECopyData(&pVB->eType, &pSrcBits, 2);
			ECopyData(&pVB->count, &pSrcBits, 2);

			m_pRDevice->TS_CreateDPResource(DP_RESOURCE_VERTEX, pSrcBits, pVB->count ,pVB );
			*ppSrcBits += pVB->count;

			pGeometry->RID_vb = pVB->RID;

			m_pResources.push_back( pVB );
		}
		
		{	// index information
			CResourceIB* pIB = new CResourceIB;

			std::string strIB = pGeometry->name;
			strIB += "_IB";
			strcpy_s( pIB->name, strIB.c_str() );
			pIB->RID = GET_HASH_KEY( pIB->name);
			
			ECopyData(&pIB->eType, &pSrcBits, 2);
			ECopyData(&pIB->count, &pSrcBits, 2);

			m_pRDevice->TS_CreateDPResource(DP_RESOURCE_INDEX, pSrcBits, pIB->count ,pIB );
			*ppSrcBits += pIB->count;	

			pGeometry->RID_ib = pIB->RID;

			m_pResources.push_back( pIB );
		}

		pMesh->RID_goemetry[i] = pGeometry->RID;

		m_pResources.push_back( pGeometry );
	}

	m_pResources.push_back( pMesh );

	return pMesh->RID;
}
