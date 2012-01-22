#include "EEntityProxyActor.h"


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

void EEntityProxyActor::Init(IEntity* pEntity)
{
	m_pEntity = pEntity;
}

void EEntityProxyActor::Update(float deltaTime)
{

}