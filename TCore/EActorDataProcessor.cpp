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

	CResourceActor* pActor = (CResourceActor*)GLOBAL::EngineMemoryMgr()->GetNewResource(RESOURCE_ACTOR);

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

	strcpy_s( pActor->name, m_name.c_str());
	GLOBAL::AssetMgr()->Insert( pActor );

	return pActor;
}