#include "EEntityMgr.h"


IEntity* EEntityMgr::SpawnEntity(std::string name)
{
	return m_EntityMap.SPawn(name);
}

IEntityProxy* EEntityMgr::SpawnEntityProxy(std::string name, ENTITY_PROXY_TYPE type)
{
	switch(type)
	{
	case ENTITY_PROXY_ACTOR: return m_ActorMap.SPawn(name);
	case ENTITY_PROXY_RENDER: return m_RendererMap.SPawn(name);
	case ENTITY_PROXY_CAMERA: return m_CameraMap.SPawn(name);
	default:
		return NULL;
	}
	return NULL;
}

void EEntityMgr::RemoveEntity(long id)
{	 
	m_EntityMap.Remove(id);	 
}	 
	 
void EEntityMgr::RemoveEntityProxy(long id, ENTITY_PROXY_TYPE type)
{	 
	switch(type)
	{
	case ENTITY_PROXY_ACTOR: return m_ActorMap.Remove(id);
	case ENTITY_PROXY_RENDER: return m_RendererMap.Remove(id);
	case ENTITY_PROXY_CAMERA: return m_CameraMap.Remove(id);
	default:
		assert(0);
	}
}	 
	 
void EEntityMgr::RemoveAllEntity()
{	 
	m_EntityMap.RemoveAll();
}	 
	 
void EEntityMgr::RemoveAllEntityProxy(ENTITY_PROXY_TYPE type)
{
	switch(type)
	{
	case ENTITY_PROXY_ACTOR: return m_ActorMap.RemoveAll();
	case ENTITY_PROXY_RENDER: return m_RendererMap.RemoveAll();
	case ENTITY_PROXY_CAMERA: return m_CameraMap.RemoveAll();
	default:
		assert(0);
	}
}

void EEntityMgr::Destroy()
{
	m_ActorMap.RemoveAll();
	m_RendererMap.RemoveAll();
	m_CameraMap.RemoveAll();
	RemoveAllEntity();
}

