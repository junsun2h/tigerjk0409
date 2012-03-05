#include "CResource.h"

#include "IAssetMgr.h"
#include "ILoader.h"
#include "IRDevice.h"

#include "EGlobal.h"
#include "EMeshDataProcessor.h"


void CalculateTangent(CResourceGeometry* pGeometry)
{
	if( pGeometry->eVertexType == FVF_4HP_4BN_2HT_4BW )
	{
		CVertexHPNTW* pOriginal = (CVertexHPNTW*)pGeometry->pVertexBuffer;
		CVertexHPNTTW* pVertexBuf = new CVertexHPNTTW[pGeometry->vertexCount];
		CVector3* pTangent1 = new CVector3[pGeometry->vertexCount * 2];
		CVector3* pTangent2 = pTangent1 + pGeometry->vertexCount;

		memset(pTangent1, 0 , sizeof(CVector3) * pGeometry->vertexCount* 2);

		for( UINT i=0; i < pGeometry->vertexCount ; ++i)
		{
			pVertexBuf[i].vPos = pOriginal[i].vPos;
			pVertexBuf[i].vNormal = pOriginal[i].vNormal;
			pVertexBuf[i].fWeight = pOriginal[i].fWeight;
			pVertexBuf[i].boneIDs = pOriginal[i].boneIDs;
		}

		uint16* pTriangle = (uint16*)pGeometry->pIndexBuffer;

		for (UINT i=0; i < pGeometry->primitiveCount*3;	i+=3)
		{
			uint16 i1 = pTriangle[i];
			uint16 i2 = pTriangle[i+1];
			uint16 i3 = pTriangle[i+2];

			float x1 = XMConvertHalfToFloat(pVertexBuf[i2].vPos.x) - XMConvertHalfToFloat(pVertexBuf[i1].vPos.x);
			float y1 = XMConvertHalfToFloat(pVertexBuf[i2].vPos.y) - XMConvertHalfToFloat(pVertexBuf[i1].vPos.y);
			float z1 = XMConvertHalfToFloat(pVertexBuf[i2].vPos.z) - XMConvertHalfToFloat(pVertexBuf[i1].vPos.z);
			float x2 = XMConvertHalfToFloat(pVertexBuf[i3].vPos.x) - XMConvertHalfToFloat(pVertexBuf[i1].vPos.x);
			float y2 = XMConvertHalfToFloat(pVertexBuf[i3].vPos.y) - XMConvertHalfToFloat(pVertexBuf[i1].vPos.y);
			float z2 = XMConvertHalfToFloat(pVertexBuf[i3].vPos.z) - XMConvertHalfToFloat(pVertexBuf[i1].vPos.z);

			float s1 = XMConvertHalfToFloat(pVertexBuf[i2].vPos.x) - XMConvertHalfToFloat(pVertexBuf[i1].vPos.x);
			float t1 = XMConvertHalfToFloat(pVertexBuf[i2].vNormal.w) - XMConvertHalfToFloat(pVertexBuf[i1].vNormal.w);
			float s2 = XMConvertHalfToFloat(pVertexBuf[i3].vPos.x) - XMConvertHalfToFloat(pVertexBuf[i1].vPos.x);
			float t2 = XMConvertHalfToFloat(pVertexBuf[i3].vNormal.w) - XMConvertHalfToFloat(pVertexBuf[i1].vNormal.w);

			float r = 1.0f / (s1 * t2 - s2 * t1);

			CVector3 sdir( (t2 * x1 - t1 * x2) * r,
							(t2 * y1 - t1 * y2) * r,
							(t2 * z1 - t1 * z2) * r);

			CVector3 tdir( (s1 * x2 - s2 * x1) * r,
							(s1 * y2 - s2 * y1) * r,
							(s1 * z2 - s2 * z1) * r);

			pTangent1[i1] += sdir;
			pTangent1[i2] += sdir;
			pTangent1[i3] += sdir;

			pTangent2[i1] += tdir;
			pTangent2[i2] += tdir;
			pTangent2[i3] += tdir;
		}

		for (UINT i = 0; i < pGeometry->vertexCount; ++i)
		{
			CVector3 n;
			n.x = XMConvertHalfToFloat( pVertexBuf[i].vNormal.x );
			n.y = XMConvertHalfToFloat( pVertexBuf[i].vNormal.y );
			n.z = XMConvertHalfToFloat( pVertexBuf[i].vNormal.z );

			CVector3& t = pTangent1[i];

			// Gram-Schmidt orthogonalization
			pTangent1[i] = CVector3::Normalize(t - n * CVector3::Dot(n, t)  );
			if( CVector3::Dot( CVector3::Cross(n, t), pTangent2[i]) < 0.0f )
				pVertexBuf[i].vTangent = XMHALF4(pTangent1[i].x, pTangent1[i].y, pTangent1[i].z, -1.0f);
			else
				pVertexBuf[i].vTangent = XMHALF4(pTangent1[i].x, pTangent1[i].y, pTangent1[i].z, 1.0f);
		}

		SAFE_DELETE(pGeometry->pVertexBuffer);
		SAFE_DELETE(pTangent1);
		pGeometry->pVertexBuffer = pVertexBuf;

		pGeometry->eVertexType = FVF_4HP_4BN_2HT_4BT_4BW;
	}
}

//----------------------------------------------------------------------------------------------------------------------------
void EMeshDataProcessor::Init(CResourceBase* pRsc, bool bForward)
{
	m_pMesh = (CResourceMesh*)pRsc;
	m_pMesh->loadState = RESOURCE_LOAD_STARTED;
	m_bForward = bForward;
}

bool EMeshDataProcessor::CompleteWork()
{
	for(UINT i =0; i< m_pMesh->goemetries.size(); ++i)
	{
		CResourceGeometry* pGeometry = m_pMesh->goemetries[i];
	}

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

		pGeometry->pMaterial = (CResourceMtrl*)GLOBAL::Loader()->Load( pGeometry->mtrlName, RESOURCE_FILE_MATERIAL, m_bForward );
		if( pGeometry->pMaterial->pTextures[TEXTURE_BUMP] != NULL )
			CalculateTangent(pGeometry);

		GLOBAL::RDevice()->CreateGraphicBuffer( pGeometry );
		if( pGeometry->IsSkinMesh() )
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