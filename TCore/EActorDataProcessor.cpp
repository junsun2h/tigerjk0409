#include "CResource.h"

#include "IAssetMgr.h"
#include "ILoader.h"

#include "EGlobal.h"
#include "EActorDataProcessor.h"


EActorDataProcessor::EActorDataProcessor( std::string name )
	: m_name(name)
{
}


EActorDataProcessor::~EActorDataProcessor()
{

}

bool EActorDataProcessor::PopData()
{
	return true;
}

bool EActorDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	return true;
}

CResourceBase* EActorDataProcessor::Process( void* pData, SIZE_T cBytes )
{
	BYTE* pSrcBits = ( BYTE* )pData;

	UINT version;
	ECopyData( &version, &pSrcBits,  4 );

	if( version != ACTOR_FILE_VERSION )
	{
		assert(0);
		return NULL;
	}

	IAssetMgr* pAssetMgr = GLOBAL::AssetMgr();
	CResourceActor* pActor = (CResourceActor*)pAssetMgr->CreateResource( RESOURCE_ACTOR, m_name.c_str() );
	pActor->loadState = RESOURCE_LOAD_STARTED;

	uint8 jointCount;
	ECopyData( &jointCount, &pSrcBits,  1 );

	for(uint8 i=0; i< jointCount; ++i)
	{
		CJoint joint;

		ECopyData( &joint, &pSrcBits,  32 );
		ECopyString(joint.name, &pSrcBits);
		ECopyString(joint.parentName, &pSrcBits);

		pActor->jointList.push_back(joint);
	}

	uint8 motionCount = 0;
	ECopyData( &motionCount, &pSrcBits,  1 );

	for(uint8 i=0; i< motionCount; ++i)
	{
		char strMotion[64];

		ECopyString(strMotion, &pSrcBits);
		CResourceMotion* motion = (CResourceMotion*)pAssetMgr->GetResource(RESOURCE_MOTION, strMotion);
		pActor->motionList.push_back(motion);
	}

	pActor->loadState = RESOURCE_LOAD_FINISHED;
	return pActor;
}