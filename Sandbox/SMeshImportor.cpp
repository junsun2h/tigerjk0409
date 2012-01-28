#include "SMeshImportor.h"
#include "CResource.h"
#include <iostream>
#include <fstream>

namespace SMESH_LOADER
{
	typedef std::vector<CResourceGeometry*>	GEOMETRY_LIST;

	TRAW_MESH::TRAW_MESH()
		: BBoxMin(FLT_MAX, FLT_MAX, FLT_MAX)
		, BBoxMax(-FLT_MAX, -FLT_MAX, -FLT_MAX)
		, coordSys(COODSYS_RAW)
	{
	}

	TRAW_MESH::~TRAW_MESH()
	{
		MTRL_FACE_MAP::iterator itr = faceMtrlID.begin();
		while (itr != faceMtrlID.end())
		{
			SAFE_DELETE(itr->second);
			itr++;
		}
	}

	void TRAW_MESH::ChangeCoordsys(TCOODSYS coodSys_)
	{
		// max coordinate -> directx coordinate
		if( coordSys == COODSYS_RAW && coodSys_ == COODSYS_DIRECTX )
		{
			coordSys = coodSys_;
			int posCount = posList.size();
			for( int i=0; i < posCount; ++i)
				TSWAP( posList[i].y, posList[i].z );

			int normalCount = normalList.size();
			for( int i=0; i < normalCount; ++i)
				TSWAP( normalList[i].y, normalList[i].z );

			TSWAP( BBoxMin.y, BBoxMin.z );
			TSWAP( BBoxMax.y, BBoxMax.z );

			int uvCount = uvList.size();
			for( int i=0; i < uvCount; ++i)
				uvList[i].y = 1 - uvList[i].y;

			TSWAP( postion.y,  postion.z);

			rotation.x *= -1;
			rotation.y *= -1;
			rotation.z *= -1;
			TSWAP( rotation.y,  rotation.z);

			int faceCount = facePos.size();
			for( int i=0; i < faceCount; i += 3)
			{
				TSWAP( facePos[i], facePos[i+2] );
				TSWAP( faceNormal[i], faceNormal[i+2] );
				TSWAP( faceUV[i], faceUV[i+2] );
			}

			TSWAP( BBoxMax.y, BBoxMax.z );
		}
	}



	//------------------------------------------------------------------------------------------------------
	bool LoadRawMesh(const char* strFileName, TRAW_MESH& RawMesh)
	{
		char* delimiters = " ";
		char* pContext = NULL;

	#define READ_BUFFER_SIZE 256
		FILE *fp;

		char buf[READ_BUFFER_SIZE];
		if( fopen_s( &fp, strFileName, "rb") != 0)
			return false;

		while (!feof (fp))
		{
			fgets (buf, sizeof (buf), fp);

			if( strncmp( buf, "transform",  9) == 0 )
			{
				fgets (buf, sizeof (buf), fp);

				RawMesh.postion.x = (float)atof( strtok_s( buf, delimiters , &pContext ) );				
				RawMesh.postion.y = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	
				RawMesh.postion.z = (float)atof( strtok_s( NULL, delimiters , &pContext ) );

				fgets (buf, sizeof (buf), fp);

				RawMesh.rotation.x = (float)atof( strtok_s( buf, delimiters , &pContext ) );		
				RawMesh.rotation.y = (float)atof( strtok_s( NULL, delimiters , &pContext ) );
				RawMesh.rotation.z = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	
				RawMesh.rotation.w = (float)atof( strtok_s( NULL, delimiters , &pContext ) );
			}
			else if( strncmp( buf, "material",  8) == 0 )
			{
				fgets (buf, sizeof (buf), fp);
				if( strncmp( buf, "Multimaterial" ,13) == 0 )
				{
					fgets (buf, sizeof (buf), fp);
					int count = atoi( buf );
					for( int i=0; i< count; ++i)
					{
						fgets (buf, sizeof (buf), fp);
						RawMesh.mtrlList.push_back( std::string(buf) );
					}
				}
				else
				{
					RawMesh.mtrlList.push_back( std::string(buf) );
				}
			}
			else if( strncmp( buf, "position",  8) == 0 )
			{
				int count = atoi( &buf[9] );
				RawMesh.posList.reserve( count );
				for( int i=0; i< count; ++i)
				{
					fgets (buf, sizeof (buf), fp);
					CVector3 pos;

					pos.x = (float)atof( strtok_s( buf, delimiters , &pContext ) );				
					pos.y = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	
					pos.z = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	

					if( RawMesh.BBoxMin.x > pos.x )
						RawMesh.BBoxMin.x = pos.x;
					else if( RawMesh.BBoxMax.x < pos.x)
						RawMesh.BBoxMax.x = pos.x;

					if( RawMesh.BBoxMin.y > pos.y )
						RawMesh.BBoxMin.y = pos.y;
					else if( RawMesh.BBoxMax.y < pos.y)
						RawMesh.BBoxMax.y = pos.y;

					if( RawMesh.BBoxMin.z > pos.z )
						RawMesh.BBoxMin.z = pos.z;
					else if( RawMesh.BBoxMax.z < pos.z)
						RawMesh.BBoxMax.z = pos.z;

					RawMesh.posList.push_back( pos );
				}
			}
			else if( strncmp( buf, "normal", 6) == 0 )
			{
				int count = atoi( &buf[7] );
				RawMesh.normalList.reserve( count );
				for( int i=0; i< count; ++i)
				{
					fgets (buf, sizeof (buf), fp);
					CVector3 normal;

					normal.x = (float)atof( strtok_s( buf, delimiters , &pContext ) );				
					normal.y = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	
					normal.z = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	

					RawMesh.normalList.push_back( normal );
				}
			}
			else if( strncmp( buf, "uv", 2 ) == 0 )
			{
				int count = atoi( &buf[3] );
				RawMesh.uvList.reserve( count );
				for( int i=0; i< count; ++i)
				{
					fgets (buf, sizeof (buf), fp);
					CVector2 uv;

					uv.x = (float)atof( strtok_s( buf, delimiters , &pContext ) );				
					uv.y = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	

					RawMesh.uvList.push_back( uv );
				}
			}
			else if( strncmp( buf, "facePos", 7 ) == 0 )
			{
				int count = atoi( &buf[8] );
				RawMesh.facePos.reserve( count );
				for( int i=0; i< count; ++i)
				{
					fgets (buf, sizeof (buf), fp);

					RawMesh.facePos.push_back( atoi( strtok_s( buf, delimiters , &pContext ) ) );				
					RawMesh.facePos.push_back( atoi( strtok_s( NULL, delimiters , &pContext ) ) );	
					RawMesh.facePos.push_back( atoi( strtok_s( NULL, delimiters , &pContext ) ) );
				}
			}
			else if( strncmp( buf, "faceUV", 6 ) == 0 )
			{
				int count = atoi( &buf[7] );
				RawMesh.faceUV.reserve( count );
				for( int i=0; i< count; ++i)
				{
					fgets (buf, sizeof (buf), fp);

					RawMesh.faceUV.push_back( atoi( strtok_s( buf, delimiters , &pContext ) ) );				
					RawMesh.faceUV.push_back( atoi( strtok_s( NULL, delimiters , &pContext ) ) );	
					RawMesh.faceUV.push_back( atoi( strtok_s( NULL, delimiters , &pContext ) ) );
				}
			}
			else if( strncmp( buf, "faceNormal", 10 ) == 0 )
			{
				int count = atoi( &buf[11] );
				RawMesh.faceNormal.reserve( count );
				for( int i=0; i< count; ++i)
				{
					fgets (buf, sizeof (buf), fp);

					RawMesh.faceNormal.push_back( atoi( strtok_s( buf, delimiters , &pContext ) ) );				
					RawMesh.faceNormal.push_back( atoi( strtok_s( NULL, delimiters , &pContext ) ) );	
					RawMesh.faceNormal.push_back( atoi( strtok_s( NULL, delimiters , &pContext ) ) );
				}
			}
			else if( strncmp( buf, "faceID", 6 ) == 0 )
			{
				int mtrlCount = RawMesh.mtrlList.size();
				for( int i=0; i < mtrlCount ; ++i)
				{
					RawMesh.faceMtrlID.insert( MTRL_FACE_MAP::value_type( i, new std::vector<int> ));
				}

				int count = atoi( &buf[7] );
				for( int i=0; i < count; ++i )
				{
					fgets (buf, sizeof (buf), fp);
					int mtrlID = atoi(buf) - 1;
					RawMesh.faceMtrlID.find( mtrlID )->second->push_back( i );
				}
			}
			else if( strncmp( buf, "weight", 6 ) == 0 )
			{
				int count = atoi( &buf[7] );	
				RawMesh.weightList.reserve( count );
				for( int i=0; i < count; ++i )
				{
					fgets (buf, sizeof (buf), fp);
					RAW_WEIGHT w;
					w.count = atoi( strtok_s( buf, delimiters , &pContext ) );
					if( w.count > 4)
						w.count = 4;

					for(int j=0 ; j < w.count ; ++j )
					{
						w.bone[j] = atoi( strtok_s( NULL, delimiters , &pContext ) );
						if ( j < 3 )
							w.weight[j] = (float)atof( strtok_s( NULL, delimiters , &pContext ) );
					}

					RawMesh.weightList.push_back( w );
				}
			}
		} // while (!feof (fp))
		fclose(fp);

		return true;
	}



	//------------------------------------------------------------------------------------------------------
	struct TVertexKey 
	{
		int	p,n,t;

		bool operator <(const struct TVertexKey& in) const
		{
			if (p == in.p)
			{
				if (n == in.n)
					return t < in.t;

				return n < in.n;
			}

			return p < in.p;
		}
	};
	typedef	std::map<TVertexKey, uint16>	UNIFIED_VERTEX_MAP;




	//------------------------------------------------------------------------------------------------------
	void CreateVertexBuffer(CResourceGeometry* pGeometry, TRAW_MESH* pRawMesh, UNIFIED_VERTEX_MAP& vertexMap)
	{
		pGeometry->vertexCount = vertexMap.size();

		if( pRawMesh->weightList.size() > 0 )
		{
			CVertexPNTW* pVertexBuf = new CVertexPNTW[ pGeometry->vertexCount ];
			pGeometry->eVertexType = FVF_3FP_4BN_2HT_4BW;

			for (UNIFIED_VERTEX_MAP::iterator itrVertex = vertexMap.begin(); itrVertex != vertexMap.end(); itrVertex++ )
			{
				CVertexPNTW* pVertex = &pVertexBuf[itrVertex->second];

				pVertex->vPos = pRawMesh->posList[itrVertex->first.p];
			
				CVector2 uv = pRawMesh->uvList[itrVertex->first.t];
				pVertex->vTex = XMHALF2( uv.x, uv.y );

				CVector3 normal = pRawMesh->normalList[itrVertex->first.n];
				pVertex->vNormal = XMBYTE4( normal.x, normal.y, normal.z, 0 );	
			
				RAW_WEIGHT& w = pRawMesh->weightList[itrVertex->first.p];

				if( w.count == 1 )
				{
					pVertex->fWeight = XMUBYTE4( w.weight[0], 0, 0, 0);
					pVertex->boneIDs = XMUBYTE4( w.bone[0], 0, 0, 0);
				}
				else if( w.count == 2 )
				{
					pVertex->fWeight = XMUBYTE4( w.weight[0], w.weight[1], 0, 0);
					pVertex->boneIDs = XMUBYTE4( w.bone[0], w.bone[1], 0, 0);
				}
				else if( w.count == 3 )
				{
					pVertex->fWeight = XMUBYTE4( w.weight[0], w.weight[1], w.weight[2], 0);
					pVertex->boneIDs = XMUBYTE4( w.bone[0], w.bone[1], w.bone[2], 0);
				}
				else if( w.count == 4 )
				{
					pVertex->fWeight = XMUBYTE4( w.weight[0], w.weight[1], w.weight[2], w.weight[3]);
					pVertex->boneIDs = XMUBYTE4( w.bone[0], w.bone[1], w.bone[2], w.bone[3]);
				}
			}

			pGeometry->pVertexBuffer = pVertexBuf;
		}
		else
		{
			CVertexHPNT* pVertexBuf = new CVertexHPNT[ pGeometry->vertexCount ];
			pGeometry->eVertexType = FVF_4HP_4BN_2HT;

			for (UNIFIED_VERTEX_MAP::iterator itrVertex = vertexMap.begin(); itrVertex != vertexMap.end(); itrVertex++ )
			{
				CVertexHPNT* pVertex = &pVertexBuf[itrVertex->second];

				CVector3& vec3 = pRawMesh->posList[itrVertex->first.p];
				pVertex->vPos = XMHALF4( vec3.x, vec3.y, vec3.z, 1);

				CVector2 uv = pRawMesh->uvList[itrVertex->first.t];
				pVertex->vTex = XMHALF2( uv.x, uv.y );

				CVector3 normal = pRawMesh->normalList[itrVertex->first.n];
				pVertex->vNormal.x = normal.x * 127;
				pVertex->vNormal.y = normal.y * 127;
				pVertex->vNormal.z = normal.z * 127;
			}

			pGeometry->pVertexBuffer = pVertexBuf;
		}
	}


	//------------------------------------------------------------------------------------------------------
	CResourceGeometry* CreateGeometry(TRAW_MESH* pRawMesh, std::vector<int>* pMtrlIDList )
	{
		UNIFIED_VERTEX_MAP vertexMap;
		IEngineMemoryMgr* pMemoryPoolMgr = GLOBAL::Engine()->EngineMemoryMgr();

		//////////////////////////////////////////////////////////////////////////
		// set geometry info
		CResourceGeometry* pGeometry = (CResourceGeometry*)pMemoryPoolMgr->GetNewResource(RESOURCE_GEOMETRY);
		strcpy_s( pGeometry->mtrlName, pRawMesh->mtrlList[0].c_str() );

		//////////////////////////////////////////////////////////////////////////
		// set index buffer info
		if( pMtrlIDList == NULL )
			pGeometry->primitiveCount = pRawMesh->facePos.size()/3;
		else
			pGeometry->primitiveCount = pMtrlIDList->size();

		pGeometry->eIndexType = INDEX_16BIT_TYPE;
		uint16* pData = new uint16[ pRawMesh->facePos.size() ];

		for ( UINT primitiveNum = 0; primitiveNum < pGeometry->primitiveCount; ++primitiveNum)
		{
			for( int j = 0; j < 3; ++j )
			{
				TVertexKey v;

				if( pMtrlIDList == NULL )
				{
					v.p = pRawMesh->facePos[ primitiveNum *3 + j ] -1;
					v.t = pRawMesh->faceUV[ primitiveNum *3 + j] - 1;
					v.n = pRawMesh->faceNormal[ primitiveNum *3 + j] -1;
				}
				else
				{
					int faceNum = (*pMtrlIDList)[primitiveNum] * 3;

					v.p = pRawMesh->facePos[ faceNum + j ] -1;
					v.t = pRawMesh->faceUV[ faceNum + j] - 1;
					v.n = pRawMesh->faceNormal[ faceNum + j] -1;
				}


				UNIFIED_VERTEX_MAP::iterator itr = vertexMap.find(v);

				if( itr == vertexMap.end() )
				{
					pData[ primitiveNum * 3 + j ] = vertexMap.size();
					vertexMap.insert( UNIFIED_VERTEX_MAP::value_type( v, vertexMap.size() ) );
				}
				else
				{				
					pData[ primitiveNum * 3 + j ] = itr->second;
				}
			}
		}

		pGeometry->pIndexBuffer = pData;
	

		//////////////////////////////////////////////////////////////////////////
		// set vertex buffer info
		CreateVertexBuffer(pGeometry, pRawMesh, vertexMap);

		return pGeometry;
	}

	void WriteString(std::fstream* file, std::string str )
	{
		uint8 strLength = str.length() + 1;
		file->write( (char*)&strLength, 1);

		if( strLength > 0 )
			file->write( (char*)str.c_str(), strLength);
	}

	//------------------------------------------------------------------------------------------------------
	void SaveMeshToFile(GEOMETRY_LIST& geometries, CVector3& min, CVector3& max, wxString name)
	{
		using namespace std;

		fstream file;
		file.open( name.char_str(), ios_base::out | ios_base::binary | ios_base::trunc );

		UINT version = MESH_FILE_VERSION;
		file.write( (char*)&version, 4);

		file.write( (char*)&min, 12);
		file.write( (char*)&max, 12);

		UINT size = geometries.size();
		file.write( (char*)&size, 1);

		for (UINT i=0; i < size; ++i)
		{
			CResourceGeometry* pGeometry = geometries[i];

			file.write( (char*)&pGeometry->eVertexType, 4);
			file.write( (char*)&pGeometry->vertexCount, 4);		
			file.write( (char*)pGeometry->pVertexBuffer, VERTEX_STRIDE(pGeometry->eVertexType) * pGeometry->vertexCount );

			file.write( (char*)&pGeometry->eIndexType, 4);
			file.write( (char*)&pGeometry->primitiveCount, 4);		

			if( pGeometry->eIndexType == INDEX_16BIT_TYPE)
				file.write( (char*)pGeometry->pIndexBuffer, 2 * pGeometry->primitiveCount * 3 );
			else
				file.write( (char*)pGeometry->pIndexBuffer, 4 * pGeometry->primitiveCount * 3 );

			WriteString( &file, pGeometry->mtrlName );
		}

		file.close();
	}


	//------------------------------------------------------------------------------------------------------
	void ImportRawMesh( TRAW_MESH* pRawMesh, wxString name )
	{
		GEOMETRY_LIST	vecGeometries;

		int mtrlCount = pRawMesh->faceMtrlID.size();

		if( mtrlCount == 0 )
		{
			CResourceGeometry* pGeometry = CreateGeometry(pRawMesh , NULL);
			vecGeometries.push_back(pGeometry);
		}
		else // split mesh per material
		{
			for( int i = 0; i < mtrlCount; ++i)
			{
				MTRL_FACE_MAP::iterator itr = pRawMesh->faceMtrlID.find(i);

				std::vector<int>* pMtrlIDList = itr->second;
				if( pMtrlIDList->size() == 0 )
					continue;

				CResourceGeometry* pGeometry = CreateGeometry(pRawMesh , pMtrlIDList);
				vecGeometries.push_back(pGeometry);
			}
		}

		SaveMeshToFile(vecGeometries, pRawMesh->BBoxMin, pRawMesh->BBoxMax, name);

		for (UINT i=0; i < vecGeometries.size(); ++i)
		{
			GLOBAL::Engine()->EngineMemoryMgr()->RemoveResource( vecGeometries[i] );
		}
	}
}
