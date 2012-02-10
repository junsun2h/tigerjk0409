#include "IMotionInstance.h"
#include "EMotionInstance.h"


bool EMotionInstance::Update(float timeDelta)
{
	if( m_Desc.pResource == NULL)
		return false;

	if( m_State.ePlayState == TMOTION_STOP)
		return false;

	UpdateFrame(timeDelta);
	UpdateBlendWeight(timeDelta);
	UpdateMatrix();

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::UpdateFrame(float timeDelta)
{
	if( m_State.ePlayState == TMOTION_FADE_OUT_AFTER_END)
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
			m_State.ePlayState = TMOTION_FADE_OUT_AFTER_END;
			m_State.blendTime = 0;
		}
	}

	m_State.blendRatio = ratio - int(ratio);
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::UpdateBlendWeight(float timeDelta)
{
	if( m_State.ePlayState == TMOTION_FADE_IN )
	{
		m_State.blendTime += timeDelta;
		if( m_State.blendTime < m_Desc.fBlendInTime )
		{
			m_State.weight = ( m_State.blendTime / m_Desc.fBlendInTime ) * m_Desc.fTargetWeight;
		}
		else
		{
			m_State.weight = m_Desc.fTargetWeight;
			m_State.ePlayState = TMOTION_PLAY;
			m_State.blendTime = 0;
		}
	}
	else if( m_State.ePlayState == TMOTION_FADE_OUT_AFTER_END )
	{
		m_State.blendTime += timeDelta;

		if( m_State.blendTime > m_Desc.fBlendOutTime )
		{
			m_State.ePlayState = TMOTION_STOP;
			return;
		}

		m_State.weight = m_Desc.fTargetWeight * (1 - m_State.blendTime / m_Desc.fBlendOutTime );
	}
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::UpdateMatrix()
{
	UINT jointCount = m_JointMatrix.size();

	if( m_State.ePlayState == TMOTION_FADE_OUT_AFTER_END )
	{
		for( UINT i =0; i< jointCount ; ++i )
		{
			CMotionKey& key = m_Desc.pResource->jointList[i].keys[m_Desc.samplingCount -1];
			
			m_JointMatrix[i].rot = key.rot;
			m_JointMatrix[i].pos = key.pos;
		}
		return;
	}

	for( UINT i =0; i< jointCount ; ++i )
	{
		CMotionKey& key = m_Desc.pResource->jointList[i].keys[m_State.currentKey];
		CMotionKey& keyNext = m_Desc.pResource->jointList[i].keys[m_State.currentKey+1];

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
void EMotionInstance::Stop(bool bInstant)
{
	if( bInstant )
	{
		m_State.ePlayState = TMOTION_STOP;
	}
	else
	{
		m_State.ePlayState = TMOTION_FADE_OUT_AFTER_END;
		m_State.blendTime = 0;
	}

	m_Desc.bLoop = false;
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::Init(CMotionDesc& desc)
{
	if( desc.pResource == NULL )
		return;

	m_Desc = desc;
	m_Desc.timePerFrame = 1.0f/m_Desc.pResource->frameRate;
	m_Desc.samplingCount = m_Desc.pResource->jointList[0].keys.size() -1;
	m_Desc.length = m_Desc.pResource->totalFrame / float(m_Desc.pResource->frameRate);

	UINT jointCount = desc.pResource->jointList.size();
	if( m_JointMatrix.size() != jointCount )
	{
		m_JointMatrix.clear();
		m_JointMatrix.reserve( jointCount );
	}

	m_State.Reset();
}

//--------------------------------------------------------------------------------------------------------------------
void EMotionInstance::Destroy()
{
	m_Desc.pResource = NULL;
}
