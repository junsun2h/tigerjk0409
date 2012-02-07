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
	const CResourceActor*	GetActorInfo() override;
	IMotionMgr*				GetMotionMgr() override;
	void					SetActor(const CResourceActor* pResource) override;

	IEntity*				GetEntity() override		{ return m_pEntity; }
	
	void					ProcessEvent( EntityEvent &event ) override;
	void					Init(IEntity* pEntity) override;

	void					Update(float deltaTime) override;

private:
	IEntity*				m_pEntity;
	const CResourceActor*	m_pResource;
	JOINT_ENTITY_LIST		m_pJointEntities;
};