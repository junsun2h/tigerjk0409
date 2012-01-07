#pragma once

#include "CDefine.h"
#include <string>
#include <locale>

#define MAX_MESH_LOD	5
#define MAX_GEOMETRY	5
#define MAX_NAME		64


enum RESOURCE_TYPE
{
	RESOURCE_VERTEX,
	RESOURCE_INDEX,
	RESOURCE_TEXTURE,		// URID Resource, unique resource ID generated from file name
	RESOURCE_GEOMETRY,
	RESOURCE_MESH,			// URID Resource 
	RESOURCE_LOD_MESH,				
	RESOURCE_ACTOR,			// URID Resource 
	RESOURCE_MOTION,		// URID Resource 
	RESOURCE_MATERIAL,		// URID Resource 
	RESOURCE_SHADER,		// URID Resource 

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
	FVF_3FP_1DC_2FT,
};


struct CVertexPC 
{
	CVector3		vPos;
	uint32			color;
};

struct CVertexPCT
{
	CVector3 vPos;
	DWORD vColor;
	XMHALF2 vTex;
};

enum CINDEX_TYPE
{
	INDEX_16BIT_TYPE,
	INDEX_32BIT_TYPE
};


struct IResource
{
	long					RID;	// resource ID, hash Key fron resource name
	RESOURCE_LOAD_STATE		state;
	double					loadBeginTime;
	char					name[MAX_NAME];

	virtual RESOURCE_TYPE	Type()	{ return RESOURCE_INVALID; }
	virtual std::string		strType()  { return ""; }
	std::string				strLoadState()	{ return ENUMSTR(RESOURCE_LOAD_STATE(state)); }

	IResource()
		: RID(-1)
		, state(RESOURCE_LOAD_NONE)
		, loadBeginTime(0)
	{
	}
};


struct CResourceTexture : IResource
{
	RESOURCE_TYPE	Type() override { return RESOURCE_TEXTURE; }
	std::string		strType() override { return ENUMSTR(RESOURCE_TEXTURE); }
};


struct CResourceVB : IResource
{
	CVERTEX_TYPE	eType;
	UINT			count;

	RESOURCE_TYPE	Type() override { return RESOURCE_VERTEX; }
	std::string		strType() override { return ENUMSTR(RESOURCE_VERTEX); }
};


struct CResourceIB : IResource
{
	CINDEX_TYPE		eType;
	UINT			count;

	RESOURCE_TYPE	Type() override { return RESOURCE_INDEX; }
	std::string		strType() override { return ENUMSTR(RESOURCE_INDEX); }
};

struct CResourceGeometry : IResource
{
	long		RID_vb;
	long		RID_ib;
	long		RID_mtrl;

	char	mtrlName[MAX_NAME];

	RESOURCE_TYPE	Type() override { return RESOURCE_GEOMETRY; }
	std::string		strType() override { return ENUMSTR(RESOURCE_GEOMETRY); }
};

struct CResourceMesh : IResource
{
	uint8	geometryNum;
	long	RID_goemetry[MAX_GEOMETRY];

	RESOURCE_TYPE	Type() override { return RESOURCE_MESH; }
	std::string		strType() override { return ENUMSTR(RESOURCE_MESH); }
};

struct CResourceLODMesh : IResource
{
	uint8	LOD;
	long	RID_mesh[MAX_MESH_LOD];

	RESOURCE_TYPE	Type() override { return RESOURCE_LOD_MESH; }
	std::string		strType() override { return ENUMSTR(RESOURCE_LOD_MESH); }
};

struct CResourceActor : IResource
{
	RESOURCE_TYPE	Type() override { return RESOURCE_ACTOR; }
	std::string		strType() override { return ENUMSTR(RESOURCE_ACTOR); }
};

struct CResourceMotion : IResource
{
	RESOURCE_TYPE	Type() override { return RESOURCE_MOTION; }
	std::string		strType() override { return ENUMSTR(RESOURCE_MOTION); }
};

struct CResourceMtrl : IResource
{
	long		RID_textures[NUM_TEXTURE_TYPE];

	RESOURCE_TYPE	Type() override { return RESOURCE_MATERIAL; }
	std::string		strType() override { return ENUMSTR(RESOURCE_MATERIAL); }
};

struct CResourceShader : IResource
{
	RESOURCE_TYPE	Type() override { return RESOURCE_SHADER; }
	std::string		strType() override { return ENUMSTR(RESOURCE_SHADER); }
};

typedef void    (CALLBACK *CALLBACK_LOAD_COMPLED)();

struct IAssetMgr
{
	virtual void				LoadCompletedResource( IResource* pResource) = 0;
	virtual long				Load(char* fileName, RESOURCE_FILE_TYPE type, CALLBACK_LOAD_COMPLED pCallback = NULL, bool bAsync = true) = 0;
 	virtual void				Clear() = 0;

	virtual const IResource*	GetResource( RESOURCE_TYPE type, long id ) = 0;
	virtual const IResource*	GetResource( RESOURCE_TYPE type, std::string name ) = 0;
};


inline long GET_HASH_KEY( std::string name )
{
	std::locale loc;
	const std::collate<char>& coll = std::use_facet<std::collate<char> >(loc);

	long hashKey = coll.hash(name.data(), name.data() + name.length());

	return hashKey;
}