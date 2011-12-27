#pragma once

#include "EEntity.h"
#include "IEntityMgr.h"
#include <atlcoll.h>
#include <set>



class EEntityMgr : public IEntityMgr
{
public:
	EEntityMgr();
	~EEntityMgr();

	IEntity*			SPawn(std::string& name) override;
	void				Clear() override;

private:
	typedef	ATL::CAtlMap<long, IEntity*>	TYPE_ENTITY_MAP;
	typedef	std::set<IEntity*>	TYPE_ENTITY_SET;

	TYPE_ENTITY_MAP		m_EntityMap;
};