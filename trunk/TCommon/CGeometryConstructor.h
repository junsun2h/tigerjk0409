#pragma once


#include "CResource.h"



enum eDIRECTION
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS,
};

struct CONE_MAKE_PARAM
{
	float radius; 
	float height; 
	UINT segment;
	CVector3 offset; 
	eDIRECTION direction;
	DWORD color;
};

namespace CGEOMETRY_CONSTRUCTOR
{
	inline void CreateCone(CONE_MAKE_PARAM& param, CVertexPC** ppVertex, uint16** ppIndex)
	{
		CVertexPC* pVertex = new CVertexPC[param.segment+1];

		if( param.direction == Z_AXIS )
		{
			pVertex[0].vPos = param.offset + CVector3(param.radius, 0, 0);
			pVertex[param.segment].vPos = param.offset + CVector3(0, 0, param.height);

			for( UINT i=1; i < param.segment ; ++i)
			{
				float angle = XM_2PI * i/float(param.segment);
				pVertex[i].vPos.x = param.radius * cos( angle);
				pVertex[i].vPos.y = param.radius * sin( angle);
				pVertex[i].vPos.z = 0.0f;

				pVertex[i].vPos += param.offset;
			}
		}
		else if( param.direction == X_AXIS)
		{
			pVertex[0].vPos = param.offset + CVector3(0, param.radius, 0);
			pVertex[param.segment].vPos = param.offset + CVector3( param.height, 0, 0);

			for( UINT i=1; i < param.segment ; ++i)
			{
				float angle = XM_2PI * i/float(param.segment);
				pVertex[i].vPos.y = param.radius * cos( angle);
				pVertex[i].vPos.z = param.radius * sin( angle);
				pVertex[i].vPos.x = 0.0f;

				pVertex[i].vPos += param.offset;
			}
		}
		else if( param.direction == Y_AXIS )
		{
			pVertex[0].vPos = param.offset + CVector3(0, 0, param.radius);
			pVertex[param.segment].vPos = param.offset + CVector3(0, param.height, 0);

			for( UINT i=1; i < param.segment ; ++i)
			{
				float angle = XM_2PI * i/float(param.segment);
				pVertex[i].vPos.z = param.radius * cos( angle);
				pVertex[i].vPos.x = param.radius * sin( angle);
				pVertex[i].vPos.y = 0.0f;

				pVertex[i].vPos += param.offset;
			}
		}

		*ppVertex = pVertex;

		uint16* pIndex = new uint16[ (param.segment * 2 - 1)* 3];
		// bottom
		for( UINT i=0; i < param.segment - 1; i++ )
		{
			int base = i*3;
			pIndex[base] = i + 1;
			pIndex[base + 1] = 0;
			pIndex[base + 2] = i + 2;
		}

		// side
		for( UINT i=0; i < param.segment; i++ )
		{
			int base = ( param.segment -1 ) * 3 + i*3;
			pIndex[base] = i;
			pIndex[base + 1] = i +1;
			pIndex[base + 2] = 10;
		}

		*ppIndex = pIndex;
	}

	inline void CreateConeGeometry(CResourceGeometry* pGeometry, CONE_MAKE_PARAM& param)
	{
		CVertexPC* pVertex = NULL;
		uint16* pIndex = NULL;

		CreateCone( param, &pVertex, &pIndex);

		for( UINT i=0; i< param.segment +1 ; ++i)
		{
			pVertex[i].color = param.color;
		}
		
		pGeometry->eVertexType = FVF_3FP_1DC;
		pGeometry->vertexCount = param.segment +1;
		pGeometry->pVertexBuffer = pVertex;

		pGeometry->eIndexType = INDEX_16BIT_TYPE;
		pGeometry->primitiveCount = param.segment * 2 - 1;
		pGeometry->pIndexBuffer = pIndex;
	}


	inline void CreateRect(CResourceGeometry* pGeometry, CVector3 vert[])
	{
		int16 index[6] = 
		{
			0, 1, 2,
			1, 2, 3,
		};

		pGeometry->vertexCount = 4;
		pGeometry->eVertexType = FVF_3FP;
		pGeometry->pVertexBuffer = new CVector3[4];
		memcpy( pGeometry->pVertexBuffer, vert, sizeof(CVector3) * 4 );

		pGeometry->primitiveCount =2;
		pGeometry->pIndexBuffer = new int16[60];
		memcpy( pGeometry->pIndexBuffer, index, 2 * 60 );
	}

}