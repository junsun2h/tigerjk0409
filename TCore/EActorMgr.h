#pragma once

#include "IEntity.h"
#include "IActorMgr.h"
#include <atlcoll.h>


class EActorMgr : public IActorMgr
{
public:
	IEntityProxyActor*	SPawn(std::string& name) override;
	void				Update(float deltaTime) override;
	void				Clear() override;

private:
	typedef	ATL::CAtlMap<long, IEntityProxyActor*>	TYPE_ACTOR_PROXY_MAP;

	TYPE_ACTOR_PROXY_MAP	m_ActorProxyyMap;
};