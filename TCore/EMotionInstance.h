#pragma once



class EMotionInstance : public IMotionInstance
{
	void					Init(CMotionDesc& desc) override;
	void					Destroy() override;

	const CMotionDesc*		GetDesc() override			{ return &m_Desc;}
	const CMotionState*		GetState() override			{ return &m_State;}

	bool					Update(float timeDelta) override;
	void					Stop(bool bInstant) override;
	void					ApplyToMotionPose(MOTION_POSE* pMotionPose) override;

	void					UpdateBlendWeight(float timeDelta);
	void					UpdateFrame(float timeDelta);
	void					UpdateMatrix();

	CMotionDesc				m_Desc;
	CMotionState			m_State;

	MOTION_POSE				m_JointMatrix;
};