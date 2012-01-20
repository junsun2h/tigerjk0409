#pragma once

#include <atlcoll.h>

#include "IEntityMgr.h"
#include "EEntityProxyActor.h"
#include "EEntityProxyCamera.h"
#include "EEntityProxyRender.h"


class EEntityProxyMgr : public IEntityProxyMgr
{
public:
	IEntityProxy*		SpawnEntityProxy(std::string name, eENTITY_PROXY_TYPE type) override;
	void				RemoveEntityProxy(long id, eENTITY_PROXY_TYPE type) override;
	void				RemoveAllEntityProxy(eENTITY_PROXY_TYPE type) override;

	void				Destroy() override;

private:
	typedef ATL::CAtlMap<long, IEntityProxy*>	ENTITY_PROXY_MAP;

	ENTITY_PROXY_MAP	m_EntityProxyMap[NUM_ENTITY_PROXY_TYPE];
};
