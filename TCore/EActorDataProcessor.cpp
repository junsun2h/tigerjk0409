#include "EActorDataProcessor.h"
#

typedef struct
{
	DWORD dwSize;
	DWORD dwHeaderFlags;
	DWORD dwHeight;
	DWORD dwWidth;
	DWORD dwPitchOrLinearSize;
	DWORD dwDepth; // only if DDS_HEADER_FLAGS_VOLUME is set in dwHeaderFlags
	DWORD dwMipMapCount;
	DWORD dwReserved1[11];
	DWORD dwSurfaceFlags;
	DWORD dwCubemapFlags;
	DWORD dwReserved2[3];
} ACTOR_FILE_EADER;


EActorDataProcessor::EActorDataProcessor()
{
}

EActorDataProcessor::~EActorDataProcessor()
{

}

bool EActorDataProcessor::MT_Complete(IAssetMgr* pAssetMgr)
{
	return true;
}

bool EActorDataProcessor::MT_Destroy()
{
	return true;
}

bool EActorDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	return true;
}