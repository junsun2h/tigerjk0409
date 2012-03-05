#include <iostream>
#include <fstream>

#include "wx/wx.h"

#include "CDefine.h"
#include "CResource.h"

#include "SRawResourceLoader.h"


namespace SRAW_FILE_LOADER
{
	typedef std::vector<CResourceGeometry*>	GEOMETRY_LIST;

	SRAW_MESH::SRAW_MESH()
		: BBoxMin(FLT_MAX, FLT_MAX, FLT_MAX)
		, BBoxMax(-FLT_MAX, -FLT_MAX, -FLT_MAX)
		, coordSys(COODSYS_3DSMAX)
	{
	}

	SRAW_MESH::~SRAW_MESH()
	{
		MTRL_FACE_MAP::iterator itr = faceMtrlID.begin();
		while (itr != faceMtrlID.end())
		{
			SAFE_DELETE(itr->second);
			itr++;
		}
	}

	void SRAW_MESH::ChangeCoordsys(TCOODSYS coodSys_)
	{
		// max coordinate -> directx coordinate
		if( coordSys == COODSYS_3DSMAX && coodSys_ == COODSYS_DIRECTX )
		{
			coordSys = coodSys_;

			int posCount = posList.size();
			for( int i=0; i < posCount; ++i)
				posList[i].y = -posList[i].y;

			int normalCount = normalList.size();
			for( int i=0; i < normalCount; ++i)
				normalList[i].y = -normalList[i].y;

			BBoxMin.y = -BBoxMin.y;
			BBoxMax.y = -BBoxMax.y;

			TSWAP( BBoxMin.y, BBoxMax.y );

			postion.y = -postion.y;

			rotation.x *= -1;
			rotation.y *= -1;
			rotation.z *= -1;

			int uvCount = uvList.size();
			for( int i=0; i < uvCount; ++i)
				uvList[i].y = 1 - uvList[i].y;


			int faceCount = facePos.size();
			for( int i=0; i < faceCount; i += 3)
			{
				TSWAP( facePos[i], facePos[i+2] );
				TSWAP( faceNormal[i], faceNormal[i+2] );
				TSWAP( faceUV[i], faceUV[i+2] );
			}
		}
	}

	void SRAW_ACTOR::ChangeCoordsys(TCOODSYS coodSys_)
	{
		if( coordSys == COODSYS_3DSMAX && coodSys_ == COODSYS_DIRECTX )
		{
			for( UINT i= 0; i < joints.size(); ++i)
			{
				joints[i].pos.y = -joints[i].pos.y;
				joints[i].rot.y = -joints[i].rot.y;
			}
		}
	}

	void SRAW_MOTION::ChangeCoordsys(TCOODSYS coodSys_)
	{
		if( coordSys == COODSYS_3DSMAX && coodSys_ == COODSYS_DIRECTX )
		{
			for( UINT i = 0; i< keys.size(); ++i)
			{
				std::vector<RAW_MOTION_KEY>& keyList = keys[i];
				for( UINT iKeys = 0; iKeys < keyList.size(); ++iKeys)
				{
					keyList[iKeys].pos.y = -keyList[iKeys].pos.y;
					keyList[iKeys].rot.y = -keyList[iKeys].rot.y;
				}
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	bool LoadRawActor(const char* strFileName, SRAW_ACTOR& rawActor)
	{
		char delimiters[2] = { '/', 0 };
		char* pContext = NULL;

#define READ_BUFFER_SIZE 256
		FILE *fp;

		char buf[READ_BUFFER_SIZE];
		if( fopen_s( &fp, strFileName, "rb") != 0)
			return false;

		while (!feof (fp))
		{
			fgets (buf, sizeof (buf), fp);
		
			if( strncmp( buf, "mesh", 4) == 0 )
			{
				rawActor.meshNames.push_back( strtok_s( &buf[4], delimiters , &pContext ) );
			}
			else if( strncmp( buf, "node", 4) == 0 )
			{
				RAW_ACTOR_NODE transform;

				transform.name = strtok_s( &buf[4], delimiters , &pContext );
				transform.parentName = strtok_s( NULL, delimiters , &pContext );

				fgets (buf, sizeof (buf), fp);

				transform.pos.x = (float)atof( strtok_s( buf, delimiters , &pContext ) );				
				transform.pos.y = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	
				transform.pos.z = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	

				transform.rot.x = (float)atof( strtok_s( NULL, delimiters , &pContext ) );				
				transform.rot.y = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	
				transform.rot.z = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	
				transform.rot.w = (float)atof( strtok_s( NULL, delimiters , &pContext ) );

				rawActor.joints.push_back( transform);
			}
		}

		fclose(fp);

		return true;
	}

	//------------------------------------------------------------------------------------------------------
	bool LoadRawMotion(const char* strFileName, SRAW_MOTION& rawMotion)
	{
		char delimiters[2] = { '/', 0 };
		char* pContext = NULL;

#define READ_BUFFER_SIZE 256
		FILE *fp;

		char buf[READ_BUFFER_SIZE];
		if( fopen_s( &fp, strFileName, "rb") != 0)
			return false;

		while (!feof (fp))
		{
			fgets (buf, sizeof (buf), fp);

			if( strncmp( buf, "framerate",  9) == 0 )
			{
				rawMotion.frameRate = atoi( &buf[10] );
			}
			else if( strncmp( buf, "totalFrame", 10) == 0 )
			{
				rawMotion.totalFrame = atoi( &buf[11] );
			}
			else if( strncmp( buf, "bakeInterval", 12) == 0 )
			{
				rawMotion.frameInterval = atoi( &buf[13] );
			}
			if( strncmp( buf, "node", 4) == 0 )
			{
				RAW_MOTION_NODE motion;

				motion.name = strtok_s( &buf[4], delimiters , &pContext );
				motion.parentName = strtok_s( NULL, delimiters , &pContext );
				rawMotion.joints.push_back(motion);

				std::vector<RAW_MOTION_KEY> keys;

				int keyCount = rawMotion.totalFrame/rawMotion.frameInterval + 1;
				if( float(rawMotion.totalFrame)/rawMotion.frameInterval > rawMotion.totalFrame/rawMotion.frameInterval )
					keyCount++;

				for( int i=0; i < keyCount; ++i)
				{
					RAW_MOTION_KEY key;

					fgets (buf, sizeof (buf), fp);

					key.pos.x = (float)atof( strtok_s( buf, delimiters , &pContext ) );				
					key.pos.y = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	
					key.pos.z = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	

					key.rot.x = (float)atof( strtok_s( NULL, delimiters , &pContext ) );				
					key.rot.y = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	
					key.rot.z = (float)atof( strtok_s( NULL, delimiters , &pContext ) );	
					key.rot.w = (float)atof( strtok_s( NULL, delimiters , &pContext ) );

					keys.push_back( key);
				}

				rawMotion.keys.push_back(keys);
			}
		}

		fclose(fp);

		return true;
	}

	//------------------------------------------------------------------------------------------------------
	bool LoadRawMesh(const char* strFileName, SRAW_MESH& RawMesh)
	{
		char delimiters[2] = { '/', 0 };
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
					fgets (buf, sizeof (buf), fp);
					RawMesh.mtrlList.push_back( strtok_s( buf, delimiters , &pContext ) );
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
			else if( strncmp( buf, "usingBone", 9 ) == 0 )
			{
				int count = atoi( &buf[10] );
				RawMesh.weightBoneList.reserve( count );
				for( int i=0; i < count; ++i )
				{
					fgets (buf, sizeof (buf), fp);
					RawMesh.weightBoneList.push_back( strtok_s( buf, delimiters , &pContext ) );
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

					int count = atoi( strtok_s( buf, delimiters , &pContext ) );
					for( int wi=0; wi < count; ++ wi)
					{
						if( wi > 3 )
							break;

						uint8 Index = atoi( strtok_s( NULL, delimiters , &pContext ) );
						Index--;

						float weight = (float)atof( strtok_s( NULL, delimiters , &pContext ) );
						
						if( weight < 0.01f )
							continue;

						w.bone[w.count] = Index;
						w.weight[w.count] = weight;

						w.count++;
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
	void CreateVertexBuffer(CResourceGeometry* pGeometry, SRAW_MESH* pRawMesh, UNIFIED_VERTEX_MAP& vertexMap)
	{
		pGeometry->vertexCount = vertexMap.size();

		if( pRawMesh->weightList.size() > 0 )
		{
			CVertexHPNTW* pVertexBuf = new CVertexHPNTW[ pGeometry->vertexCount ];
			pGeometry->eVertexType = FVF_4HP_4BN_2HT_4BW;

			for (UNIFIED_VERTEX_MAP::iterator itrVertex = vertexMap.begin(); itrVertex != vertexMap.end(); itrVertex++ )
			{
				CVertexHPNTW* pVertex = &pVertexBuf[itrVertex->second];

				CVector3& vec3 = pRawMesh->posList[itrVertex->first.p];
				CVector2 uv = pRawMesh->uvList[itrVertex->first.t];
				CVector3 normal = pRawMesh->normalList[itrVertex->first.n];

				pVertex->vPos = XMHALF4( vec3.x, vec3.y, vec3.z, uv.x);
				pVertex->vNormal = XMHALF4( normal.x, normal.y, normal.z, uv.y);

				RAW_WEIGHT& w = pRawMesh->weightList[itrVertex->first.p];

				pVertex->fWeight.x = w.weight[0] * 255;
				pVertex->fWeight.y = w.weight[1] * 255;
				pVertex->fWeight.z = w.weight[2] * 255;
				pVertex->fWeight.w = w.weight[3] * 255;
				pVertex->boneIDs = XMUBYTE4( w.bone[0], w.bone[1], w.bone[2], w.bone[3]);
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
				CVector2 uv = pRawMesh->uvList[itrVertex->first.t];
				CVector3 normal = pRawMesh->normalList[itrVertex->first.n];

				pVertex->vPos = XMHALF4( vec3.x, vec3.y, vec3.z, uv.x);
				pVertex->vNormal = XMHALF4( normal.x, normal.y, normal.z, uv.y);
			}

			pGeometry->pVertexBuffer = pVertexBuf;
		}
	}
	
	//------------------------------------------------------------------------------------------------------
	void CreateGeometry(CResourceGeometry* pGeometry, SRAW_MESH* pRawMesh, std::vector<int>* pMtrlIDList )
	{
		UNIFIED_VERTEX_MAP vertexMap;

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
	}

	void WriteString(std::fstream* file, std::string str )
	{
		uint8 strLength = str.length() + 1;
		file->write( (char*)&strLength, 1);

		if( strLength > 0 )
			file->write( (char*)str.c_str(), strLength);
	}

	//------------------------------------------------------------------------------------------------------
	void SaveMeshToFile(GEOMETRY_LIST& geometries, SRAW_MESH* pRawMesh, wxString name)
	{
		using namespace std;

		fstream file;
		file.open( name.char_str(), ios_base::out | ios_base::binary | ios_base::trunc );

		UINT version = MESH_FILE_VERSION;
		file.write( (char*)&version, 4);

		file.write( (char*)&pRawMesh->BBoxMin, 12);
		file.write( (char*)&pRawMesh->BBoxMax, 12);

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

		UINT boneCount = pRawMesh->weightBoneList.size();
		if( boneCount > 0 )
		{
			file.write( (char*)&boneCount, 1);
			for( UINT i=0; i < boneCount; ++i)
				WriteString( &file, pRawMesh->weightBoneList[i].c_str() );
		}

		file.close();
	}


	//------------------------------------------------------------------------------------------------------
	void SaveRawMeshToFile( SRAW_MESH* pRawMesh, wxString name )
	{
		GEOMETRY_LIST	vecGeometries;

		int mtrlCount = pRawMesh->faceMtrlID.size();

		if( mtrlCount == 0 )
		{
			//////////////////////////////////////////////////////////////////////////
			// set geometry info
			CResourceGeometry* pGeometry = new (_alloca( sizeof(CResourceGeometry))) CResourceGeometry;
			strcpy_s( pGeometry->mtrlName, pRawMesh->mtrlList[0].c_str() );

			CreateGeometry(pGeometry, pRawMesh , NULL);
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

				CResourceGeometry* pGeometry = new (_alloca( sizeof(CResourceGeometry))) CResourceGeometry;
				strcpy_s( pGeometry->mtrlName, pRawMesh->mtrlList[i].c_str() );

				CreateGeometry(pGeometry, pRawMesh , pMtrlIDList);
				vecGeometries.push_back(pGeometry);
			}
		}

		SaveMeshToFile(vecGeometries, pRawMesh, name);
	}

	//------------------------------------------------------------------------------------------------------
	void SaveRawActorToFile( SRAW_ACTOR* pRawActor, wxString name )
	{
		std::vector<CJoint>		jointList;

		for( UINT i= 0; i < pRawActor->joints.size(); ++i)
		{
			CJoint joint;

			strcpy_s( joint.name , pRawActor->joints[i].name.c_str() );
			strcpy_s( joint.parentName , pRawActor->joints[i].parentName.c_str() );

			joint.pos = pRawActor->joints[i].pos;
			joint.rot = pRawActor->joints[i].rot;
			joint.parentIndex = -1;

			// find parent index
			if( strcmp( joint.parentName, "NONE" ) != 0 )
			{
				for( UINT iJoint = 0; iJoint < jointList.size(); ++iJoint )
				{
					if( strcmp( jointList[iJoint].name, joint.parentName) == 0 )
					{
						joint.parentIndex = iJoint;
						break;
					}
				}

				if( joint.parentIndex == -1)
					assert(0);
			}

			jointList.push_back(joint);
		}

		//////////////////////////////////////////////////////////////////////////
		// save actor as binary file
		using namespace std;

		fstream file;
		file.open( name.char_str(), ios_base::out | ios_base::binary | ios_base::trunc );

		UINT version = ACTOR_FILE_VERSION;
		file.write( (char*)&version, 4);

		// mesh info
		uint8 meshSize = pRawActor->meshNames.size();
		file.write( (char*)&meshSize, 1);

		for (UINT i=0; i < meshSize; ++i)
		{
			WriteString( &file, pRawActor->meshNames[i].c_str() );
		}

		// joint info
		uint8 size = jointList.size();
		file.write( (char*)&size, 1);

		for (UINT i=0; i < size; ++i)
		{
			CJoint& joint = jointList[i];

			file.write( (char*)&joint, 32);

			WriteString( &file, joint.name );
			WriteString( &file, joint.parentName );
		}

		// motion info
		uint8 motionList = 0;
		file.write( (char*)&motionList, 1);

		file.close();
	}

	void SaveRawMotionToFile( SRAW_MOTION* pRawMotion, wxString name )
	{
		MOTION_NODE_LIST	jointList;

		for( UINT i= 0; i < pRawMotion->joints.size(); ++i)
		{
			CMotionNode joint;
			joint.bStaticNode = true;

			strcpy_s( joint.name , pRawMotion->joints[i].name.c_str() );
			strcpy_s( joint.parentName , pRawMotion->joints[i].parentName.c_str() );

			std::vector<RAW_MOTION_KEY>* pKeys = &pRawMotion->keys[i];

			// check this node doesn't change whole playing time
			for( UINT iKey =1; iKey < pKeys->size(); ++iKey)
			{
				RAW_MOTION_KEY& key = (*pKeys)[iKey];
				RAW_MOTION_KEY& formerKey = (*pKeys)[iKey - 1 ];

				if( CVector3::EQual( formerKey.pos , key.pos, 0.1f ) == false ||
					CQuat::EQual( formerKey.rot, key.rot, 0.001f ) == false )
				{
					joint.bStaticNode = false;
					break;
				}
			}

			if( joint.bStaticNode )
			{
				CMotionKey key;
				key.pos = (*pKeys)[0].pos;
				key.rot = (*pKeys)[0].rot;
				key.posIndex = 0;
				key.rotIndex = 0;
				joint.keys.push_back(key);
			}
			else
			{
				for( UINT iKey =0; iKey < pKeys->size(); ++iKey)
				{
					CMotionKey key;
					key.pos = (*pKeys)[iKey].pos;
					key.rot = (*pKeys)[iKey].rot;
					key.posIndex = iKey;
					key.rotIndex = iKey;

					if( iKey > 0 )
					{
						CMotionKey& formerKey = joint.keys[iKey - 1 ];
						if( CVector3::EQual( formerKey.pos , key.pos, 0.1f ) )
							key.posIndex = formerKey.posIndex;

						if( CQuat::EQual( formerKey.rot, key.rot, 0.001f ) )
							key.rotIndex = formerKey.rotIndex;
					}

					joint.keys.push_back(key);
				}
			}
			
			jointList.push_back(joint);
		}

		//////////////////////////////////////////////////////////////////////////
		// save actor as binary file
		using namespace std;

		fstream file;
		file.open( name.char_str(), ios_base::out | ios_base::binary | ios_base::trunc );

		UINT version = MOTION_FILE_VERSION;
		file.write( (char*)&version, 4);

		file.write( (char*)&pRawMotion->frameRate, 1);
		file.write( (char*)&pRawMotion->frameInterval, 1);
		file.write( (char*)&pRawMotion->totalFrame, 4);

		uint8 size = jointList.size();
		file.write( (char*)&size, 1);

		for (UINT i=0; i < size; ++i)
		{
			CMotionNode& joint = jointList[i];

			WriteString( &file, joint.name );
			WriteString( &file, joint.parentName );

			file.write( (char*)&joint.bStaticNode, 1);

			if( joint.bStaticNode )
			{
				file.write( (char*)&joint.keys[0].pos, sizeof(CVector3));
				file.write( (char*)&joint.keys[0].rot, sizeof(CQuat));
				continue;
			}

			for( UINT iKey = 0; iKey < joint.keys.size(); ++iKey )
			{	
				file.write( (char*)&joint.keys[iKey].posIndex, 1);
				if( joint.keys[iKey].posIndex == iKey)
					file.write( (char*)&joint.keys[iKey].pos, sizeof(CVector3));

				file.write( (char*)&joint.keys[iKey].rotIndex, 1);
				if( joint.keys[iKey].rotIndex == iKey)
					file.write( (char*)&joint.keys[iKey].rot, sizeof(CQuat));
			}
		}

		file.close();
	}

	void SaveActorToFile( const CResourceActor* pActor, wxString fullPath )
	{
		//////////////////////////////////////////////////////////////////////////
		// save actor as binary file
		using namespace std;

		fstream file;
		file.open( fullPath.char_str(), ios_base::out | ios_base::binary | ios_base::trunc );

		UINT version = ACTOR_FILE_VERSION;
		file.write( (char*)&version, 4);

		// mesh info
		uint8 meshSize = pActor->meshList.size();
		file.write( (char*)&meshSize, 1);

		for (UINT i=0; i < meshSize; ++i)
		{
			WriteString( &file, pActor->meshList[i]->name );
		}

		// joint Info
		uint8 jointSize = pActor->jointList.size();
		file.write( (char*)&jointSize, 1);

		for (UINT i=0; i < jointSize; ++i)
		{
			const CJoint& joint = pActor->jointList[i];

			file.write( (char*)&joint, 32);

			WriteString( &file, joint.name );
			WriteString( &file, joint.parentName );
		}

		// motion Info
		uint8 motionSize = pActor->motionList.size();
		file.write( (char*)&motionSize, 1);
		for (UINT i=0; i < motionSize; ++i)
		{
			WriteString( &file, pActor->motionList[i]->name );
		}

		file.close();
	}
}
