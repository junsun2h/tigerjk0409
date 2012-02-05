#pragma once


class EEntityProxyActor : public IEntityProxyActor 
{
	// only object pool can make&delete this class
	friend CObjectPool<EEntityProxyActor>;
	EEntityProxyActor(){}
	~EEntityProxyActor(){}

public:
	void					Destroy(){}

	const CResourceActor*	GetActorInfo() override;
	IMotionMgr*				GetMotionMgr() override;

	IEntity*				GetEntity() override		{ return m_pEntity; }
	
	void					ProcessEvent( EntityEvent &event ) override;
	void					Init(IEntity* pEntity) override;

	void					Update(float deltaTime) override;

private:
	IEntity*				m_pEntity;
};