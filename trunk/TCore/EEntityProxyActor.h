#pragma once

#include "IEntity.h"


class EEntityProxyActor : public IEntityProxyActor 
{
public:
	EEntityProxyActor(std::string& name, long id );
	~EEntityProxyActor();

	void					Init(std::string strResource) override;
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