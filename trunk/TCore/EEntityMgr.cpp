#include <atlcoll.h>

#include "CGrowableArray.h"
#include "CAABB.h"
#include "CUnitPool.h"
#include "CResource.h"

#include "IEntity.h"
#include "IEntityMgr.h"

#include "EEntity.h"
#include "EEntityMgr.h"


EEntityMgr::EEntityMgr()
	: m_MemPoolEntity(1000)
{

}

IEntity* EEntityMgr::SpawnEntity(std::string name)
{
	long newID = GET_HASH_KEY( name );
	ENTITY_MAP::CPair* pEntity = m_EntityMap.Lookup( newID );
	if( pEntity != NULL )
	{
		assert(0);
		return pEntity->m_value;
	} 

	IEntity* pNewEntity = m_MemPoolEntity.GetNew();
	pNewEntity->Init(name, newID);

	m_EntityMap.SetAt( newID, pNewEntity );

	return pNewEntity;
}

void EEntityMgr::RemoveEntity(long id)
{	 
	ENTITY_MAP::CPair* pEntity = m_EntityMap.Lookup( id );
	if( pEntity != NULL )
	{
		m_MemPoolEntity.Remove(pEntity->m_value);
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
		m_MemPoolEntity.Remove(itr->m_value);
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