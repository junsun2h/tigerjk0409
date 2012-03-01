#pragma once


class EEntityProxyActor : public IEntityProxyActor 
{
	// only object pool can make&delete this class
	friend CObjectPool<EEntityProxyActor>;
	EEntityProxyActor(){}
	~EEntityProxyActor();

public:
	void					Destroy();

	JOINT_ENTITY_LIST*		GetJointEntities() override		{ return &m_pJointEntities; }
	SKIN_REF_MATRIX*		GetSkinReferenceMatrix(UINT meshID) override	{ return &m_SkinMatrices[meshID]; }

	const CResourceActor*	GetResource() override;
	void					SetActor(const CResourceActor* pResource) override;

	IEntity*				GetEntity() override		{ return m_pEntity; }
	
	void					ProcessEvent( EntityEvent &event ) override;
	void					Init(IEntity* pEntity) override;
	
	void					Play(CMotionDesc* pDesc) override;
	void					Stop() override;
	void					Freeze() override					{ m_bPause = true; }
	void					Resume() override					{ m_bPause = false; }
	bool					IsPlaying() override;

	MOTION_INSTANCE_LIST*	GetPlayingMotions() override		{ 	return &m_PlayingMotionList; }
	void					Update(float deltaTime) override;

private:
	void					VisibleUpdate(float deltaTime);
	void					InvisibleUpdate(float deltaTime);
	void					ApplyAnimationToActor();

	IEntity*				m_pEntity;
	const CResourceActor*	m_pResource;
	JOINT_ENTITY_LIST		m_pJointEntities;

	MOTION_INSTANCE_LIST	m_PlayingMotionList;
	MOTION_POSE				m_AnimationPos;
	MOTION_POSE_MATRIX		m_AnimationMatrix;
	MOTION_POSE_MATRIX		m_InvBindMatrix;

	std::vector<SKIN_REF_MATRIX> m_SkinMatrices;

	bool					m_bPause;
};