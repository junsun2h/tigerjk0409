#include "EEntityMgr.h"


IEntity* EEntityMgr::SpawnEntity(std::string name)
{
	return m_EntityMap.SPawn(name);
}

IEntityProxy* EEntityMgr::SpawnEntityProxy(std::string name, ENTITY_PROXY_TYPE type)
{
	return NULL;
}

void EEntityMgr::RemoveEntity(long id)
{	 
	m_EntityMap.Remove(id);	 
}	 
	 
void EEntityMgr::RemoveEntityProxy(long id, ENTITY_PROXY_TYPE type)
{	 
}	 
	 
void EEntityMgr::ClearEntity()
{	 
	m_EntityMap.Clear();
}	 
	 
void EEntityMgr::ClearEntityProxy(ENTITY_PROXY_TYPE type)
{

}