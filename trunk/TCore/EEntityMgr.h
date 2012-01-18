#pragma once

#include <atlcoll.h>

#include "IEntityMgr.h"
#include "EEntity.h"
#include "EEntityProxyActor.h"
#include "EEntityProxyCamera.h"
#include "EEntityProxyRender.h"



template<class T>
class EEntityMap
{
public:
	T* SPawn(std::string name)
	{
		long newID = GET_HASH_KEY( name );
		ATL::CAtlMap<long, T*>::CPair* pObject = m_Map.Lookup( newID );
		if( pObject != NULL )
		{
			assert(0);
			return pObject->m_value;
		} 

		T* newObject = new T(name, newID);
		m_Map.SetAt( newID, newObject );

		return newObject;
	}

	void Remove(long id)
	{
		ATL::CAtlMap<long, T*>::CPair* pObject = m_Map.Lookup( id );
		if( pObject != NULL )
		{
			SAFE_DELETE( pObject->m_value );
			m_Map.RemoveKey(id);
		} 
	}

	T* Find(long id)
	{
		ATL::CAtlMap<long, T*>::CPair* pObject = m_Map.Lookup( id );
		if( pObject != NULL )
			return pObject->m_value;

		return NULL;
	}

	void RemoveAll()
	{
		POSITION pos = m_Map.GetStartPosition();
		ATL::CAtlMap<long, T*>::CPair* itr = NULL;

		while (pos)
		{
			itr = m_Map.GetNext(pos);
			SAFE_DELETE( itr->m_value );
		}
	}

	ATL::CAtlMap<long, T*>*	GetMap()	{ return &m_Map;}

private:
	ATL::CAtlMap<long, T*>	m_Map;
};


class EEntityMgr : public IEntityMgr
{
public:
	IEntity*			SpawnEntity(std::string name) override;
	IEntityProxy*		SpawnEntityProxy(std::string name, ENTITY_PROXY_TYPE type) override;

	IEntity*			GetEntity(std::string name) override	{ return GetEntity( GET_HASH_KEY(name) );}
	IEntity*			GetEntity(long id) override				{ return m_EntityMap.Find( id );}

	void				RemoveEntity(long id) override;
	void				RemoveEntity(const char* name) override	{ return RemoveEntity( GET_HASH_KEY(name) ); }
	void				RemoveEntityProxy(long id, ENTITY_PROXY_TYPE type) override;

	void				RemoveAllEntity() override;
	void				RemoveAllEntityProxy(ENTITY_PROXY_TYPE type) override;

	void				Destroy() override;

private:
	EEntityMap<EEntity>				m_EntityMap;
	EEntityMap<EEntityProxyActor>	m_ActorMap;
	EEntityMap<EEntityProxyCamera>	m_CameraMap;
	EEntityMap<EEntityProxyRender>	m_RendererMap;
};
