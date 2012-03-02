#include "CResource.h"

#include "ILoader.h"

#include "EGlobal.h"
#include "EMotionDataProcessor.h"



EMotionDataProcessor::EMotionDataProcessor()
{
}


EMotionDataProcessor::~EMotionDataProcessor()
{

}

void EMotionDataProcessor::Init(CResourceBase* pRsc, bool bForward)
{
	m_pMotion = (CResourceMotion*)pRsc;
	m_pMotion->loadState = RESOURCE_LOAD_STARTED;
}

bool EMotionDataProcessor::CompleteWork()
{
	m_pMotion->loadState = RESOURCE_LOAD_FINISHED;
	return true;
}

bool EMotionDataProcessor::PT_Process( void* pData, SIZE_T cBytes )
{
	return true;
}

void EMotionDataProcessor::Process( void* pData, SIZE_T cBytes )
{
	BYTE* pSrcBits = ( BYTE* )pData;

	UINT version;
	ECopyData( &version, &pSrcBits,  4 );

	if( version != MOTION_FILE_VERSION )
	{
		assert(0);
		m_pMotion->loadState = RESOURCE_LOAD_FAILED;
		return;
	}
	
	ECopyData( &m_pMotion->frameRate, &pSrcBits,  1 );
	ECopyData( &m_pMotion->frameInterval, &pSrcBits,  1 );
	ECopyData( &m_pMotion->totalFrame, &pSrcBits,  4);

	uint8 jointCount;
	ECopyData( &jointCount, &pSrcBits,  1 );

	int keyCount = m_pMotion->totalFrame/m_pMotion->frameInterval + 1;
	if( float(m_pMotion->totalFrame)/m_pMotion->frameInterval > m_pMotion->totalFrame/m_pMotion->frameInterval )
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

		m_pMotion->jointList.push_back(joint);
	}
}