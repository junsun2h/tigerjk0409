#include "EEntityMgr.h"
#include "EEngine.h"


IEntity* EEntityMgr::SpawnEntity(std::string name)
{
	long newID = GET_HASH_KEY( name );
	ENTITY_MAP::CPair* pEntity = m_EntityMap.Lookup( newID );
	if( pEntity != NULL )
	{
		assert(0);
		return pEntity->m_value;
	} 

	EEntity* pNewEntity = g_Engine.EngMemoryPoolMgr()->GetNewEntity();
	pNewEntity->Init(name, newID);

	m_EntityMap.SetAt( newID, (EEntity*)pNewEntity );

	return pNewEntity;
}

void EEntityMgr::RemoveEntity(long id)
{	 
	ENTITY_MAP::CPair* pEntity = m_EntityMap.Lookup( id );
	if( pEntity != NULL )
	{
		g_Engine.EngMemoryPoolMgr()->RemoveEntity(pEntity->m_value);
		m_EntityMap.RemoveKey(id);
	}
}	 
	 	 
void EEntityMgr::RemoveAllEntity()
{	 
	POSITION pos = m_EntityMap.GetStartPosition();
	ENTITY_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_EntityMap.GetNext(pos);
		g_Engine.EngMemoryPoolMgr()->RemoveEntity(itr->m_value);
	}

	m_EntityMap.RemoveAll();
}	 
	 
void EEntityMgr::Destroy()
{
	RemoveAllEntity();
}

IEntity* EEntityMgr::GetEntity(long id) 
{
	ENTITY_MAP::CPair* pEntity = m_EntityMap.Lookup( id );
	if( pEntity != NULL )
		return pEntity->m_value;

	return NULL;
}

void EEntityMgr::SaveXML(IEntity* pEntity)
{

}