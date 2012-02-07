#include "EGlobal.h"
#include "EEntityMgr.h"


IEntity* EEntityMgr::SpawnEntity(std::string name)
{
	long newID = GET_HASH_KEY( name );
	ENTITY_MAP::CPair* pEntity = m_EntityMap.Lookup( newID );
	if( pEntity != NULL )
	{
		assert(0);
		return pEntity->m_value;
	} 

	IEntity* pNewEntity = GLOBAL::EngineMemoryMgr()->GetNewEntity();
	pNewEntity->Init(name, newID);

	m_EntityMap.SetAt( newID, pNewEntity );

	return pNewEntity;
}

void EEntityMgr::RemoveEntity(long id)
{	 
	ENTITY_MAP::CPair* pEntity = m_EntityMap.Lookup( id );
	if( pEntity != NULL )
	{
		GLOBAL::EngineMemoryMgr()->RemoveEntity(pEntity->m_value);
		m_EntityMap.RemoveKey(id);
	}
}	 
	 	 
void EEntityMgr::RemoveAllEntity()
{	
	// delete safely whole entities
	// There is possibility to delete entity in entity
	while(m_EntityMap.GetCount() != 0)
	{
		POSITION pos = m_EntityMap.GetStartPosition();
		ENTITY_MAP::CPair* itr = m_EntityMap.GetNext(pos);
		GLOBAL::EngineMemoryMgr()->RemoveEntity(itr->m_value);
		m_EntityMap.RemoveKey(itr->m_key);
	}
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