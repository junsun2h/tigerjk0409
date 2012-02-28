#include "CResource.h"

#include "IMotionInstance.h"

#include "EMotionInstance.h"


EMotionInstance::~EMotionInstance()
{
	m_JointMatrix.clear();
	m_Desc.pResource = NULL;
}

//--------------------------------------------------------------------------------------------------------------------
eMOTION_PLAY_STATE EMotionInstance::VisibleUpdate(float timeDelta)
{
	if( m_Desc.pResource == NULL)
	{
		assert(0);
		return MOTION_PLAY_INVAILD;
	}

	if( m_State.ePlayState == MOTION_STOPPED)
		return MOTION_STOPPED;

	UpdateFrame(timeDelta);
	UpdateBlendWeight(timeDelta);
	UpdateMatrix();
	
	return m_State.ePlayState;
}


//--------------------------------------------------------------------------------------------------------------------
eMOTION_PLAY_STATE EMotionInstance::CulledUpdate(float timeDelta)
{
	if( m_Desc.pResource == NULL)
	{
		assert(0);
		return MOTION_PLAY_INVAILD;
	}

	if( m_State.ePlayState == MOTION_STOPPED)
		return MOTION_STOPPED;

	UpdateFrame(timeDelta);
	UpdateBlendWeight(timeDelta);

	return m_State.ePlayState;
}


//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::UpdateFrame(float timeDelta)
{
	if( m_State.ePlayState == MOTION_PLAY_FADE_OUT_AFTER_END)
		return;

	m_State.passedTime += timeDelta;

	float frame = m_State.passedTime/m_TimePerFrame;
	float fkey = frame/m_Desc.pResource->frameInterval;
	m_State.currentKey = int(fkey);
	
	if( m_State.passedTime > m_Desc.length )
	{
		m_State.loopCount++;
		m_State.passedTime -= m_Desc.length;
		m_State.blendRatio = fkey - m_State.currentKey;
		m_State.currentKey = 0;

		if( m_Desc.bLoop == false )
		{
			m_State.ePlayState = MOTION_PLAY_FADE_OUT_AFTER_END;
			m_State.blendTime = 0;
		}
	}
	else
	{
		m_State.blendRatio = fkey - m_State.currentKey;
	}
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::UpdateBlendWeight(float timeDelta)
{
	if( m_State.ePlayState == MOTION_PLAY_FADE_IN )
	{
		m_State.blendTime += timeDelta;
		if( m_State.blendTime < m_Desc.fBlendInTime )
		{
			m_State.weight = ( m_State.blendTime / m_Desc.fBlendInTime ) * m_Desc.fTargetWeight;
		}
		else
		{
			m_State.weight = m_Desc.fTargetWeight;
			m_State.ePlayState = MOTION_PLAY;
			m_State.blendTime = 0;
		}
	}
	else if( m_State.ePlayState == MOTION_PLAY_FADE_OUT_AFTER_END )
	{
		m_State.blendTime += timeDelta;

		if( m_State.blendTime > m_Desc.fBlendOutTime )
		{
			m_State.ePlayState = MOTION_STOPPED;
			return;
		}

		m_State.weight = m_Desc.fTargetWeight * (1 - m_State.blendTime / m_Desc.fBlendOutTime );
	}
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::UpdateMatrix()
{
	UINT jointCount = m_JointMatrix.size();
	const MOTION_NODE_LIST& resourceJoint = m_Desc.pResource->jointList;

	if( m_State.ePlayState == MOTION_PLAY_FADE_OUT_AFTER_END )
	{
		UINT lastKey = resourceJoint[0].keys.size() -1;

		for( UINT i =0; i< jointCount ; ++i )
		{
			const CMotionNode& node = resourceJoint[i];
			const CMotionKey& key = node.keys[lastKey];
			
			m_JointMatrix[i].rot = node.keys[key.rotIndex].rot;
			m_JointMatrix[i].pos = node.keys[key.posIndex].pos;
		}
		return;
	}
	else
	{
		for( UINT i =0; i< jointCount ; ++i )
		{
			const CMotionNode& node = resourceJoint[i];
			const CMotionKey& key = node.keys[m_State.currentKey];
			const CMotionKey& keyNext = node.keys[m_State.currentKey+1];

			// if transform doesn't changed, don't interpolate it.
			if( key.rotIndex == keyNext.rotIndex )
				m_JointMatrix[i].rot = node.keys[key.rotIndex].rot;
			else
				m_JointMatrix[i].rot = XMQuaternionSlerp( node.keys[key.rotIndex].rot.m128,	node.keys[key.rotIndex].rot.m128, m_State.blendRatio );

			if( key.posIndex == keyNext.posIndex )
				m_JointMatrix[i].pos = node.keys[key.posIndex].pos;
			else
				m_JointMatrix[i].pos = CVector3::Lerp( node.keys[key.posIndex].pos, node.keys[keyNext.posIndex].pos, m_State.blendRatio );
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::ApplyToMotionPose(MOTION_POSE* pMotionPose)
{
	UINT jointCount = pMotionPose->size();

	if( m_State.weight < 1)
	{
		for( UINT i =0; i< jointCount ; ++i )
		{
			if( m_MapBetweenActorAndNode[i] == -1 )
				continue;

			CMotionTransform& motionTM = m_JointMatrix[ m_MapBetweenActorAndNode[i] ];

			(*pMotionPose)[i].rot = XMQuaternionSlerp( (*pMotionPose)[i].rot.m128, motionTM.rot.m128, m_State.weight );
			(*pMotionPose)[i].pos = CVector3::Lerp( (*pMotionPose)[i].pos, motionTM.pos, m_State.weight );
		}
	}
	else
	{
		for( UINT i =0; i< jointCount ; ++i )
		{
			if( m_MapBetweenActorAndNode[i] == -1 )
				continue;

			(*pMotionPose)[i] = m_JointMatrix[ m_MapBetweenActorAndNode[i] ];
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::Init(CMotionDesc* pDesc, long generateTiming)
{
	if( pDesc->pResource == NULL || pDesc->pResourceActor == NULL)
	{
		assert(0);
		return;
	}

	m_Desc = *pDesc;
	m_TimePerFrame = 1.0f/m_Desc.pResource->frameRate;
	m_Desc.length = m_Desc.pResource->totalFrame / float(m_Desc.pResource->frameRate);

	const CResourceMotion* pMotion = pDesc->pResource;
	const CResourceActor* pActor = pDesc->pResourceActor;

	//////////////////////////////////////////////////////////////////////////
	// assign motion node matrix
	UINT motionNodeCount = pMotion->jointList.size();
	if( m_JointMatrix.size() != motionNodeCount )
	{
		m_JointMatrix.clear();
		for( UINT i=0; i < motionNodeCount; ++i )
			m_JointMatrix.push_back( CMotionTransform() );
	}

	//////////////////////////////////////////////////////////////////////////
	// makes map between actor's nodes & motion's nodes
	UINT actorNodeCount = pActor->jointList.size();
	for(UINT i =0; i < actorNodeCount; ++i)
	{
		int index = -1;
		for( UINT iM = 0; iM < motionNodeCount; ++iM )
		{
			if( strcmp(pActor->jointList[i].name, pMotion->jointList[iM].name) == 0 )
				index = iM;
		}
		m_MapBetweenActorAndNode.push_back(index);
	}	

	//////////////////////////////////////////////////////////////////////////
	// set animation states
	m_State.Reset();

	if( m_Desc.fBlendInTime == 0)
		m_State.ePlayState = MOTION_PLAY;
	else
		m_State.ePlayState = MOTION_PLAY_FADE_IN;

	m_State.id = generateTiming;
}