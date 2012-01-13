#pragma once

#include "CDefine.h"
#include <string>
#include <locale>
#include <atlcoll.h>

#define MAX_MESH_LOD	5
#define MAX_GEOMETRY	5
#define MAX_NAME		64


enum RESOURCE_TYPE
{
	RESOURCE_VERTEX,
	RESOURCE_INDEX,
	RESOURCE_GEOMETRY,
	RESOURCE_TEXTURE,
	RESOURCE_MESH,	
	RESOURCE_LOD_MESH,
	RESOURCE_ACTOR,	
	RESOURCE_MOTION,
	RESOURCE_MATERIAL,
	RESOURCE_SHADER,

	NUM_RESOURCE_TYPE,

	RESOURCE_INVALID
};


enum RESOURCE_FILE_TYPE
{
	RESOURCE_FILE_TEXTURE,
	RESOURCE_FILE_ACTOR,
	RESOURCE_FILE_MESH,
	RESOURCE_FILE_MOTION,
	RESOURCE_FILE_MATERIAL,
	RESOURCE_FILE_SHADER,

	NUM_RESOURCE_FILE_TYPE
};


enum DEVICE_DEPENDENT_RESOURCE
{
	DP_RESOURCE_VERTEX,
	DP_RESOURCE_VERTEX_OUT,
	DP_RESOURCE_INDEX,
	DP_RESOURCE_TEXTURE,
	DP_RESOURCE_SHADER,

	NUM_DP_RESOURCE
};

enum RESOURCE_LOAD_STATE
{
	RESOURCE_LOAD_NONE,
	RESOURCE_LOAD_STARTED,
	RESOURCE_LOAD_FINISHED
};

enum TEXTURE_TYPE
{
	PARAM_DIFFISE,
	PARAM_SPECULAR,
	PARAM_NORMAL,
	PARAM_TRANSPARENCY,

	NUM_TEXTURE_TYPE
};


enum CVERTEX_TYPE
{
	FVF_3FP,
	FVF_3FP_1DC,		// 3 float position + 1 DWORD Color
	FVF_3FP_1DC_2HT,
	FVF_3FP_4BN_2HT,
	FVF_3FP_4BN_2HT_4BW,
};


struct CVertexPC 
{
	CVector3		vPos;
	uint32			color;

	size_t			Size()	{ return 16; }
};

struct CVertexPCT
{
	CVector3	vPos;
	DWORD		vColor;
	XMHALF2		vTex;

	size_t			Size()	{ return 20; }
};

struct CVertexPNT
{
	CVector3	vPos;
	XMBYTE4		vNormal;
	XMHALF2		vTex;

	size_t			Size()	{ return 20; }
};

struct CVertexPNTW
{
	CVector3	vPos;
	XMBYTE4		vNormal;
	XMHALF2		vTex;
	XMUBYTE4	boneIDs;
	XMUBYTE4	fWeight;

	size_t			Size()	{ return 28; }
};


enum CINDEX_TYPE
{
	INDEX_16BIT_TYPE,
	INDEX_32BIT_TYPE
};


struct CResourceBase
{
	long					RID;	// resource ID, hash Key fron resource name
	RESOURCE_LOAD_STATE		state;
	double					loadBeginTime;
	char					name[MAX_NAME];

	virtual RESOURCE_TYPE	Type()	{ return RESOURCE_INVALID; }
	virtual std::string		strType()  { return ""; }
	std::string				strLoadState()	{ return ENUMSTR(RESOURCE_LOAD_STATE(state)); }

	CResourceBase()
		: RID(-1)
		, state(RESOURCE_LOAD_NONE)
		, loadBeginTime(0)
	{
	}
	virtual ~CResourceBase(){}
};


struct CResourceTexture : CResourceBase
{
	RESOURCE_TYPE	Type() override { return RESOURCE_TEXTURE; }
	std::string		strType() override { return ENUMSTR(RESOURCE_TEXTURE); }
};


struct CResourceVB : CResourceBase
{
	CVERTEX_TYPE	eType;
	UINT			count;
	void*			pData;

	RESOURCE_TYPE	Type() override { return RESOURCE_VERTEX; }
	std::string		strType() override { return ENUMSTR(RESOURCE_VERTEX); }

	~CResourceVB()
	{
		SAFE_DELETE_ARRAY(pData);
	}
};


struct CResourceIB : CResourceBase
{
	CINDEX_TYPE		eType;
	UINT			primitiveCount;
	void*			pData;

	RESOURCE_TYPE	Type() override { return RESOURCE_INDEX; }
	std::string		strType() override { return ENUMSTR(RESOURCE_INDEX); }

	~CResourceIB()
	{
		SAFE_DELETE_ARRAY(pData);
	}
};

struct CResourceGeometry : CResourceBase
{
	long		vertexBuffer;
	long		indexBuffer;
	long		defaultMtrl;

	char	mtrlName[MAX_NAME];

	CResourceGeometry()
		: vertexBuffer(-1)
		, indexBuffer(-1)
		, defaultMtrl(-1)
	{
	}

	RESOURCE_TYPE	Type() override { return RESOURCE_GEOMETRY; }
	std::string		strType() override { return ENUMSTR(RESOURCE_GEOMETRY); }
};

struct CResourceMesh : CResourceBase
{
	uint8	geometryNum;
	long	goemetries[MAX_GEOMETRY];

	RESOURCE_TYPE	Type() override { return RESOURCE_MESH; }
	std::string		strType() override { return ENUMSTR(RESOURCE_MESH); }
};

struct CResourceLODMesh : CResourceBase
{
	uint8	LOD;
	long	RID_mesh[MAX_MESH_LOD];

	RESOURCE_TYPE	Type() override { return RESOURCE_LOD_MESH; }
	std::string		strType() override { return ENUMSTR(RESOURCE_LOD_MESH); }
};

struct CResourceActor : CResourceBase
{
	RESOURCE_TYPE	Type() override { return RESOURCE_ACTOR; }
	std::string		strType() override { return ENUMSTR(RESOURCE_ACTOR); }
};

struct CResourceMotion : CResourceBase
{
	RESOURCE_TYPE	Type() override { return RESOURCE_MOTION; }
	std::string		strType() override { return ENUMSTR(RESOURCE_MOTION); }
};

struct CResourceMtrl : CResourceBase
{
	long		RID_textures[NUM_TEXTURE_TYPE];

	RESOURCE_TYPE	Type() override { return RESOURCE_MATERIAL; }
	std::string		strType() override { return ENUMSTR(RESOURCE_MATERIAL); }
};

struct CResourceShader : CResourceBase
{
	RESOURCE_TYPE	Type() override { return RESOURCE_SHADER; }
	std::string		strType() override { return ENUMSTR(RESOURCE_SHADER); }
};

typedef void    (CALLBACK *CALLBACK_LOAD_COMPLED)();
typedef	ATL::CAtlMap<long, CResourceBase*>	TYPE_RESOURCE_MAP;

struct IAssetMgr
{
	virtual long				LoadCompletedResource( CResourceBase* pResource) = 0;
	virtual long				Load(char* fileName, RESOURCE_FILE_TYPE type, CALLBACK_LOAD_COMPLED pCallback = NULL, bool bAsync = true) = 0;
 	virtual void				Clear() = 0;
 	virtual void				Remove(RESOURCE_TYPE type, long id) = 0;
	virtual void				Remove(RESOURCE_TYPE type, std::string& name) = 0;

	virtual const CResourceBase*		GetResource( RESOURCE_TYPE type, long id ) = 0;
	virtual const CResourceBase*		GetResource( RESOURCE_TYPE type, std::string name ) = 0;
	virtual const TYPE_RESOURCE_MAP*	GetResources( RESOURCE_TYPE type ) = 0;
};


inline long GET_HASH_KEY( std::string name )
{
	std::locale loc;
	const std::collate<char>& coll = std::use_facet<std::collate<char> >(loc);

	long hashKey = coll.hash(name.data(), name.data() + name.length());

	return hashKey;
}