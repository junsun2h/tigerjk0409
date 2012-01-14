#pragma once

#include "IEntity.h"

struct IEntityMgr
{
	virtual IEntity*		SpawnEntity(std::string name) =0;
	virtual IEntityProxy*	SpawnEntityProxy(std::string name, ENTITY_PROXY_TYPE type)  =0;

	virtual IEntity*		GetEntity(std::string name) =0;
	virtual IEntity*		GetEntity(long id) =0;

	virtual void			RemoveEntity(long id)  =0;
	virtual void			RemoveEntityProxy(long id, ENTITY_PROXY_TYPE type)  =0;

	virtual void			RemoveAllEntity()  =0;
	virtual void			RemoveAllEntityProxy(ENTITY_PROXY_TYPE type)  =0;

	virtual	void			Destroy() = 0;
};