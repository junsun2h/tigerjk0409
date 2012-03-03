#include "CResource.h"

#include "IMotionInstance.h"

#include "EMotionInstance.h"


EMotionInstance::~EMotionInstance()
{
	m_Desc.pResource = NULL;
}

//--------------------------------------------------------------------------------------------------------------------
eMOTION_PLAY_STATE EMotionInstance::UpdateFrame(float timeDelta)
{
	if( m_Desc.pResource == NULL)
	{
		assert(0);
		return MOTION_PLAY_INVAILD;
	}

	if( m_State.ePlayState == MOTION_STOPPED)
		return MOTION_STOPPED;
	
	if( m_State.ePlayState == MOTION_PLAY_FADE_OUT_AFTER_END)
		return MOTION_PLAY_FADE_OUT_AFTER_END;

	//////////////////////////////////////////////////////////////////////////
	// update frame
	m_State.passedTime += timeDelta;
	if( m_State.passedTime > m_Desc.length )
	{
		m_State.loopCount++;
		m_State.passedTime -= m_Desc.length;
		float frame = m_State.passedTime/m_TimePerFrame;
		float fkey = frame/m_Desc.pResource->frameInterval;

		m_State.blendRatio = fkey - int(fkey);
		m_State.currentKey = 0;

		if( m_Desc.bLoop == false )
		{
			m_State.ePlayState = MOTION_PLAY_FADE_OUT_AFTER_END;
			m_State.blendTime = 0;
	
			const MOTION_NODE_LIST& resourceJoint = m_Desc.pResource->jointList;
			UINT lastKey = resourceJoint[0].keys.size() -1;
			for( UINT i =0; i< m_Map.size() ; ++i )
			{
				const CMotionNode& node = m_Desc.pResource->jointList[ m_Map[i].motionNodeIndex ];

				if( node.bStaticNode )
					continue;

				const CMotionKey& key = node.keys[lastKey];

				m_Map[i].rot = node.keys[key.rotIndex].rot;
				m_Map[i].pos = node.keys[key.posIndex].pos;
			}
		}
	}
	else
	{
		float frame = m_State.passedTime/m_TimePerFrame;
		float fkey = frame/m_Desc.pResource->frameInterval;
		m_State.currentKey = int(fkey);
		m_State.blendRatio = fkey - m_State.currentKey;
	}	

	UpdateBlendWeight(timeDelta);
	return m_State.ePlayState;
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
	if( m_State.ePlayState == MOTION_PLAY_FADE_OUT_AFTER_END )
		return;

	for( UINT i =0; i< m_Map.size() ; ++i )
	{
		const CMotionNode& node = m_Desc.pResource->jointList[ m_Map[i].motionNodeIndex ];

		if( node.bStaticNode )
			continue;

		const CMotionKey& key = node.keys[m_State.currentKey];
		const CMotionKey& keyNext = node.keys[m_State.currentKey+1];

		// if transform doesn't changed, don't interpolate it.
		if( key.rotIndex == keyNext.rotIndex )
			m_Map[i].rot = node.keys[key.rotIndex].rot;
		else
			m_Map[i].rot = XMQuaternionSlerp( node.keys[key.rotIndex].rot.m128,	node.keys[keyNext.rotIndex].rot.m128, m_State.blendRatio );

		if( key.posIndex == keyNext.posIndex )
			m_Map[i].pos = node.keys[key.posIndex].pos;
		else
			m_Map[i].pos = CVector3::Lerp( node.keys[key.posIndex].pos, node.keys[keyNext.posIndex].pos, m_State.blendRatio );
	}
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::ApplyToMotionPose(MOTION_POSE* pMotionPose)
{
	if( m_State.weight < 1)
	{
		for( UINT i =0; i< m_Map.size() ; ++i )
		{
			uint8 actorNode = m_Map[i].actorNodeIndex;

			(*pMotionPose)[actorNode].rot = XMQuaternionSlerp( (*pMotionPose)[actorNode].rot.m128, m_Map[i].rot.m128, m_State.weight );
			(*pMotionPose)[actorNode].pos = CVector3::Lerp( (*pMotionPose)[actorNode].pos, m_Map[i].pos, m_State.weight );
		}
	}
	else
	{
		for( UINT i =0; i< m_Map.size() ; ++i )
		{
			(*pMotionPose)[ m_Map[i].actorNodeIndex ].rot = m_Map[i].rot;
			(*pMotionPose)[ m_Map[i].actorNodeIndex ].pos = m_Map[i].pos;
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

	UINT motionNodeCount = pMotion->jointList.size();

	//////////////////////////////////////////////////////////////////////////
	// makes map between actor's nodes & motion's nodes
	UINT actorNodeCount = pActor->jointList.size();
	for(UINT i =0; i < actorNodeCount; ++i)
	{
		for( UINT iM = 0; iM < motionNodeCount; ++iM )
		{
			if( strcmp(pActor->jointList[i].name, pMotion->jointList[iM].name) == 0 )
			{
				MapKeyActorMotion mapkey;
				
				mapkey.actorNodeIndex = i;
				mapkey.motionNodeIndex = iM;
				mapkey.pos = pMotion->jointList[iM].keys[0].pos;
				mapkey.rot = pMotion->jointList[iM].keys[0].rot;

				m_Map.push_back(mapkey);
			}
		}
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