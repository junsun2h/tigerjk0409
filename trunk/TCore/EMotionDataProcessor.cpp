#include "CResource.h"

#include "IAssetMgr.h"
#include "ILoader.h"

#include "EGlobal.h"
#include "EMotionDataProcessor.h"



EMotionDataProcessor::EMotionDataProcessor( std::string name )
	: m_name(name)
{
}


EMotionDataProcessor::~EMotionDataProcessor()
{

}

bool EMotionDataProcessor::PopData()
{
	return true;
}

bool EMotionDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	return true;
}

CResourceBase* EMotionDataProcessor::Process( void* pData, SIZE_T cBytes )
{
	BYTE* pSrcBits = ( BYTE* )pData;

	UINT version;
	ECopyData( &version, &pSrcBits,  4 );

	if( version != MOTION_FILE_VERSION )
	{
		assert(0);
		return NULL;
	}

	IAssetMgr* pAssetMgr = GLOBAL::AssetMgr();
	CResourceMotion* pMotion = (CResourceMotion*)pAssetMgr->CreateResource( RESOURCE_MOTION, m_name.c_str() );
	pMotion->loadState = RESOURCE_LOAD_STARTED;

	ECopyData( &pMotion->frameRate, &pSrcBits,  1 );
	ECopyData( &pMotion->frameInterval, &pSrcBits,  1 );
	ECopyData( &pMotion->totalFrame, &pSrcBits,  4);

	uint8 jointCount;
	ECopyData( &jointCount, &pSrcBits,  1 );

	int keyCount = pMotion->totalFrame/pMotion->frameInterval + 1;
	if( float(pMotion->totalFrame)/pMotion->frameInterval > pMotion->totalFrame/pMotion->frameInterval )
		keyCount++;

	for(uint8 i=0; i< jointCount; ++i)
	{
		CMotionNode joint;

		ECopyString(joint.name, &pSrcBits);
		ECopyString(joint.parentName, &pSrcBits);

		for( int iKey = 0; iKey < keyCount; ++iKey)
		{
			CMotionKey key;

			ECopyData( &key.posIndex, &pSrcBits, 1 );
			if( key.posIndex == iKey )
				ECopyData( &key.pos, &pSrcBits, sizeof(CVector3) );

			ECopyData( &key.rotIndex, &pSrcBits, 1 );
			if( key.rotIndex == iKey )
				ECopyData( &key.rot, &pSrcBits, sizeof(CQuat) );

			joint.keys.push_back( key);
		}

		pMotion->jointList.push_back(joint);
	}
	
	pMotion->loadState = RESOURCE_LOAD_FINISHED;
	return pMotion;
}