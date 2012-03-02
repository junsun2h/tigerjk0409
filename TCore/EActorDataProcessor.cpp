#include "CResource.h"

#include "IAssetMgr.h"
#include "ILoader.h"

#include "EGlobal.h"
#include "EActorDataProcessor.h"


EActorDataProcessor::EActorDataProcessor()
{
}


EActorDataProcessor::~EActorDataProcessor()
{

}

void EActorDataProcessor::Init(CResourceBase* pRsc, bool bForward)
{
	m_pActor = (CResourceActor*)pRsc;
	m_pActor->loadState = RESOURCE_LOAD_STARTED;
	m_bForward = bForward;
}

bool EActorDataProcessor::CompleteWork()
{
	if( m_bForward )
	{
		m_pActor->loadState = RESOURCE_LOAD_FINISHED;
		return true;
	}

	UINT meshCount = m_pActor->meshList.size();
	for( UINT i =0 ; i < meshCount; ++i )
	{
		if( m_pActor->meshList[i]->loadState != RESOURCE_LOAD_FINISHED )
			return false;
	}

	UINT motionCount = m_pActor->motionList.size();
	for( UINT i =0 ; i < motionCount; ++i )
	{
		if( m_pActor->motionList[i]->loadState != RESOURCE_LOAD_FINISHED )
			return false;
	}
	
	m_pActor->loadState = RESOURCE_LOAD_FINISHED;
	return true;
}

void EActorDataProcessor::Process( void* pData, SIZE_T cBytes )
{
	IAssetMgr* pAssetMgr = GLOBAL::AssetMgr();

	BYTE* pSrcBits = ( BYTE* )pData;

	UINT version;
	ECopyData( &version, &pSrcBits,  4 );

	if( version != ACTOR_FILE_VERSION )
	{
		assert(0);
		m_pActor->loadState = RESOURCE_LOAD_FAILED;
		return;
	}

	// mesh data
	uint8 meshCount = 0;
	ECopyData( &meshCount, &pSrcBits,  1 );

	for(uint8 i=0; i< meshCount; ++i)
	{
		char strMesh[64];

		ECopyString(strMesh, &pSrcBits);
		CResourceMesh* pMesh = (CResourceMesh*)pAssetMgr->GetResource(RESOURCE_MESH, strMesh);
		if( pMesh == NULL)
			pMesh = (CResourceMesh*)GLOBAL::Loader()->Load( strMesh, RESOURCE_FILE_MESH, m_bForward );

		m_pActor->meshList.push_back(pMesh);
	}

	// joint data
	uint8 jointCount;
	ECopyData( &jointCount, &pSrcBits,  1 );

	for(uint8 i=0; i< jointCount; ++i)
	{
		CJoint joint;

		ECopyData( &joint, &pSrcBits,  32 );
		ECopyString(joint.name, &pSrcBits);
		ECopyString(joint.parentName, &pSrcBits);

		m_pActor->jointList.push_back(joint);
	}

	// motion data
	uint8 motionCount = 0;
	ECopyData( &motionCount, &pSrcBits,  1 );

	for(uint8 i=0; i< motionCount; ++i)
	{
		char strMotion[64];

		ECopyString(strMotion, &pSrcBits);
		CResourceMotion* motion = (CResourceMotion*)pAssetMgr->GetResource(RESOURCE_MOTION, strMotion);
		if( motion == NULL)
			motion = (CResourceMotion*)GLOBAL::Loader()->Load( strMotion, RESOURCE_FILE_MOTION, m_bForward);

		m_pActor->motionList.push_back(motion);
	}

	m_pActor->loadState = RESOURCE_LOAD_FINISHED_WAIT_FOR_SUB_RSC;
}