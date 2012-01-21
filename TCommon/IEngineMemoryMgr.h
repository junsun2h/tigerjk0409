#pragma once


#include "CResource.h"
#include "IEntity.h"
#include "IEntityProxy.h"



struct IEngineMemoryMgr
{
	virtual CResourceBase*	GetNewResource(eRESOURCE_TYPE type) = 0;
	virtual void			RemoveResource(CResourceBase* pResource) = 0;
};