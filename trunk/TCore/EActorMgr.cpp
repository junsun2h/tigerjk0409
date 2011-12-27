#include "EActorMgr.h"
#include "EEntityProxyActor.h"



IEntityProxyActor* EActorMgr::SPawn(std::string& name)
{
	long newID = GET_HASH_KEY( name );
	TYPE_ACTOR_PROXY_MAP::CPair* pActorProxy = m_ActorProxyyMap.Lookup( newID );
	if( pActorProxy != NULL )
	{
		assert(0);
		return pActorProxy->m_value;
	} 

	EEntityProxyActor* newEnitty = new EEntityProxyActor(name, newID);
	m_ActorProxyyMap.SetAt( newID, newEnitty );

	return newEnitty;
}

void EActorMgr::Update(float deltaTime)
{
	POSITION pos = m_ActorProxyyMap.GetStartPosition();
	TYPE_ACTOR_PROXY_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_ActorProxyyMap.GetNext(pos);
		itr->m_value->Update(deltaTime);
	}
}

void EActorMgr::Clear()
{
	POSITION pos = m_ActorProxyyMap.GetStartPosition();
	TYPE_ACTOR_PROXY_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_ActorProxyyMap.GetNext(pos);
		SAFE_DELETE( itr->m_value );
	}
}