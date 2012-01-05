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
			m_Map.RemoveKey(id);
		} 
	}

	void Clear()
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
private:
	IEntity*			SpawnEntity(std::string name) override;
	IEntityProxy*		SpawnEntityProxy(std::string name, ENTITY_PROXY_TYPE type) override;

	void				RemoveEntity(long id) override;
	void				RemoveEntityProxy(long id, ENTITY_PROXY_TYPE type) override;

	void				ClearEntity() override;
	void				ClearEntityProxy(ENTITY_PROXY_TYPE type) override;

private:
	EEntityMap<EEntity>				m_EntityMap;
	EEntityMap<EEntityProxyActor>	m_ActorMap;
	EEntityMap<EEntityProxyCamera>	m_CameraMap;
	EEntityMap<EEntityProxyRender>	m_RenderObjectMap;
};