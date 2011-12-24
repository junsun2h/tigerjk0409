#pragma once

#include "EEntity.h"
#include <atlcoll.h>
#include <set>



class EEntityMgr
{
public:
	EEntityMgr();
	~EEntityMgr();

	IEntity*	SPawn(CEntityProxyCreateParam param);
	bool		IsUsingPerUpdate(IEntity* pEntity);

private:
	typedef	ATL::CAtlMap<long, IEntity*>	TYPE_ENTITY_MAP;
	typedef	std::set<IEntity*>	TYPE_ENTITY_SET;

	TYPE_ENTITY_MAP		m_EntityMap;
	TYPE_ENTITY_SET		m_PerFrameUpdateEntities;
};