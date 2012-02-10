#pragma once


class EEntityProxyActor : public IEntityProxyActor 
{
	// only object pool can make&delete this class
	friend CObjectPool<EEntityProxyActor>;
	EEntityProxyActor(){}
	~EEntityProxyActor(){}

public:
	void					Destroy();

	JOINT_ENTITY_LIST*		GetJointEntities() override { return &m_pJointEntities; }
	const CResourceActor*	GetResource() override;
	void					SetActor(const CResourceActor* pResource) override;

	IEntity*				GetEntity() override		{ return m_pEntity; }
	
	void					ProcessEvent( EntityEvent &event ) override;
	void					Init(IEntity* pEntity) override;
	
	void					Play(CMotionDesc* pDesc) override;
	void					Freeze() override;
	void					Stop() override;
	bool					IsPlaying() override;
	MOTION_INSTANCE_LIST*	GetPlayingMotions() override		{ 	return &m_PlayingMotionList; }

private:
	void					UpdateJoint();

	IEntity*				m_pEntity;
	const CResourceActor*	m_pResource;
	JOINT_ENTITY_LIST		m_pJointEntities;

	MOTION_INSTANCE_LIST	m_PlayingMotionList;
	MOTION_POSE				m_JointMatrix;
	bool					m_bPause;
};