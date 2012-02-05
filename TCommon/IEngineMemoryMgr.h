#pragma once


#include "CResource.h"
#include "IEntity.h"
#include "IEntityProxy.h"



struct IEngineMemoryMgr
{
	virtual CResourceBase*	GetNewResource(eRESOURCE_TYPE type) = 0;
	virtual void			RemoveResource(CResourceBase* pResource) = 0;

	virtual IEntity*		GetNewEntity() =0;
	virtual void			RemoveEntity(IEntity* pEntity) =0;

	virtual IEntityProxy*	GetNewProxy(eENTITY_PROXY_TYPE type) =0;
	virtual void			RemoveProxy(IEntityProxy* pProxy) = 0;
};