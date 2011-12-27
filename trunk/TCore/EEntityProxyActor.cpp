#include "EEntityProxyActor.h"


EEntityProxyActor::EEntityProxyActor(std::string& name, long id)
	: m_Name(name)
	, m_ID(id)
	, m_EntityID(-1)
{

}

EEntityProxyActor::~EEntityProxyActor()
{

}

const CResourceActor* EEntityProxyActor::GetActorInfo()
{
	return NULL;
}

IMotionMgr*	EEntityProxyActor::GetMotionMgr()
{
	return NULL;
}

void EEntityProxyActor::ProcessEvent( EntityEvent &event )
{

}

void EEntityProxyActor::SetEntity(IEntity* pEntity)
{
	m_EntityID = pEntity->GetID();
}

void EEntityProxyActor::Update(float deltaTime)
{

}

void EEntityProxyActor::Init(std::string strResource)
{

}