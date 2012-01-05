#pragma once

#include "IEntity.h"

struct IEntityMgr
{
	virtual IEntity*		SpawnEntity(std::string name) =0;
	virtual IEntityProxy*	SpawnEntityProxy(std::string name, ENTITY_PROXY_TYPE type)  =0;

	virtual void			RemoveEntity(long id)  =0;
	virtual void			RemoveEntityProxy(long id, ENTITY_PROXY_TYPE type)  =0;

	virtual void			ClearEntity()  =0;
	virtual void			ClearEntityProxy(ENTITY_PROXY_TYPE type)  =0;
};