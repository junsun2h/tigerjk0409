#pragma once



class EMotionInstance : public IMotionInstance
{
	friend CObjectPool<EMotionInstance>;
	EMotionInstance(){}
	~EMotionInstance();

public:
	void					Init(CMotionDesc* pDesc, long generateTiming);

	const CMotionDesc*		GetDesc() override			{ return &m_Desc;}
	const CMotionState*		GetState() override			{ return &m_State;}

	eMOTION_PLAY_STATE		VisibleUpdate(float timeDelta) override;
	eMOTION_PLAY_STATE		CulledUpdate(float timeDelta) override;

	void					ApplyToMotionPose(MOTION_POSE* pMotionPose) override;

private:
	void					UpdateBlendWeight(float timeDelta);
	void					UpdateFrame(float timeDelta);
	void					UpdateMatrix();


	CMotionDesc				m_Desc;
	CMotionState			m_State;

	MOTION_POSE				m_JointMatrix;

	float					m_TimePerFrame;
};