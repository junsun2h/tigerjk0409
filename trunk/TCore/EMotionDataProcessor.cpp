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

	CResourceMotion* pMotion = (CResourceMotion*)GLOBAL::ResourceMemMgr()->GetNew(RESOURCE_MOTION);

	ECopyData( &pMotion->frameRate, &pSrcBits,  1 );
	ECopyData( &pMotion->frameInterval, &pSrcBits,  1 );
	ECopyData( &pMotion->totalFrame, &pSrcBits,  4);

	uint8 jointCount;
	ECopyData( &jointCount, &pSrcBits,  1 );

	int keyCount = pMotion->totalFrame/pMotion->frameInterval + 1;
	if( pMotion->totalFrame/float( pMotion->frameRate) > keyCount )
		keyCount++;

	for(uint8 i=0; i< jointCount; ++i)
	{
		CMotionNode joint;

		ECopyString(joint.name, &pSrcBits);
		ECopyString(joint.parentName, &pSrcBits);

		for( int iKey = 0; iKey < keyCount; ++iKey)
		{
			CMotionKey key;

			ECopyData( &key.bKeyChanged, &pSrcBits,  1 );
			if( key.bKeyChanged )
			{
				ECopyData( &key, &pSrcBits,  28 );
				key.keyIndex = iKey;
			}
			else
			{
				key.keyIndex = joint.keys[iKey-1].keyIndex;
			}

			joint.keys.push_back( key);
		}

		pMotion->jointList.push_back(joint);
	}
	
	strcpy_s( pMotion->name, m_name.c_str());
	GLOBAL::AssetMgr()->Insert( pMotion );

	return pMotion;
}