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

	eMOTION_PLAY_STATE		UpdateFrame(float timeDelta) override;
	void					UpdateMatrix() override;

	void					ApplyToMotionPose(MOTION_POSE* pMotionPose) override;

private:
	void					UpdateBlendWeight(float timeDelta);


	CMotionDesc				m_Desc;
	CMotionState			m_State;

	MOTION_POSE				m_JointMatrix;
	std::vector<int>		m_MapBetweenActorAndNode;

	float					m_TimePerFrame;
};