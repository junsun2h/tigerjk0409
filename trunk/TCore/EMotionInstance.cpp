#include "CResource.h"

#include "IMotionInstance.h"

#include "EMotionInstance.h"


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

	float ratio = m_State.passedTime/m_Desc.timePerFrame;
	m_State.currentKey = int(ratio);

	if( m_State.passedTime > m_Desc.length )
	{
		m_State.loopCount++;
		m_State.passedTime -= m_Desc.length;
		m_State.currentKey = 0;

		if( m_Desc.bLoop == false )
		{
			m_State.ePlayState = MOTION_PLAY_FADE_OUT_AFTER_END;
			m_State.blendTime = 0;
		}
	}

	m_State.blendRatio = ratio - int(ratio);
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

	if( m_State.ePlayState == MOTION_PLAY_FADE_OUT_AFTER_END )
	{
		for( UINT i =0; i< jointCount ; ++i )
		{
			const CMotionKey& key = m_Desc.pResource->jointList[i].keys[m_Desc.samplingCount -1];
			
			m_JointMatrix[i].rot = key.rot;
			m_JointMatrix[i].pos = key.pos;
		}
		return;
	}

	for( UINT i =0; i< jointCount ; ++i )
	{
		const CMotionKey& key = m_Desc.pResource->jointList[i].keys[m_State.currentKey];
		const CMotionKey& keyNext = m_Desc.pResource->jointList[i].keys[m_State.currentKey+1];

		m_JointMatrix[i].rot = XMQuaternionSlerp( key.rot.m128, keyNext.rot.m128, m_State.blendRatio );
		m_JointMatrix[i].pos = CVector3::Lerp( key.pos, keyNext.pos, m_State.blendRatio );
	}
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::ApplyToMotionPose(MOTION_POSE* pMotionPose)
{
	UINT jointCount = m_JointMatrix.size();

	if( m_State.weight < 1)
	{
		for( UINT i =0; i< jointCount ; ++i )
		{
			(*pMotionPose)[i].rot = XMQuaternionSlerp( (*pMotionPose)[i].rot.m128,  m_JointMatrix[i].rot.m128, m_State.weight );
			(*pMotionPose)[i].pos = CVector3::Lerp( (*pMotionPose)[i].pos, m_JointMatrix[i].pos, m_State.weight );
		}
	}
	else
	{
		for( UINT i =0; i< jointCount ; ++i )
		{
			(*pMotionPose)[i] = m_JointMatrix[i];
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::Init(CMotionDesc* pDesc, long generateTiming)
{
	if( pDesc->pResource == NULL )
		return;

	m_Desc = *pDesc;
	m_Desc.timePerFrame = 1.0f/m_Desc.pResource->frameRate;
	m_Desc.samplingCount = m_Desc.pResource->jointList[0].keys.size() -1;
	m_Desc.length = m_Desc.pResource->totalFrame / float(m_Desc.pResource->frameRate);

	UINT jointCount = pDesc->pResource->jointList.size();
	if( m_JointMatrix.size() != jointCount )
	{
		m_JointMatrix.clear();
		m_JointMatrix.reserve( jointCount );
	}

	m_State.Reset();
	m_State.id = generateTiming;
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::Destroy()
{
	m_JointMatrix.clear();
	m_Desc.pResource = NULL;
}
