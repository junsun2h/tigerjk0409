#include "EEntityProxyMgr.h"
#include "EEngine.h"


IEntityProxy* EEntityProxyMgr::SpawnEntityProxy(std::string name, eENTITY_PROXY_TYPE type)
{
	EEngineMemoryMgr* pPool = g_Engine.EngMemoryPoolMgr();

	if ( type == ENTITY_PROXY_ACTOR )
	{
		EEntityProxyActor* pActor = (EEntityProxyActor*)pPool->GetNewProxy( ENTITY_PROXY_ACTOR );
		pActor->Init(name, GET_HASH_KEY(name) );
		m_EntityProxyMap[ENTITY_PROXY_ACTOR].SetAt( pActor->GetID(), pActor );
		return pActor;
	}
	else if( type == ENTITY_PROXY_CAMERA )
	{
		EEntityProxyCamera* pCamera = (EEntityProxyCamera*)pPool->GetNewProxy( ENTITY_PROXY_CAMERA );
		pCamera->Init(name, GET_HASH_KEY(name) );
		m_EntityProxyMap[ENTITY_PROXY_CAMERA].SetAt( pCamera->GetID(), pCamera );
		return pCamera;
	}
	else if( type == ENTITY_PROXY_RENDER )
	{
		EEntityProxyRender* pRenderUnit = (EEntityProxyRender*)pPool->GetNewProxy( ENTITY_PROXY_RENDER );
		pRenderUnit->Init(name, GET_HASH_KEY(name) );
		m_EntityProxyMap[ENTITY_PROXY_RENDER].SetAt( pRenderUnit->GetID(), pRenderUnit );
		return  pRenderUnit;
	}

	assert(0);
	return NULL;
}

void EEntityProxyMgr::RemoveEntityProxy(long id, eENTITY_PROXY_TYPE type)
{
	ENTITY_PROXY_MAP::CPair* pProxy = m_EntityProxyMap[type].Lookup(id);
	if( pProxy != NULL )
	{
		g_Engine.EngMemoryPoolMgr()->RemoveProxy(pProxy->m_value);
		m_EntityProxyMap[type].RemoveKey(id);
	}
	else
	{
		assert(0);
	}
}

void EEntityProxyMgr::RemoveAllEntityProxy(eENTITY_PROXY_TYPE type)
{
	EEngineMemoryMgr* pPool = g_Engine.EngMemoryPoolMgr();

	POSITION pos = m_EntityProxyMap[type].GetStartPosition();
	ENTITY_PROXY_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_EntityProxyMap[type].GetNext(pos);
		pPool->RemoveProxy(itr->m_value);
	}

	m_EntityProxyMap[type].RemoveAll();
}

void EEntityProxyMgr::Destroy()
{
	for( int i=0; i< NUM_ENTITY_PROXY_TYPE; ++i)
		RemoveAllEntityProxy( eENTITY_PROXY_TYPE(i) );
}
