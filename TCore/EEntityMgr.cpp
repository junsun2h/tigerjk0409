#include "EEntityMgr.h"
#include "EEngine.h"


IEntity* EEntityMgr::SpawnEntity(std::string name)
{
	long newID = GET_HASH_KEY( name );
	ENTITY_MAP::CPair* pObject = m_EntityMap.Lookup( newID );
	if( pObject != NULL )
	{
		assert(0);
		return pObject->m_value;
	} 

	EEntity* newObject = g_Engine.EngMemoryPoolMgr()->GetNewEntity();
	newObject->Init(name, newID);

	m_EntityMap.SetAt( newID, (EEntity*)newObject );

	return newObject;
}

void EEntityMgr::RemoveEntity(long id)
{	 
	ENTITY_MAP::CPair* pObject = m_EntityMap.Lookup( id );
	if( pObject != NULL )
	{
		g_Engine.EngMemoryPoolMgr()->RemoveEntity(pObject->m_value);
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
	return NULL; 
}