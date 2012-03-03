#pragma once



struct MapKeyActorMotion
{
	uint8		actorNodeIndex;
	uint8		motionNodeIndex;
	CQuat		rot;
	CVector3	pos;
};

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

	std::vector<MapKeyActorMotion>		m_Map;

	float					m_TimePerFrame;
};