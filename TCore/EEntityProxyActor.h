#pragma once

#include "IEntity.h"
#include "CUnitPool.h"


class EEntityProxyActor : public IEntityProxyActor 
{
	// only object pool can make&delete this class
	friend CObjectPool<EEntityProxyActor>;
	EEntityProxyActor(){}
	~EEntityProxyActor(){}

public:
	void					Init(std::string& name, long id);
	void					Destroy(){}

	const CResourceActor*	GetActorInfo() override;
	IMotionMgr*				GetMotionMgr() override;

	long					GetID()	override			{ return m_ID; }
	IEntity*				GetEntity() override		{ return m_pEntity; }
	
	void					ProcessEvent( EntityEvent &event ) override;
	void					SetEntity(IEntity* pEntity) override;

	void					Update(float deltaTime) override;

private:
	long					m_ID;
	IEntity*				m_pEntity;
	std::string				m_Name;
};