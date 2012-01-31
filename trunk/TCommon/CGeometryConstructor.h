#pragma once


#include "CResource.h"



enum eDIRECTION
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS,
	CENTER
};

struct CONE_MAKE_PARAM
{
	float radius; 
	float height; 
	UINT segment;
	CVector3 offset; 
	eDIRECTION direction;
	DWORD color;
	
	CONE_MAKE_PARAM()
	{
		memset( this, 0, sizeof(CONE_MAKE_PARAM) );
	}
};

struct CIRCLE_MAKE_PARAM
{
	float radius; 
	UINT segment;
	CVector3 offset; 
	eDIRECTION direction;
	DWORD color;

	CIRCLE_MAKE_PARAM()
	{
		memset( this, 0, sizeof(CIRCLE_MAKE_PARAM) );
	}
};

struct BOX_MAKE_PARAM
{
	CVector3 min;
	CVector3 max;
	CVector3 offset; 
	DWORD color;

	BOX_MAKE_PARAM()
	{
		memset( this, 0, sizeof(BOX_MAKE_PARAM) );
	}
};

namespace CGEOMETRY_CONSTRUCTOR
{
	void CreateCircle(CIRCLE_MAKE_PARAM& param, CVertexPC** ppVertex, uint16** ppIndex);
	void CreateCone(CONE_MAKE_PARAM& param, CVertexPC** ppVertex, uint16** ppIndex);
	void CreateBox(BOX_MAKE_PARAM& param, CVertexPC** ppVertex, uint16** ppIndex);
	void CreateBoxLine(BOX_MAKE_PARAM& param, CVertexPC** ppVertex);

	void CreateCircleGeometry(CResourceGeometry* pGeometry, CIRCLE_MAKE_PARAM& param);
	void CreateConeGeometry(CResourceGeometry* pGeometry, CONE_MAKE_PARAM& param);
	void CreateBoxGeometry(CResourceGeometry* pGeometry, BOX_MAKE_PARAM& param);

	//-----------------------------------------------------------------------------------------------------------
	inline void CreateBoxLine(BOX_MAKE_PARAM& param, CVertexPC** ppVertex)
	{
		CVector3 tmpVertex[8];

		tmpVertex[0] = param.min;

		tmpVertex[1] = tmpVertex[0];
		tmpVertex[1].x = param.max.x;

		tmpVertex[2] = param.max;
		tmpVertex[2].y = param.min.y;

		tmpVertex[3] = param.min;
		tmpVertex[3].z = param.max.z;

		tmpVertex[4] = tmpVertex[0];
		tmpVertex[4].y = param.max.y;

		tmpVertex[5] = tmpVertex[1];
		tmpVertex[5].y = param.max.y;

		tmpVertex[6] = tmpVertex[2];
		tmpVertex[6].y = param.max.y;

		tmpVertex[7] = tmpVertex[3];
		tmpVertex[7].y = param.max.y;

		CVertexPC* pVertices = new CVertexPC[24];
		// bottom
		pVertices[0].vPos = tmpVertex[0];
		pVertices[1].vPos = tmpVertex[1];
		pVertices[2].vPos = tmpVertex[1];
		pVertices[3].vPos = tmpVertex[2];
		pVertices[4].vPos = tmpVertex[2];
		pVertices[5].vPos = tmpVertex[3];
		pVertices[6].vPos = tmpVertex[3];
		pVertices[7].vPos = tmpVertex[0];

		// top
		pVertices[8].vPos = tmpVertex[4];
		pVertices[9].vPos = tmpVertex[5];
		pVertices[10].vPos = tmpVertex[5];
		pVertices[11].vPos = tmpVertex[6];
		pVertices[12].vPos = tmpVertex[6];
		pVertices[13].vPos = tmpVertex[7];
		pVertices[14].vPos = tmpVertex[7];
		pVertices[15].vPos = tmpVertex[4];

		// side
		pVertices[16].vPos = tmpVertex[0];
		pVertices[17].vPos = tmpVertex[4];
		pVertices[18].vPos = tmpVertex[1];
		pVertices[19].vPos = tmpVertex[5];
		pVertices[20].vPos = tmpVertex[2];
		pVertices[21].vPos = tmpVertex[6];
		pVertices[22].vPos = tmpVertex[3];
		pVertices[23].vPos = tmpVertex[7];

		*ppVertex = pVertices;
	}

	//-----------------------------------------------------------------------------------------------------------
	inline void CreateCircle(CIRCLE_MAKE_PARAM& param, CVertexPC** ppVertex, uint16** ppIndex)
	{
		if( param.segment < 3)
		{
			assert(0);
			return;
		}
		
		CVertexPC* pVertex = new CVertexPC[param.segment];

		if( param.direction == Z_AXIS )
		{
			pVertex[0].vPos = param.offset + CVector3(param.radius, 0, 0);

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

		uint16* pIndex = new uint16[ (param.segment - 2)* 3];
		// bottom
		for( UINT i=0; i < param.segment - 2; i++ )
		{
			int base = i*3;
			pIndex[base] = i + 1;
			pIndex[base + 1] = 0;
			pIndex[base + 2] = i + 2;
		}

		*ppIndex = pIndex;
	}

	//-----------------------------------------------------------------------------------------------------------
	inline void CreateCircleGeometry(CResourceGeometry* pGeometry, CIRCLE_MAKE_PARAM& param)
	{
		if( param.segment < 3)
		{
			assert(0);
			return;
		}

		CVertexPC* pVertex = NULL;
		uint16* pIndex = NULL;

		CreateCircle( param, &pVertex, &pIndex);
		
		for( UINT i=0; i< param.segment ; ++i)
		{
			pVertex[i].color = param.color;
		}

		pGeometry->eVertexType = FVF_3FP_1DC;
		pGeometry->vertexCount = param.segment;
		pGeometry->pVertexBuffer = pVertex;

		pGeometry->eIndexType = INDEX_16BIT_TYPE;
		pGeometry->primitiveCount = param.segment - 2;
		pGeometry->pIndexBuffer = pIndex;
	}

	//-----------------------------------------------------------------------------------------------------------
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

	//-----------------------------------------------------------------------------------------------------------
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

	//-----------------------------------------------------------------------------------------------------------
	inline void CreateBox(BOX_MAKE_PARAM& param, CVertexPC** ppVertex, uint16** ppIndex)
	{
		CVector3 tmpVertex[8];

		tmpVertex[0] = param.min;

		tmpVertex[1] = tmpVertex[0];
		tmpVertex[1].x = param.max.x;

		tmpVertex[2] = param.max;
		tmpVertex[2].y = param.min.y;

		tmpVertex[3] = param.min;
		tmpVertex[3].z = param.max.z;

		tmpVertex[4] = tmpVertex[0];
		tmpVertex[4].y = param.max.y;

		tmpVertex[5] = tmpVertex[1];
		tmpVertex[5].y = param.max.y;

		tmpVertex[6] = tmpVertex[2];
		tmpVertex[6].y = param.max.y;

		tmpVertex[7] = tmpVertex[3];
		tmpVertex[7].y = param.max.y;

		CVertexPC* pVertex = new CVertexPC[8];
		
		for( int i=0; i < 8 ; ++i)
		{
			pVertex[i].vPos = param.offset + tmpVertex[i];
		}

		*ppVertex = pVertex;

		uint16 index[36] = 
		{
			// bottom
			0,1,2,
			0,2,3,

			// top
			5,4,6,
			6,4,7,

			// sides
			3,2,6,
			3,6,7,

			0,3,7,
			0,7,4,

			1,0,4,
			1,4,5,

			2,1,6,
			6,1,5
		};
		
		*ppIndex = new uint16[36];
		memcpy(*ppIndex, index, 2 * 36);
	}

	//-----------------------------------------------------------------------------------------------------------
	inline void CreateBoxGeometry(CResourceGeometry* pGeometry, BOX_MAKE_PARAM& param)
	{
		CVertexPC* pVertex = NULL;
		uint16* pIndex = NULL;

		CreateBox( param, &pVertex, &pIndex);

		for( UINT i=0; i< 8 ; ++i)
		{
			pVertex[i].color = param.color;
		}

		pGeometry->eVertexType = FVF_3FP_1DC;
		pGeometry->vertexCount = 8;
		pGeometry->pVertexBuffer = pVertex;

		pGeometry->eIndexType = INDEX_16BIT_TYPE;
		pGeometry->primitiveCount = 12;
		pGeometry->pIndexBuffer = pIndex;
	}
}