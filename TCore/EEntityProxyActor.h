#pragma once


class EEntityProxyActor : public IEntityProxyActor 
{
	// only object pool can make&delete this class
	friend CObjectPool<EEntityProxyActor>;
	EEntityProxyActor(){}
	~EEntityProxyActor(){}

public:
	void					Destroy();

	const CResourceActor*	GetActorInfo() override;
	IMotionMgr*				GetMotionMgr() override;
	void					SetActor(const CResourceActor* pResource) override;

	IEntity*				GetEntity() override		{ return m_pEntity; }
	
	void					ProcessEvent( EntityEvent &event ) override;
	void					Init(IEntity* pEntity) override;

	void					Update(float deltaTime) override;

private:
	typedef std::vector<IEntity*>	JOINT_LIST;

	IEntity*				m_pEntity;
	const CResourceActor*	m_pResource;
	JOINT_LIST				m_pJoints;
};