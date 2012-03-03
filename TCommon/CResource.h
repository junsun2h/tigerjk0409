#pragma once

#include "CVertex.h"
#include "CUnitPool.h"
#include <string>
#include <locale>


#define MESH_FILE_VERSION	1
#define ACTOR_FILE_VERSION	1
#define MOTION_FILE_VERSION	1

#define MAX_GEOMETRY	5
#define MAX_NAME_LENGTH		64


enum eIMAGE_FILE_FORMAT
{
	IMG_FILE_BMP         = 0,
	IMG_FILE_JPG         = 1,
	IMG_FILE_PNG         = 3,
	IMG_FILE_DDS         = 4,
	IMG_FILE_TIFF		  = 10,
	IMG_FILE_GIF		  = 11,
	IMG_FILE_WMP		  = 12,
	IMG_FILE_FORCE_DWORD = 0x7fffffff
};


enum eRESOURCE_TYPE
{
	RESOURCE_GEOMETRY,
	RESOURCE_TEXTURE,
	RESOURCE_MESH,
	RESOURCE_ACTOR,	
	RESOURCE_MOTION,
	RESOURCE_MATERIAL,

	NUM_RESOURCE_TYPE,

	RESOURCE_INVALID
};


enum eRESOURCE_FILE_TYPE
{
	RESOURCE_FILE_TEXTURE,
	RESOURCE_FILE_ACTOR,
	RESOURCE_FILE_MESH,
	RESOURCE_FILE_MOTION,
	RESOURCE_FILE_MATERIAL,

	NUM_RESOURCE_FILE_TYPE,

	RESOURCE_FILE_INVALID
};

enum eRESOURCE_LOAD_STATE
{
	RESOURCE_LOAD_NONE,
	RESOURCE_MEMORY_CREATED,
	RESOURCE_LOAD_STARTED,
	RESOURCE_LOAD_FINISHED,
	RESOURCE_LOAD_FINISHED_WAIT_FOR_SUB_RSC,
	RESOURCE_LOAD_FAILED
};



enum eCINDEX_TYPE
{
	INDEX_16BIT_TYPE,
	INDEX_32BIT_TYPE,

	INDEX_INVALID
};


struct CResourceBase
{
	long					RID;	// resource ID, hash Key fron resource name
	eRESOURCE_LOAD_STATE	loadState;
	double					loadBeginTime;
	char					name[MAX_NAME_LENGTH];

	virtual eRESOURCE_TYPE	Type() const	{ return RESOURCE_INVALID; }
	virtual std::string		strType()  { return ""; }
	std::string				strLoadState()	{ return ENUMSTR(eRESOURCE_LOAD_STATE(loadState)); }
	
	CResourceBase()
		: RID(-1)
		, loadState(RESOURCE_LOAD_NONE)
		, loadBeginTime(0)
	{
	}
	virtual ~CResourceBase(){}
};

//-------------------------------------------------------------------------------------------------
enum eTEXTURE_USAGE
{
	TEXTURE_NORMAL,
	TEXTURE_RENDER_RAGET,
};

enum eTEXTURE_FORMAT
{
	COLOR_FORMAT_UNKNOWN	                 = 0,
	COLOR_FORMAT_R32G32B32A32_TYPELESS       = 1,
	COLOR_FORMAT_R32G32B32A32_FLOAT          = 2,
	COLOR_FORMAT_R32G32B32A32_UINT           = 3,
	COLOR_FORMAT_R32G32B32A32_SINT           = 4,
	COLOR_FORMAT_R32G32B32_TYPELESS          = 5,
	COLOR_FORMAT_R32G32B32_FLOAT             = 6,
	COLOR_FORMAT_R32G32B32_UINT              = 7,
	COLOR_FORMAT_R32G32B32_SINT              = 8,
	COLOR_FORMAT_R16G16B16A16_TYPELESS       = 9,
	COLOR_FORMAT_R16G16B16A16_FLOAT          = 10,
	COLOR_FORMAT_R16G16B16A16_UNORM          = 11,
	COLOR_FORMAT_R16G16B16A16_UINT           = 12,
	COLOR_FORMAT_R16G16B16A16_SNORM          = 13,
	COLOR_FORMAT_R16G16B16A16_SINT           = 14,
	COLOR_FORMAT_R32G32_TYPELESS             = 15,
	COLOR_FORMAT_R32G32_FLOAT                = 16,
	COLOR_FORMAT_R32G32_UINT                 = 17,
	COLOR_FORMAT_R32G32_SINT                 = 18,
	COLOR_FORMAT_R32G8X24_TYPELESS           = 19,
	COLOR_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
	COLOR_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
	COLOR_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
	COLOR_FORMAT_R10G10B10A2_TYPELESS        = 23,
	COLOR_FORMAT_R10G10B10A2_UNORM           = 24,
	COLOR_FORMAT_R10G10B10A2_UINT            = 25,
	COLOR_FORMAT_R11G11B10_FLOAT             = 26,
	COLOR_FORMAT_R8G8B8A8_TYPELESS           = 27,
	COLOR_FORMAT_R8G8B8A8_UNORM              = 28,
	COLOR_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
	COLOR_FORMAT_R8G8B8A8_UINT               = 30,
	COLOR_FORMAT_R8G8B8A8_SNORM              = 31,
	COLOR_FORMAT_R8G8B8A8_SINT               = 32,
	COLOR_FORMAT_R16G16_TYPELESS             = 33,
	COLOR_FORMAT_R16G16_FLOAT                = 34,
	COLOR_FORMAT_R16G16_UNORM                = 35,
	COLOR_FORMAT_R16G16_UINT                 = 36,
	COLOR_FORMAT_R16G16_SNORM                = 37,
	COLOR_FORMAT_R16G16_SINT                 = 38,
	COLOR_FORMAT_R32_TYPELESS                = 39,
	COLOR_FORMAT_D32_FLOAT                   = 40,
	COLOR_FORMAT_R32_FLOAT                   = 41,
	COLOR_FORMAT_R32_UINT                    = 42,
	COLOR_FORMAT_R32_SINT                    = 43,
	COLOR_FORMAT_R24G8_TYPELESS              = 44,
	COLOR_FORMAT_D24_UNORM_S8_UINT           = 45,
	COLOR_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
	COLOR_FORMAT_X24_TYPELESS_G8_UINT        = 47,
	COLOR_FORMAT_R8G8_TYPELESS               = 48,
	COLOR_FORMAT_R8G8_UNORM                  = 49,
	COLOR_FORMAT_R8G8_UINT                   = 50,
	COLOR_FORMAT_R8G8_SNORM                  = 51,
	COLOR_FORMAT_R8G8_SINT                   = 52,
	COLOR_FORMAT_R16_TYPELESS                = 53,
	COLOR_FORMAT_R16_FLOAT                   = 54,
	COLOR_FORMAT_D16_UNORM                   = 55,
	COLOR_FORMAT_R16_UNORM                   = 56,
	COLOR_FORMAT_R16_UINT                    = 57,
	COLOR_FORMAT_R16_SNORM                   = 58,
	COLOR_FORMAT_R16_SINT                    = 59,
	COLOR_FORMAT_R8_TYPELESS                 = 60,
	COLOR_FORMAT_R8_UNORM                    = 61,
	COLOR_FORMAT_R8_UINT                     = 62,
	COLOR_FORMAT_R8_SNORM                    = 63,
	COLOR_FORMAT_R8_SINT                     = 64,
	COLOR_FORMAT_A8_UNORM                    = 65,
	COLOR_FORMAT_R1_UNORM                    = 66,
	COLOR_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
	COLOR_FORMAT_R8G8_B8G8_UNORM             = 68,
	COLOR_FORMAT_G8R8_G8B8_UNORM             = 69,
	COLOR_FORMAT_BC1_TYPELESS                = 70,
	COLOR_FORMAT_BC1_UNORM                   = 71,
	COLOR_FORMAT_BC1_UNORM_SRGB              = 72,
	COLOR_FORMAT_BC2_TYPELESS                = 73,
	COLOR_FORMAT_BC2_UNORM                   = 74,
	COLOR_FORMAT_BC2_UNORM_SRGB              = 75,
	COLOR_FORMAT_BC3_TYPELESS                = 76,
	COLOR_FORMAT_BC3_UNORM                   = 77,
	COLOR_FORMAT_BC3_UNORM_SRGB              = 78,
	COLOR_FORMAT_BC4_TYPELESS                = 79,
	COLOR_FORMAT_BC4_UNORM                   = 80,
	COLOR_FORMAT_BC4_SNORM                   = 81,
	COLOR_FORMAT_BC5_TYPELESS                = 82,
	COLOR_FORMAT_BC5_UNORM                   = 83,
	COLOR_FORMAT_BC5_SNORM                   = 84,
	COLOR_FORMAT_B5G6R5_UNORM                = 85,
	COLOR_FORMAT_B5G5R5A1_UNORM              = 86,
	COLOR_FORMAT_B8G8R8A8_UNORM              = 87,
	COLOR_FORMAT_B8G8R8X8_UNORM              = 88,
	COLOR_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
	COLOR_FORMAT_B8G8R8A8_TYPELESS           = 90,
	COLOR_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
	COLOR_FORMAT_B8G8R8X8_TYPELESS           = 92,
	COLOR_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
	COLOR_FORMAT_BC6H_TYPELESS               = 94,
	COLOR_FORMAT_BC6H_UF16                   = 95,
	COLOR_FORMAT_BC6H_SF16                   = 96,
	COLOR_FORMAT_BC7_TYPELESS                = 97,
	COLOR_FORMAT_BC7_UNORM                   = 98,
	COLOR_FORMAT_BC7_UNORM_SRGB              = 99,
	COLOR_FORMAT_FORCE_UINT                  = 0xffffffff
};

class CResourceTexture : public CResourceBase
{
public:
	CResourceTexture()
	{
		pShaderResourceView = NULL;
		pRenderTargetView = NULL;
		pTextureSource = NULL;
		usage = TEXTURE_NORMAL;
		Format = COLOR_FORMAT_UNKNOWN;
		bDeleteMemoryAfterLoading = false;
	}

	void*				pShaderResourceView;
	void*				pRenderTargetView;
	void*				pTextureSource;

	bool				bDeleteMemoryAfterLoading;

	eTEXTURE_USAGE		usage;
	eTEXTURE_FORMAT		Format;
	UINT				Width;
	UINT				height;
	UINT				MipLevels;

	eRESOURCE_TYPE	Type() const override		{ return RESOURCE_TEXTURE; }
	std::string		strType() override			{ return ENUMSTR(RESOURCE_TEXTURE); }
};

//-------------------------------------------------------------------------------------------------
class CResourceGeometry : public CResourceBase
{
public:
	CResourceGeometry()
		: pVertexBuffer(NULL)
		, pIndexBuffer(NULL)
		, pGraphicMemoryVertexBuffer(NULL)
		, pGraphicMemoryIndexBuffer(NULL)
		, pGraphicMemoryVertexBufferOut(NULL)
		, primitiveCount(0)
		, vertexCount(0)
		, defaultMtrl(-1)
		, eIndexType(INDEX_INVALID)
		, eVertexType(FVF_INVALID)
	{
	}

	~CResourceGeometry()
	{
		SAFE_DELETE_ARRAY(pVertexBuffer);
		SAFE_DELETE_ARRAY(pIndexBuffer);
	}

	bool			IsSkinedMesh() const
	{
		if( eVertexType == FVF_3FP_4BN_2HT_4BW || eVertexType == FVF_4HP_4BN_2HT_4BW )
			return true;
		return false;
	}

	eCVERTEX_TYPE	eVertexType;
	UINT			vertexCount;
	void*			pVertexBuffer;
	void*			pGraphicMemoryVertexBuffer; 
	void*			pGraphicMemoryVertexBufferOut; 

	eCINDEX_TYPE	eIndexType;
	UINT			primitiveCount;
	void*			pIndexBuffer;
	void*			pGraphicMemoryIndexBuffer;

	long			defaultMtrl;
	
	char			mtrlName[MAX_NAME_LENGTH];


	eRESOURCE_TYPE	Type() const override { return RESOURCE_GEOMETRY; }
	std::string		strType() override { return ENUMSTR(RESOURCE_GEOMETRY); }
};

//-------------------------------------------------------------------------------------------------
typedef std::vector<std::string>	SKIN_BONE_LIST;

class CResourceMesh : public CResourceBase
{
	// only object pool can make&delete this class
	friend CObjectPool<CResourceMesh>;
	CResourceMesh(){}
	~CResourceMesh(){}
public:

	std::vector<CResourceGeometry*>	goemetries;
	CVector3						min;
	CVector3						max;
	SKIN_BONE_LIST					skinBoneList;

	eRESOURCE_TYPE	Type() const override { return RESOURCE_MESH; }
	std::string		strType() override { return ENUMSTR(RESOURCE_MESH); }
};

//-------------------------------------------------------------------------------------------------
struct CMotionKey
{
	CQuat		rot;
	CVector3	pos;
	int8		posIndex;
	int8		rotIndex;
};

typedef std::vector<CMotionKey>	MOTION_KEY_LIST;

struct CMotionNode
{
	MOTION_KEY_LIST	keys;
	bool			bStaticNode;
	char			name[MAX_NAME_LENGTH];
	char			parentName[MAX_NAME_LENGTH];
};

typedef std::vector<CMotionNode> MOTION_NODE_LIST;

class CResourceMotion : public CResourceBase
{
	// only object pool can make&delete this class
	friend CObjectPool<CResourceMotion>;
	CResourceMotion(){}
	~CResourceMotion()
	{
		jointList.clear();	
	}

public:
	uint8				frameRate;
	UINT				totalFrame;
	uint8				frameInterval;
	MOTION_NODE_LIST	jointList;

	eRESOURCE_TYPE	Type() const override		{ return RESOURCE_MOTION; }
	std::string		strType() override	{ return ENUMSTR(RESOURCE_MOTION); }
};


//-------------------------------------------------------------------------------------------------
struct CJoint
{
	CQuat			rot;
	CVector3		pos;
	int				parentIndex;

	char			name[MAX_NAME_LENGTH];
	char			parentName[MAX_NAME_LENGTH];
};

typedef std::vector<CJoint>					JOINT_LIST;
typedef std::vector<const CResourceMotion*>	MOTION_LIST;
typedef std::vector<const CResourceMesh*>	MESH_LIST;


class CResourceActor : public CResourceBase
{
	// only object pool can make&delete this class
	friend CObjectPool<CResourceActor>;
	CResourceActor(){}
	~CResourceActor()
	{
		meshList.clear();
		jointList.clear();	
		motionList.clear();
	}

public:
	MESH_LIST			meshList;
	JOINT_LIST			jointList;
	MOTION_LIST			motionList;

	eRESOURCE_TYPE	Type() const override	{ return RESOURCE_ACTOR; }
	std::string		strType() override		{ return ENUMSTR(RESOURCE_ACTOR); }
};

//-------------------------------------------------------------------------------------------------
enum eEFFECT_TYPE
{
	GPASS_VS_LAMBERT,
	GPASS_VS_LAMBERT_WEIGHT,
	GPASS_VS_NORMALMAP,
	GPASS_VS_NORMALMAP_WEIGHT,

	GPASS_PS_LAMBERT,

	//////////////////////////////////////////////////////////////////////////
	MPASS_VS_FONT,
	MPASS_VS_COLOR,

	MPASS_PS_FONT,
	MPASS_PS_COLOR,

	NUM_EFFECT_TYPE,

	INVAILD_EFFECT,
};

enum eTEXTURE_TYPE
{
	PARAM_DIFFISE,
	PARAM_SPECULAR,
	PARAM_NORMAL,
	PARAM_TRANSPARENCY,

	NUM_TEXTURE_TYPE
};

class CResourceMtrl : public CResourceBase
{
	// only object pool can make&delete this class
	friend CObjectPool<CResourceMtrl>;
	CResourceMtrl(){}
	~CResourceMtrl(){}

public:
	long			RID_textures[NUM_TEXTURE_TYPE];
	eEFFECT_TYPE	effectType;

	eRESOURCE_TYPE	Type() const override		{ return RESOURCE_MATERIAL; }
	std::string		strType() override	{ return ENUMSTR(RESOURCE_MATERIAL); }
};


//-------------------------------------------------------------------------------------------------
inline long GET_HASH_KEY( std::string name )
{
	std::locale loc;
	const std::collate<char>& coll = std::use_facet<std::collate<char> >(loc);

	long hashKey = coll.hash(name.data(), name.data() + name.length());

	return hashKey;
}