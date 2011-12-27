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
	long					GetEntityID() override		{ return m_EntityID; }
	
	void					ProcessEvent( EntityEvent &event ) override;
	void					SetEntity(IEntity* pEntity) override;

	void					Update(float deltaTime) override;

private:
	long					m_ID;
	long					m_EntityID;
	std::string				m_Name;
};