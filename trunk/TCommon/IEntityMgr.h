#pragma once

#include "IEntity.h"

struct IEntityMgr
{
	virtual IEntity*		SpawnEntity(std::string name) =0;

	virtual IEntity*		GetEntity(std::string name) =0;
	virtual IEntity*		GetEntity(long id) =0;

	virtual void			RemoveEntity(long id)  =0;
	virtual void			RemoveEntity(const char* name)  =0;

	virtual void			RemoveAllEntity()  =0;

	virtual	void			Destroy() = 0;
};


struct IEntityProxyMgr
{
	virtual IEntityProxy*	SpawnEntityProxy(std::string name, eENTITY_PROXY_TYPE type)  =0;
	virtual void			RemoveEntityProxy(long id, eENTITY_PROXY_TYPE type)  =0;
	virtual void			RemoveAllEntityProxy(eENTITY_PROXY_TYPE type)  =0;

	virtual	void			Destroy() = 0;
};